<<<<<<< HEAD:libuthread/queue_tester.c
#include "queue.h"
#include <assert.h>
#include <stdlib.h>
=======
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
>>>>>>> 77745f3dea14f0d6a737d8307413f83e73858c38:progs/queue_tester.c
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");
	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *int_ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&int_ptr);
	printf(">> test: simple en/dequeue: item returned is %d\n", *int_ptr);
	TEST_ASSERT(int_ptr == &data);
}

/* Callback function that increments items by a certain value */
static int inc_item(void *data, void *arg)
{
    int *a = (int*)data;
    int inc = (int)(long)arg;

    *a += inc;

    return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(void *data, void *arg)
{
    int *a = (int*)data;
    int match = (int)(long)arg;

    if (*a == match)
        return 1;

    return 0;
}

/* Callback funtion that allows you to print the items in the queue */
int print_queue(void *data, void *arg) {
	int *a = (int*)data;
	char *b = (char*)arg;
	
	printf("%d, ", *a);
	//printf("hi there\n");
	return 0;
}

void test_iterator(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int *ptr;
    fprintf(stderr, "*** TEST queue_iterate ***\n");
    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < (int)sizeof(data) / (int)sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    printf(">> Test 1: print queue:\n");
    queue_iterate(q, print_queue, NULL, NULL);
    //printf("this is where we are getting to\n");

    /* Add value '1' to every item of the queue */
    printf("\n>> Test 2: increment every item:\n");
    queue_iterate(q, inc_item, (void*)1, NULL);
    queue_iterate(q, print_queue, NULL, NULL);
    assert(data[0] == 2);
    assert(data[9] == 11);

    printf("\n>> Test 3: find 5:\n");
    /* Find and get the item which is equal to value '5' */
    ptr = NULL;
    queue_iterate(q, find_item, (void*)5, (void**)&ptr);
    TEST_ASSERT(ptr != NULL);
    TEST_ASSERT(*ptr == 5);
    TEST_ASSERT(ptr == &data[3]);
}

void test_empty_dequeue(void)
{
	void *ptr;
	queue_t empty_q = queue_create();
	fprintf(stderr, "*** TEST empty dequeue ***\n");
	
	TEST_ASSERT(queue_dequeue(empty_q, &ptr) == -1);
}

void test_destroy_full_queue(void)
{
	queue_t q = queue_create();
	int data[] = {1, 2};

	fprintf(stderr, "*** TEST destroy non-empty queue ***\n");
    for (int i = 0; i < (int)sizeof(data) / (int)sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);	
    printf("current queue: ");
    queue_iterate(q, print_queue, NULL, NULL);
    printf("\n");
    TEST_ASSERT(queue_destroy(q) == -1);
}

void test_destroy_queue(void)
{
	fprintf(stderr, "*** TEST destroy empty queue ***\n");
	queue_t q = queue_create();
	TEST_ASSERT(q != NULL);
	TEST_ASSERT(queue_destroy(q) == 0);
}

void test_enqueue_null(void)
{
	queue_t q = queue_create();
	fprintf(stderr, "*** TEST enqueue NULL ***\n");
	TEST_ASSERT(queue_enqueue(q, NULL) == -1);	
}

void test_delete(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    //int *ptr;
    fprintf(stderr, "*** TEST queue_delete ***\n");
    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < (int)sizeof(data) / (int)sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);
    queue_enqueue(q, &data[4]);

    printf("current queue: ");
    queue_iterate(q, print_queue, NULL, NULL);
    printf("\n");

    printf(">> Test 1: delete first item in queue &data[0]\n");
    TEST_ASSERT(queue_delete(q, (void*)&data[0]) == 0);
    printf("current queue: ");
    queue_iterate(q, print_queue, NULL, NULL);
    printf("\n");

    printf(">> Test 2: delete &data[4] (there are two in the queue)\n");
	TEST_ASSERT(queue_delete(q, (void*)&data[4]) == 0);
    printf("current queue: ");
    queue_iterate(q, print_queue, NULL, NULL);
    printf("\n");

    printf(">> Test 3: delete &data[4] (there is one in the queue)\n");
    TEST_ASSERT(queue_delete(q, (void*)&data[4]) == 0);
    printf("current queue: ");
    queue_iterate(q, print_queue, NULL, NULL);
    printf("\n");

    printf(">> Test 4: failed deletetion of &data[10]+1\n");
    TEST_ASSERT(queue_delete(q, (void*)(&data[10] + 1)) == -1);

    printf(">> Test 5: deleting NULL\n");
    TEST_ASSERT(queue_delete(q, NULL) == -1);	

    //remove one of these test cases and add one at the beginning where the first time is deleted
}

void test_length()
{
	queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 5, 9, 10};
    int i;
    int *ptr;
    fprintf(stderr, "*** TEST queue_length ***\n");
    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < (int)sizeof(data) / (int)sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    printf(">> Test 1: length of 10 queue\n");
	printf("current queue: ");
    queue_iterate(q, print_queue, NULL, NULL);
    printf("\n");
    TEST_ASSERT(queue_length(q) == 10);

    printf(">> Test 2: length of 8 queue\n");
    printf("current queue: ");
    queue_iterate(q, print_queue, NULL, NULL);
    printf("\n");
    queue_dequeue(q, (void **)&ptr);
    queue_dequeue(q, (void **)&ptr);
    TEST_ASSERT(queue_length(q) == 8);
}

int main(void)
{

	test_create();
	test_queue_simple();
	test_iterator();
	test_empty_dequeue();
	test_destroy_queue();
	test_destroy_full_queue();
	test_enqueue_null();
	test_delete();
	test_length();

	return 0;
}

int main(void){
	test_create();
	test_queue_simple();

}