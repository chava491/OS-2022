/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>
static int lotteryScheduling(void);

extern void ctxsw(void *, void *);
/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */
syscall resched(void)
{
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */
    oldproc = &proctab[currpid];
    if (PRCURR == oldproc->state){
		oldproc->state = PRREADY;
		enqueue(currpid, readylist);
    }
    /* place current process at end of ready queue
     * We recommend you use a helper function for the following:
     * TODO: Get the total number of tickets from all processes that are in current and ready states.
     * Utilize the random() function to pick a random ticket. 
     * Traverse through the process table to identify which proccess has the random ticket value.
     * Remove process from queue.
     * Set currpid to the new process.
     */
//two for loops and a couple of if statements.
//
    //  currpid = dequeue(readylist);
    currpid = lotteryScheduling();
    remove(currpid); //This removes our process from the ready list
    newproc = &proctab[currpid];
    newproc->state = PRCURR;    /* mark it currently running    */

#if PREEMPT
    preempt = QUANTUM;
#endif

    ctxsw(&oldproc->stkptr, &newproc->stkptr);

    /* The OLD process returns here when resumed. */
    return OK;
}

static int lotteryScheduling(void) {
   int totaltickets = 0;
   int i = 0;
   int ticketSum = 0; //keeps track of current total tickets in second for loop
   for(i = 0; i < NPROC; i++) {
	pcb *ppcb = &proctab[i];
 	
        if ((PRCURR == ppcb->state) || (PRREADY == ppcb->state)) {
                totaltickets += ppcb->tickets;      
        }
    }

    int nticket = random(totaltickets);
    for(i = 0; i < NPROC; i++) {
	pcb *ppcb = &proctab[i];
	if ((PRCURR == ppcb->state) || (PRREADY == ppcb->state)) {
        	ticketSum = ticketSum + ppcb->tickets;
		if(ticketSum > nticket) {
        		break;
	        }
	}
	     
    }
	return i;
}
