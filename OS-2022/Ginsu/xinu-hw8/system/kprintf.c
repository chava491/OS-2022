/*
 * COSC 3250 - Project # 3
 * This is the method of our os that will allow us to read in characters as well as manage the flag register so as to print chars out when dr is not busy.
 * @Salvador Guel and Sierra Thomas
 * Dr. Dennis Brylow
 * TA-BOT:MAILTO salvador.guel@marquette.edu sierra.thomas@mu.edu 
 */

/**
 * @file kprintf.c
 */

/* Embedded Xinu, Copyright (C) 2009, 2022.  All rights reserved. */

#include <xinu.h>
#include <pl011.h>

/**
 * Synchronously read a character from a UART.  This blocks until a character is
 * available.  The interrupt handler is not used.
 *
 * @return
 *      The character read from the UART cast to an <code>int</code>.
 *      If ASCII value 4 is seen, return special constant EOF instead.
 */
syscall kgetc(void)
{
    volatile struct pl011_uart_csreg *regptr;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    // TODO: Check UART flags register, and
    //       once the receiver is not empty, get and return character c.
    //       If ASCII value 4 is seen, return special constant EOF instead.

	while (regptr->fr & PL011_FR_RXFE){} //This while loop continously checks to see if the flag register is set to the binary number that means it is empty and once it says it is empty it will exit the while loop and continue to run the next code.
		int c;
		c = regptr->dr;
		if (c == '4') {	//This if else statement reads what char is in the reigster and checks to see if it is an available char and if it is it will return it, but if it is the EOT ASCII Value 4 then it will return EOF.
			return EOF;
		}
		else {
			return c;
		}
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */

syscall kcheckc(void)
{
// TODO: Check the UART for characters, and return true or false.
    volatile struct pl011_uart_csreg *regptr;
    regptr = (struct pl011_uart_csreg *)0x3F201000;
	//This if else statement returns a 0 aka false if the flag registers indicates that the register is empty and a 1 aka true if the flag register indicates the register is not empty aka has a char.
	if(regptr->fr & PL011_FR_RXFE) {
		return 0;
	}
	else{
		return 1;
	}
}

/**
 * Synchronously write a character to a UART.  This blocks until the character
 * has been written to the hardware.  The interrupt handler is not used.
 *
 * @param c
 *      The character to write.
 *
 * @return
 *      The character written to the UART.
 */

syscall kputc(uchar c)
{
    volatile struct pl011_uart_csreg *regptr;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    // TODO: Check UART flags register.
    //       Once the Transmitter FIFO is not full, send character c.
    // 		look at pl011.h and the link in the project description
    //       Special handling -- if c is newline, transmit carriage return
    //       followed by newline.

	//The while loop will exit once the transmit FIFO is empty and then it will return a carriage return and wait for the transmit register to be empty again through a while loop and then return \n or if the previous condition was not met then it simply returned the char that is in the register.
	while (regptr->fr & PL011_FR_TXFF) {}	// Once the Transmitter FIFO is not full (aka it will break out of the look when the FIFO is empty.
			if (c == '\n') {  	//Special handling -- if c is newline, transmit carriage return followed by newline.
				c = '\r';
				regptr->dr = c;

				while (regptr->fr & PL011_FR_TXFF) {}
				
				c ='\n';
				regptr->dr = c;
			}
			
			else {
				regptr->dr = c; //send character c.
			}
return (int)c;
}

/**
 * kernel printf: formatted, synchronous output to SERIAL0.
 *
 * @param format
 *      The format string.  Not all standard format specifiers are supported by
 *      this implementation.  See _doprnt() for a description of supported
 *      conversion specifications.
 * @param ...
 *      Arguments matching those in the format string.
 *
 * @return
 *      The number of characters written.
 */

syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;
    
    irqmask im;
    im = disable();

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(int, int))kputc, 0);
    va_end(ap);
    
	restore(im);
	return retval;
}
