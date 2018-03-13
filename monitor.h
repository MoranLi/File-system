
/*
Authors:
   Jaden Ball, jjb465, 11195900
   Mahmud Azam, mfa256, 11211602
*/

#include "list.h"

#ifndef MONITOR_H
#define MONITOR_H


/* Number of condition variables: */
#define NUM_CONDITIONS 32

/* Condition variable: */
typedef struct Cond {
    LIST *pids;
    int num_waiting;
    int cond_sem;
} Cond;

/* Monitor: */
typedef struct Monitor {
    LIST *enter_q;
    LIST *urg_q;
    Cond conditions[NUM_CONDITIONS];
    int num_entering;
    int enter;
    int num_urgent;
    int urg;
} Monitor;


/* The singleton monitor: */
extern Monitor *mon;

/*
  Initializes the monitor and its data structures
*/
void MonInit();

/*
  Signals that the process entered the critical section
*/
void MonEnter();

/*
  Signals that the process left the critical section
*/
void MonLeave();

/*
  Suspends the process until the condition variable is available
*/
void MonWait(int cond);

/*
  Resumes the process that was waiting the longest for condition
  variable cond.
*/
void MonSignal(int cond);

#endif
