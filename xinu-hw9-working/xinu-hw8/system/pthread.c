/**
 * @file pthread.c
 * Internal Xinu functions implementing system calls for PThread API.
 * @ Salvador Guel, Ryan Duffy,  and Sierra Thomas
 * Dr.Dennis Brylow
 * @file pthread.c
 * @provides pthread
 * TA-BOT:MAILTO salvador.guel@marquette.edu sierra.thomas@mu.edu ryan.a.duffy@marquette.edu 
*/
/* Embedded Xinu, Copyright (C) 2009, 2022.  All rights reserved. */

#include <xinu.h>

/* for syscall argument parsing */
#define SCARG(type, args)  (type)(*args++)

/* 
 * syscall pthread_create(pthread_t *thread, pthread_attr_t *attr,
 *               void *(*start_routine)(void *), void *arg);
 */
syscall sc_create(int *args)
{
    pthread_t    *thread = SCARG(pthread_t *, args);
    pthread_attr_t *attr = SCARG(pthread_attr_t *, args);       /* ignore */
    void  *start_routine = SCARG(void *, args);
    void            *arg = SCARG(void *, args);

    /**
     * TODO: Translate the pthread_create() call into the equivalent
     * call of our existing create() function for spawning new processes.
     * Use defaults INITSTK and INITPRIO for initial stack size and lottery
     * tickets.  Don't forget to use ready() to move the new process into
     * the PRREADY state.
     */
    *thread = create(start_routine, INITSTK, INITPRIO,"PThread0", 1, arg);
    ready((int )thread, RESCHED_YES);
    return OK;
}


/* 
 * syscall pthread_join(pthread_t thread, void **retval);
 */
syscall sc_join(int *args)
{
    irqmask im;
    im = disable();
    pthread_t thread = SCARG(pthread_t, args);
    void    **retval = SCARG(void **, args);       /* ignore */

    int ps;
    register pcb *ppcb;
    ppcb = &proctab[currpid]; //set ppcb to address of currpid
    /**
     * TODO: Move the calling process into the PRJOIN state,
     * Enqueue it on the JOIN queue of thread's PCB, and
     * yield the processor.
     */
    
    ppcb->state = PRJOIN; //ppcb in ready state
    enqueue(currpid,proctab[thread].joinq); //enqueue currpid to ppcb joinq
    restore(im);
    resched();
    return OK;
}

/*
 * syscall pthread_mutex_lock(pthread_mutex_t *mutex);
 */
syscall sc_lock(int *args)
{
    pthread_mutex_t *mutex = SCARG(pthread_mutex_t *, args);

    /**
     * TODO: Use the atomic CAS operation to secure the mutex lock.
     */
     //calls compare and swap to check if thread can be lcoked
    while(_atomic_compareAndSwapStrong(mutex, PTHREAD_MUTEX_UNLOCKED, PTHREAD_MUTEX_LOCKED) == FALSE) 
    {
	resched();
    }   
    return OK;
}

/*
 * syscall pthread_mutex_unlock(pthread_mutex_t *mutex);
 */
syscall sc_unlock(int *args)
{
    pthread_mutex_t *mutex = SCARG(pthread_mutex_t *, args);

    /**
     * TODO: Release the mutex lock.
     */
    mutex = PTHREAD_MUTEX_UNLOCKED; //unlocks thread
    return OK;
}