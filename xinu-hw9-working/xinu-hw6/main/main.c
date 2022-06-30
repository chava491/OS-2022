/**
 * COSC 3250 - Project # 3
 * This is the method of our os that will allow us to read in characters as well as manage the flag register so as to print chars out when dr is not busy.
 * @Salvador Guel and Sierra Thomas
 * Dr. Dennis Brylow
 * TA-BOT:MAILTO salvador.guel@marquette.edu sierra.thomas@mu.edu 
 */

/**
 * @file     main.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2022.  All rights reserved. */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLEN 127

/**
 * Main process.  You can modify this routine to customize what Embedded Xinu
 * does when it starts up.
 */
void testcases(void);

process main(void) {
	kprintf("Hello Xinu World!\n");
	testcases();
	kprintf("===TEST END===");
return 0;
}
