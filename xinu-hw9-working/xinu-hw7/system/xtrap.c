/*
 * COSC 3250 - Project # 4
 * What we added to xtrap.c(): Here we simply added a trap for 4 SWI codes for now. We do this by checking if it first a SWI exception and then we get the code for the swi code it is asking for which will in turn do the kernel privelege the user requested and return the result to user mode.
 * @Salvador Guel and Sierra Thomas
 * Dr. Dennis Brylow
 * TA-BOT:MAILTO salvador.guel@marquette.edu sierra.thomas@mu.edu 
 */
/**
 * @file xtrap.c 
 * @provides xtrap
 *
 */
/* Embedded XINU, Copyright (C) 2007, 2020.  All rights reserved. */

#include <xinu.h>

char *trap_names[] = {
    "Reset",
    "Undefined Instruction",
    "Software Interrupt (SWI)",
    "Prefetch Abort",
    "Data Abort",
    "IRQ Interrupt Request",
    "FIQ Interrupt Request"
};

/**
 * Generic XINU Trap (Interrupt) Handler.
 *
 * @param cause the contents of the cause register used to decode the trap
 * @param frame pointer to the interrupt frame with saved status
 *
 */
void xtrap(long *frame, int cause)
{
    /* Decode and handle IRQ separately. */
    if (cause == ARM_EXCEPTION_IRQ)
    {
        dispatch();
        return;
    }

    /* TODO: Implement system calls for Xinu.
     * 1) Recognize when cause of hardware exception is SWI opcode,
     * 2) Find the responsible SWI opcode in memory,
     * 3) Decode what system call was requested by examining opcode,
     * 4) Call syscall_dispatch() with the syscall number and any
     *    passed arguments, and
     * 5) On return from syscall_dispatch(), setup proper return to
     *    instruction after SWI call, with return value in place.
     */

     else if (cause == ARM_EXCEPTION_SWI) { //This checks to see if the cause of the interrupt was a software interrupt
	int *adress = (int *)frame[14]; //Here I am creating a pointer that points to the adress of word 14 that is in the stack that xtrap() points to.
	int swicode = *(adress); //Here, I dereference the previously made pointer tha point to adress 14 so as to get the contents of this register.
	swicode = swicode & 0xFFFFFF; //Here I mask off the neccessary bits to decode the SWI code that was called.
 	frame[14] += 4; //I am going up one word to the next register (word 15) due to the fact that it will return out of the interrupt. If this is not done, then
			// a loop will happen that keeps calling the same swi code.
	frame[0] = syscall_dispatch(swicode, (int*)frame); //Now here, we save the return value to register 0.
	return; //we return out of this function
     }

    /* If not an IRQ or SWI, fall through to generic exception handler */
    kprintf("\r\n\r\nXINU Exception [%s]\r\n", trap_names[cause]);

    kprintf("Faulting address: 0x%08X\r\n", frame[14]);
    kprintf("Faulting SPSR:    0x%08X\r\n", frame[15]);

    kprintf("[0x%08X] r12:0x%08X   lr:0x%08X\r\n",
            frame + CTX_IP, frame[CTX_IP], frame[13]);
    kprintf("[0x%08X]  r8:0x%08X   r9:0x%08X  r10:0x%08X  r11:0x%08X\r\n",
            frame + CTX_R8,
            frame[CTX_R8], frame[CTX_R9], frame[CTX_R10], frame[CTX_R11]);
    kprintf("[0x%08X]  r4:0x%08X   r5:0x%08X   r6:0x%08X   r7:0x%08X\r\n",
            frame + CTX_R4,
            frame[CTX_R4], frame[CTX_R5], frame[CTX_R6], frame[CTX_R7]);
    kprintf("[0x%08X]  r0:0x%08X   r1:0x%08X   r2:0x%08X   r3:0x%08X\r\n",
            frame + CTX_R0,
            frame[CTX_R0], frame[CTX_R1], frame[CTX_R2], frame[CTX_R3]); 
   while (1)
        ;                       /* forever */
}
