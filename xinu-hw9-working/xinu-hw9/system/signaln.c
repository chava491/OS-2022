/**
 * @file signaln.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup semaphores
 *
 * Signal a semaphore @p count times, releasing @p count waiting threads.
 *
 * @param sem
 *      Semaphore to signal.
 * @param count
 *      Number of times to signal, which must be positive.
 *
 * @return
 *      ::OK on success, ::SYSERR on failure.  This function can only fail if @p
 *      sem did not specify a valid semaphore of if @p count was not positive.
 */
syscall signaln(semaphore sem, int count)
{
    register struct semtabent *semptr;
    int c;
    irqmask im;

    im = disable();
    if (isbadsem(sem) || (count <= 0))
    {
        restore(im);
        return SYSERR;
    }

    /**
     * TODO: Increment the semaphore 'count' times.
     * If the count was negative, wake waiting threads.
     * Yield the processor when the work is done.
     */
    semptr = &semtab[sem];					// To obtain the semaphore we require, we look it up in the semaphore table semtab[] 
    int temp = semptr->count;
    semptr->count+=count;
    
    if(temp < 0){ 						// Here we check if the semaphore count is negative.
								// If so, that means |count| threads are waiting on the semaphore.	
	for (c = temp; c < 0; c++) { 				// We wake the processes until the count is no longer 0.
		pid_typ waitingsem =  dequeue(semptr->queue);   // Dequeue a thread from the waiting threads list of the semaphore.
 		ready(waitingsem, RESCHED_YES);     		// Call ready() to wake that thread and change its state.
                // waitingsem->state = "SFREE";			// Redundant because ready() already changes the state. --"DO NOT INCLUDE THIS"	- Dr.Brylow
	}		
    }

    resched();
    restore(im);
    return OK;
}
