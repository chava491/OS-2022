/**
 * @file getmem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Allocate heap memory.
 *
 * @param nbytes
 *      Number of bytes requested.
 *
 * @return
 *      ::SYSERR if @p nbytes was 0 or there is no memory to satisfy the
 *      request; otherwise returns a pointer to the allocated memory region.
 *      The returned pointer is guaranteed to be 8-byte aligned.  Free the block
 *      with memfree() when done with it.
 */
/* for syscall argument parsing */
#define SCARG(type, args)  (type)(*args++)

void *sc_getmem(int *args)
{
   ulong nbytes = SCARG(ulong, args);
   irqmask im;
   im = disable(); // Disable the interrupts
     
   register memblk *prev, *leftover;

    if (0 == nbytes)
    {
	restore(im); //We reenable the interrupts
	return (void *)SYSERR;
    }

    /* round to multiple of memblock size   */
    nbytes = (ulong)roundmb(nbytes);

    /* TODO:
     *      - Disable interrupts
     *      - Traverse through the freelist
     *        to find a block that's suitable 
     *        (Use First Fit with remainder splitting)
     *      - Restore interrupts
     *      - return memory address if successful
     */
	
	memblk *node = freelist.head; // Initialize node to point to the head of the freelist
	prev = NULL;		      // Initialize previous to NULL
	while(node != NULL) {	      // Here we are making sure we do not hit the end of the freelist
		if(node->length == nbytes) {	// The case that the node we are at has the same length as what we want then we just allocate that node
			if (prev != NULL){	// make sure that there is a previous node to link next node to for them to be linked
			prev->next = node->next; // To link the list together (prev->next is like kid a on the left linking to the node->next, person to the right)
						 // We are connecting the previous node to the next node (thus linking the list to one another)
			}
			else {
			freelist.head = node->next; // We are saying her "Hey from the freelist.head to the node->next we can not use that because we just seperate
						    // it for the call getmem we did.
			}
			freelist.size -= nbytes; // Decrement the freelist by nbytes.
			restore(im); //renable the interupts
			return node; //return the address of the memory address
		}
		else if(node->length > nbytes) {
			/*
 			* leftover = &FreeBlock to put back into freelist
 			* update the length of leftover
 			* 
 			*/
			leftover = (memblk *)(((ulong )node) + nbytes); //We are determing the adress of where the remainder block starts (What is kept in the free list) 
			leftover->length = node->length - nbytes; //we decrease the node length of the newblk by subtracting the size of the node we are at 
			leftover->next = node->next;	//Link the newblk to the next node avaialable
			if (prev != NULL){	       // Here we make sure that there is a previous node to be linked to our new node
				prev->next = leftover;  //We set the next blk of the previous node to the newblk we created to link them (hence link list)
			}
			else {
				freelist.head = leftover; //We set the adress of the freelist.head back to point to the adress of newblk since that memory from 
							//freelist.head to newblk is no longer usable 
			}
			node->length = nbytes;	//Set the length variable of the current node we are on to the nbytes we want it to reflect.
			freelist.size -= nbytes; //Decrement the freelist size to account for the nbytes we just "removed" when getmem() gets calles
			restore(im);  // Turn interrupts back on
			return node; //We return the adress of the current node
		}
		else //This is the case in which node->length < nbytes. Thus, we can not use this block do to it being to small so we move on to the next block in
		     // freelist to restart and check that size
		{
			prev = node; //Here we update the prev variable to the current node we are at because we are about to go to the next node in the following line
			node = node->next; //Here we now move to the next node by updating the node variable to the next node in the linked list
		}
	}
	
	restore(im); // We turn interrupts back on
	return (void *)SYSERR; //If all else fails their is system error thrown
}
