/*
 * @file uartPutc.c
 */

/* Embedded Xinu, Copyright (C) 2018. All rights reserved. */

#include <xinu.h>

extern struct uart serial_port;


/**
 * Asynchronously write a character to the UART.
 *
 * @param c
 *      The character to write.
 *
 * @return OK on success
 */
devcall uartPutc(device *devptr, char c) {
    irqmask im = disable();
    
/*
 *struct uart{
 *Pointers to associated structures
 * volatile void *csr;
 * Control & status registers
 * UART input fields 
 * uchar iflags;                Input flags                        
 * semaphore isema;             Count of input bytes ready         
 * ushort istart;               Index of first byte                
 * ushort icount;               Bytes in buffer                    
 * uchar in[UART_IBLEN];        Input buffer                       
 *
 * UART output fields
 * uchar oflags;                Output flags                       
 * semaphore osema;            Count of buffer space free         
 * ushort ostart;              Index of first byte                
 * ushort ocount;              Bytes in buffer                    
 * uchar out[UART_OBLEN];      Output buffer                      
 * volatile bool oidle;        UART transmitter idle              
 * };

extern struct uart serial_port;
 */

    /* TODO:
     * 1) First, check if the UART transmitter is idle (see include/uart.h for UART control block information).  If it is idle, set its state to
     * busy (hint: not idle), then write the character to the UART control
     * and status register's data register.  
     * If it is not idle, use the appropriate UART semaphore to wait for the UART's buffer space to be
     * free.  
     * Next, place the character into the UART output buffer with
     * respect to the index of the buffer's first byte, amount of bytes in
     * buffer, and total length of the UART output buffer.  Then, increment
     * the counter of bytes in the output buffer.
     *
     * Note: This is an ouput semaphore.
     * Note: The output semaphore is uartGetc.c 
     */  
 
    if (serial_port.oidle == 1){ //write character to uart control and status reigesters
	serial_port.oidle = 0;
	((struct pl011_uart_csreg *)serial_port.csr)->dr = c;   		
    }
    else {
    	wait(serial_port.osema); 
    	/* Finding the tail */
    	ushort tail = serial_port.ostart + serial_port.ocount;
    	/* Make sure that it does not exceed buffer limit and "wraps" around to the start*/
    	ushort index = tail % UART_OBLEN;

    	serial_port.out[index] = c;
    	serial_port.ocount++;	
    }

    restore(im);
    return OK;
}
