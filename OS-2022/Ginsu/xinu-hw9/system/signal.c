/**
 * @file signal.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup semaphores
 *
 * Signal a semaphore, releasing up to one waiting thread.
 *
 * @param sem
 *      Semaphore to signal.
 *
 * @return
 *      ::OK on success, ::SYSERR on failure.  This function can only fail if @p
 *      sem did not specify a valid semaphore.
 */
syscall signal(semaphore sem)
{
    register struct semtabent *semptr;
    irqmask im;

    im = disable();
    if (isbadsem(sem))
    {
        restore(im);
        return SYSERR;
    }

    /**
     * TODO: Increment the semaphore count.
     * If the count was negative, wake a waiting thread.
     */
    semptr = &semtab[sem];				// To obtain the semaphore we require, we look it up in the semaphore table semtab[].
    
    if(semptr->count < 0) {				// Check to see if the count for the semaphore is negative
	pid_typ waitingsem = dequeue(semptr->queue);     // -> dequeue a waiting thread to get its pid.
 	ready(waitingsem, RESCHED_YES);                 // -> Wake the waiting thread - this also changes its state.
    }
    
    semptr->count++;
    
    restore(im);
    return OK;
}
