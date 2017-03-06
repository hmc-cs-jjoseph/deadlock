#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

void signal_handler(int signum) {
	fprintf(stdout, "Caught a signal: %d\n", signum);
	if (signum == SIGINT) exit(1);
}

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void *thread_func(void *arg) {
	fprintf(stdout, "Thread: acquiring lock 2\n");
	pthread_mutex_lock(&lock2);
	fprintf(stdout, "Thread: acquired lock 2\nThread: attempting to acquire lock 1 ...\n");
	fprintf(stdout, "RIP\n");
	pthread_mutex_lock(&lock1);

	return NULL;
}

int main() {
	signal(SIGINT, signal_handler);
	pthread_t thread;


	fprintf(stdout, "Main: acquiring lock 1\n");
	pthread_mutex_lock(&lock1);													/* Main thread grabs the first lock */
	fprintf(stdout, "Main: creating new thread\n");
	pthread_create(&thread, NULL, thread_func, NULL);		/* Main spawns a thread */
	fprintf(stdout, "Main: yielding to new thread\n");
	sched_yield();																			/* Now, a context switch happens... */

	/* New thread grabs the second lock
	 * and then the first -- oops!
	 * Now the new thread has to wait for
	 * the main thread to release lock 1,
	 * but the main thread is waiting for
	 * the new thread to release lock 2.
	 */

	pthread_join(thread, NULL);
	pthread_mutex_lock(&lock2);													/* Main thread can't acquire lock 2 */
	
	fprintf(stdout, "Main: returning from main\n");
	return 0;
}
