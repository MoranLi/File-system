
/*
Authors:
   Jaden Ball, jjb465, 11195900
   Mahmud Azam, mfa256, 11211602
*/

/*
   Reference:
   We are using the LIST library written by Derek Perrin and Dominic McKeith
*/

#include "monitor.h"
#include </student/cmpt332/pthreads/os.h>
#include "list.h"
#include <stdio.h>

/* The Singleton Monitor: */
Monitor *mon = NULL;

int base_sem;
int num_threads;

/*
  Initializes the monitor and its data structures
*/
void MonInit(){
    int i;
    num_threads = 0;
    base_sem = NewSem(1);
    mon = (Monitor *)malloc(sizeof(Monitor));

    mon->enter = NewSem(1); /* We only allow one thread to enter at first */
    mon->enter_q = ListCreate();
    mon->num_entering = 0;
    mon->urg = NewSem(0); /* Threads can only wait on the urgent queue */
    mon->urg_q = ListCreate();
    mon->num_urgent = 0;

    for(i = 0; i < NUM_CONDITIONS; i++) {
        int new_sem;
        new_sem = NewSem(0); /* No thread can continue upon wait */
        mon->conditions[i].num_waiting = 0;
        mon->conditions[i].cond_sem = new_sem;
        mon->conditions[i].pids = ListCreate();
    }
}

/*
  Signals that the process entered the critical section
*/
void MonEnter(){
    PID *my;
    my = malloc(sizeof(PID));
    *my = MyPid();

    P(base_sem);
    mon->num_entering++;
    ListPrepend(mon->enter_q, my);
    V(base_sem);

    /* Enter monitor: */
    P(mon->enter);
    num_threads++;
    printf("Num threads running: %d\n", num_threads);
}

void leave_or_wait() {
    if(ListCount(mon->urg_q) > 0
            && mon->num_urgent > 0){ /* At least one thread in urgent queue */
        PID *pid;
        pid = ListTrim(mon->urg_q);
        if(NULL == pid) {
            fprintf(stderr, "ListTrim(urg_q) returns NULL when count > 0\n");
            exit(1);
        }
        free(pid);
        mon->num_urgent--;
        V(mon->urg);
    } else if(ListCount(mon->enter_q) > 0
            && mon->num_entering > 0) {
        PID *pid;
        pid = ListTrim(mon->enter_q);
        if(NULL == pid) {
            fprintf(stderr, "ListTrim(enter_q) returns NULL when count > 0\n");
            exit(1);
        }
        free(pid);
        mon->num_entering--;
        V(mon->enter);
    } else {
        ; /* No waiting threads */
    }
}

/*
  Signals that the process left the critical section
*/
void MonLeave(){
    /* Let a thread from the enter or the urgent queue to run: */
    num_threads--;
    printf("Num threads running: %d\n", num_threads);
    leave_or_wait();
}

/*
  Suspends the process until the condition variable is available
*/
void MonWait(int cond) {
    PID *my;
    my = malloc(sizeof(PID));
    *my = MyPid();

    ListPrepend(mon->conditions[cond].pids, my);
    mon->conditions[cond].num_waiting++;
    num_threads--;
    printf("Num threads running: %d\n", num_threads);

    /* Let a thread from the enter or the urgent queue to run: */
    leave_or_wait();
    P(mon->conditions[cond].cond_sem); /* Wait for the condition */
}

/*
  Resumes the process that was waiting the longest for condition
  variable cond.
*/
void MonSignal(int cond){
    if(ListCount(mon->conditions[cond].pids) > 0
            && mon->conditions[cond].num_waiting > 0){
        PID *my, *pid;
        my = malloc(sizeof(PID));
        *my = MyPid();
        ListPrepend(mon->urg_q, my);
        mon->num_urgent++;

        /* There is a thread waiting on the given condition: */
        mon->conditions[cond].num_waiting--;
        pid = ListTrim(mon->conditions[cond].pids);
        if(NULL == pid) {
            fprintf(stderr,
                "ListTrim(condition) returns NULL when count > 0\n");
            exit(1);
        }
        free(pid);
        V(mon->conditions[cond].cond_sem);
        P(mon->urg);
        num_threads++;
        printf("Num threads running: %d\n", num_threads);
    } else {
        ; /* No threads to signal */
    }
}
