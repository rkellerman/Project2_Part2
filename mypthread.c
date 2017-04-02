//mypthread.c

#include "mypthread.h"


typedef struct threadNode{

// Type your commands

}Node;

//Global

// Type your globals...

// Type your own functions (void)
// e.g., free up sets of data structures created in your library


/*  Write your thread create function here...*/
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *arg){


}


/* Write your thread exit function here...*/
void mypthread_exit (void * retval){}


/* Write your thread yield function here... */
int mypthread_yield (void){}

/*  Write your thread join function here...*/
int mypthread_join (mypthread_t thread, void ** retval){}

/* Write whatever function is left here ....*/
void mypthread_whatever (/*...*/){}
