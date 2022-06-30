/**
 * @file create.c
 * @provides create, newpid, userret
 * COSC 3250 - Project # 3
 * This create.c allows us to to create processes and with the help of calling ctxsw.S, it is also able to create mutiple processes and switch between them. Think of it as the Fork() function in regular c.
 * @ Salvador Guel and Sierra Thomas
 * Dr.Dennis Brylow
 * @file testcases.c
 * @provides testcases
 * TA-BOT:MAILTO salvador.guel@marquette.edu sierra.thomas@mu.edu
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

static pid_typ newpid(void);
void userret(void);
//void *getstk(ulong);
//---would have it here ------------- xinu-hw7
void *getmem(ulong);
/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */

//TODO: add parameter for tickets to the syscall create.
syscall create(void *funcaddr, ulong ssize, uint tickets, char *name, ulong nargs,  ... ) {
    ulong *saddr;               /* stack address                */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */
    if (ssize < MINSTK)
        ssize = MINSTK;
    ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;
    /* round up to even boundary    */
//    saddr = (ulong *)getstk(ssize);     /* allocate new stack and pid   */
//----- would have it here----------------------xinu-hw7
    memblk *node = getmem(ssize);
    saddr = (ulong *)node + node->length;
    
    pid = newpid();
    /* a little error checking      */
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid)) {
        return SYSERR;
    }

    numproc++;
    ppcb = &proctab[pid];
	
	// TODO: Setup PCB entry for new process.
		strncpy(ppcb->name, name, strlen(name));
		//ppcb->stklen = ssize;
		//ppcb->stkbase = saddr - ssize/sizeof(ulong);
		ppcb->stklen = node->length;
		ppcb->stkbase = saddr;
		ppcb->state = PRSUSP;
		ppcb->stkptr = NULL;
		ppcb->tickets = tickets;
	//This is where we initialize all of the variables defined in the beginning of the proc.h file located in the include directory.
	//pccb->state = PRFREE; //Good. make sure the right sideis the right state that we are supposed to set it too.
	
    /* Initialize stack with accounting block. */
    *saddr = STACKMAGIC;
    *--saddr = pid;
    *--saddr = ppcb->stklen;
    *--saddr = (ulong)ppcb->stkbase;

    /* Handle variable number of arguments passed to starting function   */
    //due to this being an arm processor it only accepts stacks with a chunk of 8 bytes. Thus, this is what the following if statement does. If there is a certain number of extra arguments included, more memory is allocated for the processor to save the results in the stack 
	if (nargs > 4) {
        	pads = ((nargs - 1) / 4) * 4;
	}
    /* If more than 4 args, pad record size to multiple of native memory */
    /*  transfer size.  Reserve space for extra args                     */
    	for (i = 0; i < pads; i++){        
		*--saddr = 0;
	}
	// TODO: Initialize process context.
	// First, we want to set different values of our stack to be important things, these three things are values of our stack pointer.
	// saddr[some spot in the stack] =funcaddr
	// saddr[some spot] = userret
		
	ulong*argaddr = saddr;
	//I create the space for the extra arguements
	for (i=0;i < CTX_PC;i++) {
		saddr--;
		*saddr = 0;
	}
		saddr[CTX_SP] = (ulong)userret; //Here we are setting the value in this register to be the processes return adress.This userret is defined in the directory include and the vim file proc.h.
		//saddr[15] = (ulong)procaddr;
		saddr[CTX_LR] = (ulong)funcaddr; 
	        saddr [CTX_PC] = ARM_MODE_SYS |  ARM_F_BIT; 
	// TODO:  Place arguments into activation record.
	//        See K&R 7.3 for example using va_start, va_arg and
	//        va_end macros for variable argument functions.
		
		va_start(ap, nargs); //use va_start for nargs
		//The following for loop will decide where the first 4 elements go and then the activation record.
		for (i = 0; i < nargs; i++) { 
			if (i<4){
				saddr[i] = va_arg(ap,int);
			}
			else {
				
				argaddr[i-4] = va_arg(ap,int);
			}
		}

		va_end(ap);
		ppcb->stkptr = saddr;

	//First four arguments in stack, registerss r0-r3
	//All other arguments, are stored in stack above registers.
     
   return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return     */
    static pid_typ nextpid = 0;

    for (pid = 0; pid < NPROC; pid++){                           /* check all NPROC slots    */
        nextpid = (nextpid + 1) % NPROC;
        if (PRFREE == proctab[nextpid].state) {
            return nextpid;
        }
    }
    return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void) {
    kill(currpid);
}
