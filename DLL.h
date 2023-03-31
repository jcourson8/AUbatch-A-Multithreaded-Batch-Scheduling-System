#ifndef DLL_H
#define DLL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#define FCFS 0
#define SJF 1
#define PRIORITY 2

struct node{  
    struct job  *thisjob;  
    struct node *prev;  
    struct node *next;
}; 

typedef struct node node;

struct job {
    char * NAME;
    int CPU_TIME;
	int PRI;
	struct timeval ARRIVAL_TIME;
	const char * PROGRESS;
	struct timeval START_TIME; // (gets changed by dispatcher)
	struct timeval FINISH_TIME;
};

typedef struct job job;

/* attributes... */
typedef struct {
    struct job  *thisjob;
    struct node *head, *tail;
    bool issorted;
} DLL;


/* Shape's operations (Shape's interface)... */
void DLL_ctor(DLL * const me);
node * DLL_gethead(DLL const * const me);
node * DLL_gettail(DLL const * const me);
bool DLL_getissorted(DLL const * const me);
void DLL_sethead(DLL * const me, node * newhead);
void DLL_settail(DLL * const me, node * newtail);
void DLL_setissorted(DLL * const me, bool issortedval);
void DLL_addnode(DLL * const me, job *curjob);
void DLL_display(DLL * const me);
void DLL_displaydone(DLL * const me);
void DLL_sortlist(DLL * const me, int policy);
bool DLL_isempty(DLL * const me);
int DLL_length(DLL * const me);
int DLL_waitingjobs(DLL * const me);
void DLL_insertfirst(DLL * const me, job *curjob);
// void DLL_sortedinsert(DLL * const me, job *curjob, int policy);
int cmptime(struct timeval *t1, struct timeval *t2); 
void DLL_removedone(DLL * const me);
// node * DLL_consumetail(DLL * const me);
#endif /* DLL_H */

