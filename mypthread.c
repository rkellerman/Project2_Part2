//mypthread.c

#include "mypthread.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ucontext.h>



typedef struct threadNode{

	struct Node * prev;
	mypthread_t * thread;
	struct Node * next;

}Node;

//Global

// Type your globals...

Node * head, * tail, * temp;
int numNodes = 0;
int current_tid = 1;
int numThreads = 1;

// Type your own functions (void)
// e.g., free up sets of data structures created in your library

mypthread_t * getHead(){
	return (head->thread);
}

mypthread_t * getTail(){
	return (tail->thread);
}

int create(){
	temp = (Node*)malloc(sizeof(Node));
	temp->prev = NULL;
	temp->next = NULL;
	numNodes++;

	return 0;
}

int enqueue(mypthread_t * thread){
	if (head == NULL){
		create();
		head = temp;
		tail = temp;
	}
	else {
		create();
		tail->next = temp;
		temp->prev = tail;
		tail = temp;
		tail->next = head;
		head->prev = tail;
	}
	temp->thread = thread;

	return 0;
}

mypthread_t * find(int tid){
	temp = head;
	while (temp != NULL){
		if (temp->thread->tid == tid){
			return temp->thread;
		}
		else {
			temp = temp->next;
		}
	}
	// not found
	exit(0);
}

mypthread_t * findActive(int tid){
	temp = head;
	while (temp != NULL){
		if (temp->thread->tid == tid){
			break;
		}
		else {
			temp = temp->next;
		}
	}
	if (temp->next != NULL){
		temp = temp->next;
		while (temp != NULL){
			if (temp->thread->state == ACTIVE){
				return temp->thread;
			}
			else {
				temp = temp->next;
			}
		}
		exit(0);
	}
	exit(0);

}

/*  Write your thread create function here...*/
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *arg){


	if (numNodes == 0){
		mypthread_t * main = (mypthread_t*)malloc(sizeof(mypthread_t));
		main->tid = numThreads;
		numThreads++;
		ucontext_t * context = (ucontext_t*)malloc(sizeof(ucontext_t));
		main->context = context;
		main->context->uc_stack.ss_sp = (char*)malloc(sizeof(char) * 4096);
		main->context->uc_stack.ss_size = 4096;
		main->state = ACTIVE;
		enqueue(main);
	}

	ucontext_t * context = (ucontext_t*)malloc(sizeof(ucontext_t));
	thread->context = context;
	getcontext(thread->context);
	thread->context->uc_stack.ss_sp = (char*)malloc(sizeof(char) * 4096);
	thread->context->uc_stack.ss_size = 4096;
	thread->state = ACTIVE;
	thread->tid = numThreads;
	numThreads++;
	makecontext(thread->context, (void(*)())start_routine, 1, arg);
	enqueue(thread);

	// printf("Thread created..\n");

	return 0;

}




/* Write your thread exit function here...*/
void mypthread_exit (void * retval){

	// printf("Exiting thread...\n");

	mypthread_t * current = find(current_tid);
	current->state = DEAD;
	free(current->context);

	if (current->from != 0){
		mypthread_t * from = find(current->from);
		from->state = ACTIVE;
	}

	mypthread_t * next = findActive(current->tid);
	if (current_tid == next->tid){
		return;
	}
	current_tid = next->tid;
	setcontext(next->context);

}


/* Write your thread yield function here... */
int mypthread_yield (void){


	mypthread_t * current = find(current_tid);
	mypthread_t * next = findActive(current->tid);

	if (current_tid == next->tid){
		return 0;
	}

	current_tid = next->tid;
	swapcontext(current->context, next->context);
	return 0;

}

/*  Write your thread join function here...*/
int mypthread_join (mypthread_t thread, void ** retval){

	// printf("Joining thread...\n");

	int tid = thread.tid;
	mypthread_t * current = find(current_tid);
	mypthread_t * with = find(thread.tid);

	if (with->state != ACTIVE){
		return 0;
	}
	current->state = BLOCKED;
	with->from = current_tid;
	current_tid = tid;
	swapcontext(current->context, with->context);

	return 0;

}

/* Write whatever function is left here ....*/
void mypthread_whatever (/*...*/){}
