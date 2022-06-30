/**
 * @file uartGetc.c
 */
/* Embedded Xinu, Copyright (C) 2018. All rights reserved. */

#include <xinu.h>

extern struct uart serial_port;


/**
 * Asynchrounously read a character from the UART.
 *
 * @return
 *      The character read from the UART.
 */
devcall uartGetc(device *devptr) {
    uchar c;
    irqmask im = disable();

    /* TODO:
     * Asynchronously read a character from the UART.
     * 1. Wait for input using the appropriate semaphore. --> This is where we use the wait or signal. 
     *     --> I believe we have to use wait since we are waiting for a character and thuse would want to do it Asynchronously.
     * 2. Store (aka put it into the c character we declared here from uart buffer) the first byte in the input buffer (as a character) to later be returned.
     * 3. Then, decrement the counter of bytes in the input buffer.
     * 4. Increment the index of the input buffer's first byte with respect to the total length of the UART input buffer.
     * 	   --> With respect to the UART input buffer just means that it cannot exceed the max limit of the UART buffer which is a global constant called
     * 	   -->  UART_IBLEN. Thus, we simply use this as our MOD factor. If it exceeds the limit of the buffer, the remainder will take us back to the start.
     */
	
        /* 1 -- wait for input using the wait semaphore*/
    	wait(serial_port.isema);
	
	/* 2 -- store the first byte of the buffer in char c */
        c = serial_port.in[serial_port.istart];			       // istart is declared in the uart struct as "Index of the first byte"
	
	/* 3 -- Decrement the counter */
    	serial_port.icount--;					       // icount is a ushort defined in the uart struct as "Bytes in buffer"
        /* 4 -- Increment the index of the input buffers first byte */
    	serial_port.istart = (serial_port.istart + 1) % UART_IBLEN;    // UART_IBLEN is defined in the UART struct as the mac number of bytes for input buffer.
    	restore(im);
    	return c;
//First we get a character in the bottom half
//Take that character put it in buffer
//istart = first byte initialized as 0;
//icount = number of bytes in buffer;
//UART_IBLEN = max
//we get 3 characetrs and they are buffered;
//icount = 3
//istart = 0;
//tail of the list = istart+icount => this index is the next empty slot to put a new character into the buffer.
//istart++; = 1
//icount--;
}
