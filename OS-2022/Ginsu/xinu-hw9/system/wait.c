/**
 * @file wait.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup semaphores
 *
 * Wait on a semaphore.
 *
 * If the semaphore's count is positive, it will be decremented and this
 * function will return immediately.  Otherwise, the currently running thread
 * will be put to sleep until the semaphore is signaled with signal() or
 * signaln(), or freed with semfree().
 *
 * @param sem
 *      Semaphore to wait on.
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail if @p
 *      sem did not specify a valid semaphore.
 */
syscall wait(semaphore sem)
{
    register struct semtabent *semptr;
    register pcb *procptr;
    irqmask im;

    im = disable();
    if (isbadsem(sem))
    {
        restore(im);
        return SYSERR;
    }

    /*
     * THIS IS THE SEMAPHORE STRUCT IN xinu-hw9/include/semaphore.h
     * struct semtabent
     * {
     *     char state;                 the state SFREE or SUSED 
     *     int count;                  count for this semaphore
     *     qid_typ queue;              waiting processes
     *     };   
     *     extern struct semtabent semtab[]; 
     */

    /**
     * TODO:
     * Decrement the semaphore count.  If it becomes negative,
     * put the process in a PRWAIT state and wait in the semaphore
     * queue.
     */

    semptr = &semtab[sem];		 // We look up the semaphore in the global semtab[] table.
    procptr = &proctab[currpid];   	 // We look up the process in the global proctab[] table.

    semptr->count--;			 // We decrement the semaphore count.

    if (semptr->count < 0) {		 // Check if the semaphore count becomes negative with the just made decrement.
	procptr->state = PRWAIT;         // -> Change the state of the process to PRWAIT.
	enqueue(currpid, semptr->queue); // ->Enqueue this waiting thread to the semaphore waiting thread queue.
    	resched();
    }

    restore(im);
    return OK;
}
