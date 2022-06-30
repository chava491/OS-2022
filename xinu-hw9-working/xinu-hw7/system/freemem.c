/**
 * @file freemem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Frees a block of heap-allocated memory.
 *
 * @param memptr
 *      Pointer to memory block allocated with memget().
 *
 * @param nbytes
 *      Length of memory block, in bytes.  (Same value passed to memget().)
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail
 *      because of memory corruption or specifying an invalid memory block.
 */
syscall freemem(void *memptr, ulong nbytes)
{
    irqmask im;
    im = disable(); //Disable the interrupts
    register struct memblock *block, *next, *prev;
    ulong top;

    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < freelist.base)
        || ((ulong)memptr > freelist.base + freelist.bound))
    {
	restore(im); //Restore the interupts before we return from the function
        return SYSERR;
    }

    block = (struct memblock *)memptr;
    nbytes = (ulong)roundmb(nbytes);

    /* TODO:
     *      - Disable interrupts
     *      - Find where the memory block should
     *        go back onto the freelist (based on address)
     *      - Find top of previous memblock
     *      - Make sure block is not overlapping on prev or next blocks
     *      - Coalesce with previous block if adjacent
     *      - Coalesce with next block if adjacent
     *      - Restore interrupts
     */
memblk *node = freelist.head;
if(block < node) { //This is when the block is before the freelist.head adress (aka to the "left" if we are looking at adresses from left to right
        if(block + nbytes == node){ //block and head can be combined
                next = node->next; //Next node is set as the freelist.head next node.
                block->next = next; //We combine the block and head by pointing the block->next to the node->next
                block->length = nbytes + node->length; //update block length by adding the blocks original length and the length of the freelist.head node we just combined with the block
                freelist.head = block; //We point the freelist.head to the new node which is the start of the block since we just free it and it is to the "left" of the head.

	}
        else { //This is when the block and head cannot be combined
                block->length = nbytes; //We update the block length to the passed in nbytes we want to free.
                next = node; //The next node is defined as freelist.head
                block->next = next; //We want the block to link to the freelist.head
		freelist.head = block;// We then update the freelist.head to point to the start of the block we just freed.
//Note the memory between block and block->next which is the original freelist.head adress before we changes it is not free. This is why we link from the block we just freed to the node originally defined as freelist.head so as to not count the memory in between the two as free, since it is not free because we did not combine the block and the node original defined as freelist.head
        }
}

else { //This is when the block is after the freelist.head adress (aka to the "right" if we are looking at adresses from left to right
        top = (ulong)freelist.head + freelist.head->length; //We find the top (aka the bound of the node at freelist.head
        next = freelist.head; //We set next equal to the freelist.head
        do{
		prev = next; //We update our prev value to freelist.head
                next = next->next;//We set the next->next to next which is freelist.head
                if(next!=NULL && next!=0){ //Make sure we have a next node available
                        top = (ulong)next + next->length;
                }
        }
	while(top < (ulong)block);
      //------------Checking Meory Overlap------------------
        if((freelist.size != 0) && ((prev + prev->length) > block)){ //overlap occurs on the "left" side of the block
		restore(im); //restore interupts
                return SYSERR; //return error if memory overlap happens on the "left" side of the block
        }
        else if((next!=NULL || next!=0) && (freelist.size != 0) && (block + nbytes > next)){ //Overlap occurs on the "right" side of the block
	       	restore(im); //restore interrupts
                return SYSERR; //return error
      	}
 
     //------------Checking if block is adjacent with prev or next--------------	
	if((block + nbytes == next) && (prev + prev->length == block)){ //Everything is combined: prev, block, and next because they are next to each other
                prev->length = prev->length + nbytes + next->length;
                prev->next = next->next;
        }
        else if((prev + prev->length) == block) { //block and previous can be combined because block is right next to it on the "left" side.
                prev->length = prev->length + nbytes;
		prev->next = next;                
        }
        else if((block + nbytes) == next) { //block and next are combined because they are next to each other by block is on the "right" side
                block->length = nbytes + next->length;
		block->next = next->next;
                prev->next = block;
        }
        else { // Nothing is combined because nothing is next to each other
                prev->next = block;
                block->next = next;
                block->length = nbytes;
        }
		} //END OF ELSE (AKA THE ENTIRE SECTION HERE)

        if((freelist.head == NULL) || (freelist.head == 0)){
                    block->next = NULL;
                    freelist.head = block;
    
            }	
	freelist.size += nbytes;
    	restore(im); //Restore the interupts before we return from the function
  	return OK;
}
