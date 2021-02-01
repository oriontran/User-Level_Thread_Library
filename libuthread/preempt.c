#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"

#define HZ 100
sigset_t block_alarm;
int init;

void signal_handler(int signal){
	uthread_yield();
}

void preempt_disable(void)
{
	sigprocmask (SIG_BLOCK, &block_alarm, NULL);
}

void preempt_enable(void)
{
	sigprocmask (SIG_UNBLOCK, &block_alarm, NULL);
}

void preempt_start(void)
{
	if(init == 0) {
		sigemptyset(&block_alarm);
		init = 1;
	}
	struct sigaction sa;
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaddset(&sa.sa_mask, SIGVTALRM);
	if (sigaction(SIGVTALRM, &sa, NULL) < 0)
		perror("sigaction");
	struct itimerval timer;
	timer.it_interval.tv_usec = 10000;
	timer.it_interval.tv_sec = 0;
	timer.it_value.tv_usec = 10000;
	timer.it_value.tv_sec = 0;
	if (setitimer(ITIMER_VIRTUAL, &timer, NULL) < 0)
		perror("setitimer");
}
