
#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

enum state {
	READY,
	BLOCKED,
};

struct TCB {
	uthread_t tid;
	uthread_ctx_t context;
	enum state thread_state;
	void* top_of_stack;
	int retval;
	uthread_t pair_tid;
};
typedef struct TCB TCB;

// Global variables
uthread_t thread_ID = 0;
queue_t ready_queue;
queue_t blocked_queue;
queue_t zombie_queue;
TCB* active_thread;

static int find_matching_pair_tid (void *data, void *arg){
	TCB* temp_thread = (TCB*)data;
	int thread_id = *(int *)arg;
	if (temp_thread->pair_tid == thread_id)
		return 1;
	return 0;
}

static int find_matching_tid (void *data, void *arg){
	TCB* temp_thread = (TCB*)data;
	int thread_id = *(int *)arg;
	if (temp_thread->tid == thread_id)
		return 1;
	return 0;
}

TCB* init_thread(){
	TCB* new_thread = (TCB*)calloc(1, sizeof(TCB));
	new_thread->tid = thread_ID;
	new_thread->thread_state = READY;
	new_thread->top_of_stack = uthread_ctx_alloc_stack();
	thread_ID++;
	return new_thread;

};

void uthread_yield(void)
{
	preempt_disable();
	TCB* oldest_thread;
	TCB* curr_thread = active_thread;

	if(active_thread->thread_state != BLOCKED){
		queue_enqueue(ready_queue, active_thread);
	}
	queue_dequeue(ready_queue, (void**)&oldest_thread);
	active_thread = oldest_thread;
	uthread_ctx_switch(&curr_thread->context, &oldest_thread->context);
	preempt_enable();
}

uthread_t uthread_self(void)
{
	return active_thread->tid;
}

int uthread_create(uthread_func_t func, void *arg)
{
	if ((int)thread_ID == 0){ // The context of the main thread does not need to be initialized
		preempt_start();
		preempt_disable();
		ready_queue = queue_create();
		blocked_queue = queue_create();
		zombie_queue = queue_create();
		TCB *main_thread = (TCB*)malloc(sizeof(TCB));
		main_thread->tid = thread_ID;
		thread_ID++;
		main_thread->thread_state = READY;
		main_thread->top_of_stack = NULL;
		active_thread = main_thread;
	}
	preempt_disable();
	TCB *new_thread = init_thread();
	if(!new_thread || (int)thread_ID > 65535) // Max number of threads
		return -1;
	uthread_ctx_init(&new_thread->context, new_thread->top_of_stack, (uthread_func_t) func, arg);
	queue_enqueue(ready_queue, new_thread);
	preempt_enable();
	return new_thread->tid;
}

void uthread_exit(int retval)
{
	preempt_disable();
	TCB *next_thread;
	TCB *reference_thread = NULL;
	active_thread->retval = retval;
	queue_iterate(blocked_queue, find_matching_pair_tid, (void*)&active_thread->tid, (void**)&reference_thread);
	if(reference_thread != NULL) { // We have found a matching TID, and join is now complete
		reference_thread->thread_state = READY;
		reference_thread->retval = retval;
		queue_enqueue(ready_queue, reference_thread);
		queue_delete(blocked_queue, reference_thread);

	}
	else {
		queue_enqueue(zombie_queue, active_thread);
	}
	if(queue_length(ready_queue) > 0){ // Do not do a context switch if there are no more threads
		queue_dequeue(ready_queue, (void**) &next_thread);
		uthread_ctx_t store_old_context = active_thread->context;
		active_thread = next_thread;
		preempt_enable();
		uthread_ctx_switch(&store_old_context, &next_thread->context);
	}
}

int uthread_join(uthread_t tid, int *retval)
{
	preempt_disable();
	if((int)tid == 0 || (active_thread->tid == tid) || tid > thread_ID){
		preempt_enable();
		return -1;
	}
	TCB *reference_thread = NULL;
	queue_iterate(zombie_queue, find_matching_tid, (void*)&active_thread->tid, (void**)&reference_thread);
	if(reference_thread != NULL) { // Iterate through zombie_queue to check if paired thread is there
		if(reference_thread->retval != NULL) {
			*retval = reference_thread->retval;
		}
		queue_delete(zombie_queue, reference_thread);
		uthread_ctx_destroy_stack(reference_thread->top_of_stack);
	}
	else {
		active_thread->thread_state = BLOCKED;
		active_thread->pair_tid = tid;
		queue_enqueue(blocked_queue, active_thread);
	}
	uthread_yield();
	if(active_thread->retval != NULL) {
		*retval = active_thread->retval;
	}
	preempt_enable();
	return 0;
}
