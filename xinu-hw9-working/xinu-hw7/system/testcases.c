/**
 * @file testcases.c
 * @provides testcases
 *
 * TA-BOT:MAILTO salvador.guel@marquette.edu
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

extern void main(int, char *);

int testmain(int argc, char **argv)
{
    int i = 0;
    kprintf("Hello XINU World!\r\n");

    for (i = 0; i < 10; i++)
    {
        kprintf("This is process %d\r\n", currpid);

        user_yield();
    }
    return 0;
}

void testbigargs(int a, int b, int c, int d, int e, int f, int g, int h)
{
    kprintf("Testing bigargs...\r\n");
    kprintf("a = 0x%08X\r\n", a);
    kprintf("b = 0x%08X\r\n", b);
    kprintf("c = 0x%08X\r\n", c);
    kprintf("d = 0x%08X\r\n", d);
    kprintf("e = 0x%08X\r\n", e);
    kprintf("f = 0x%08X\r\n", f);
    kprintf("g = 0x%08X\r\n", g);
    kprintf("h = 0x%08X\r\n", h);
}

void printpcb(int pid)
{
    pcb *ppcb = NULL;

    /* Using the process ID, access it in the PCB table. */
    ppcb = &proctab[pid];

    /* Printing PCB */
    kprintf("Process name		  : %s \r\n", ppcb->name);

    switch (ppcb->state)
    {
    case PRFREE:
        kprintf("State of the process	  : FREE \r\n");
        break;
    case PRCURR:
        kprintf("State of the process 	  : CURRENT \r\n");
        break;
    case PRSUSP:
        kprintf("State of the process	  : SUSPENDED \r\n");
        break;
    case PRREADY:
        kprintf("State of the process	  : READY \r\n");
        break;
    default:
        kprintf("ERROR: Process state not correctly set!\r\n");
        break;
    }

    /* Print PCB contents and registers */
    kprintf("Base of run time stack    : 0x%08X \r\n", ppcb->stkbase);
    kprintf("Stack length of process   : %8u \r\n", ppcb->stklen);
}

void lotterytest(void)
{
		ready(create((void *)testmain, INITSTK,5, "MAIN1",2, 0, NULL),
              RESCHED_YES);
        	ready(create((void *)testmain, INITSTK,6, "MAIN2",2, 0, NULL),
              RESCHED_YES);
}

void PrintFreeList(void){
	memblk *node = freelist.head;
	while(node != NULL){
		//kprintf("Memblock #: 0x%x Length: %lu Base: %lu Bound: %lu \r\n", node, node->length, freelist.base, freelist.bound);
		kprintf("Freelist.head: 0x%08x Length: %lu Base: %lu Bound: %lu Next: 0x%08x \r\n", node, freelist.size, freelist.base, freelist.bound, node->next);
		node = node->next;
	}
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;

    kprintf("0) Test user_none syscall\r\n");
    kprintf("1) Test user_getc syscall\r\n");
    kprintf("2) Test user_putc syscall\r\n");
    kprintf("3) Create three processes that test user_yield syscall\r\n");
    kprintf("4) Call lotterytest(void) - Create two processes\r\n");
    kprintf("5) Print FreeList\r\n");
    kprintf("6) Calling getmem twice with n bytes\r\n");
    kprintf("7) Calling getmem(1000) 3 times and then freemem() run twice (described when ran)\r\n");
    kprintf("8) Test Malloc - Description prints out.\r\n");
    kprintf("9) Test Malloc/Free - Description prints out.\r\n");
    kprintf("===TEST BEGIN===\r\n");

    // TODO: Test your operating system!
	enable();
  	c = disable();
  	kprintf("cpsr = 0x%08X\r\n", c);
	c = kgetc();
    switch (c)
    {
    case '0':

        // Test user_none
        kprintf("This is a test of ...");
        user_none();
        kprintf("user_none() syscall\r\n");
        break;

    case '1':
        kprintf("Enter another character through user_getc() syscall :");
        c = user_getc(0);
        kprintf("\r\nCharacter entered was \'%c\'\r\n", c);
        break;

    case '2':
        kprintf("Echo characters until 'X': ");
        while ((c = user_getc(0)) != 'X')
        {
            user_putc(0, c);
        }
        kprintf("\r\n");
        break;

    case '3':
        // Create three copies of a process, and let them play.
	ready(create((void *)testmain, INITSTK,6, "MAIN1", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK,6, "MAIN2", 2, 0, NULL),
              RESCHED_NO);
	ready(create((void *)testmain, INITSTK,7, "MAIN3", 2, 0, NULL),
              RESCHED_YES);
        while (numproc > 1)
            resched();
        break;	
    case '4':
	lotterytest();
	while (numproc > 1)
	    resched();
    case '5' :
	PrintFreeList();    
	break;
    case '6' :
	kprintf("Freelist: ");
	PrintFreeList();
	kprintf("Number of bites used in getmem(): 1000 and 8\r\n");
	getmem(1000);
	PrintFreeList();
	getmem(8);
	PrintFreeList();
	break;
    case '7' :
	kprintf("The original free List:\r\n");
	PrintFreeList();
	ulong *first = getmem(1000);
	kprintf("Number first = 0x%x\r\n", first);
	kprintf("--------------------After 1st getmem(1000)--------------------\r\n");
	PrintFreeList();
	ulong *second = getmem(1000);
	kprintf("--------------------After 2nd getmem(1000)--------------------\r\n");
	PrintFreeList();
	getmem(500);
	kprintf("--------------------After 3rd getmem(500)--------------------\r\n");
	PrintFreeList();
	kprintf("----------------freemem(0x%08x, 1000) is called:----------------\r\n", first);
	freemem(first, 1000);
	PrintFreeList();
	kprintf("----------------freemem(0x%08x, 500) is called:----------------\r\n", second);
	freemem(second, 500);
	PrintFreeList();
	break;
    case '8' :
	kprintf("The original free List:\r\n");
        PrintFreeList();
        malloc(1000);
        kprintf("--------------------After malloc(1000)--------------------\r\n");
	PrintFreeList();
        malloc(500);
        kprintf("--------------------After malloc(500)--------------------\r\n");
        PrintFreeList();
        malloc(2000);
        kprintf("--------------------After malloc(2000)--------------------\r\n");
        PrintFreeList();
	break;
    default:
    case '9' :
	kprintf("The original free List:\r\n");
        PrintFreeList();
        ulong *firstmalloc = malloc(1000);
        kprintf("--------------------After 1st malloc(1000)--------------------\r\n");
        kprintf("Address of the 1st malloc = 0x%08x\r\n", firstmalloc);
	PrintFreeList();
        ulong *secondmalloc = malloc(1000);
        kprintf("--------------------After 2nd malloc(1000)--------------------\r\n");
        PrintFreeList();
        ulong *thirdmalloc = malloc(1000);
        kprintf("--------------------After 3rd malloc(1000)--------------------\r\n");
        PrintFreeList();
        kprintf("-------------free(0x%08x) is called -- 1st malloc:-------------\r\n", firstmalloc);
        free(firstmalloc);
        PrintFreeList();
        kprintf("-------------free(0x%08x) is called -- 2st malloc:-------------\r\n", secondmalloc);
        free(secondmalloc);
        PrintFreeList();
	kprintf("-------------free(0x%08x) is called -- 3st malloc:-------------\r\n", thirdmalloc);
        free(thirdmalloc);
        PrintFreeList();
	break;
	}
	return;
}

