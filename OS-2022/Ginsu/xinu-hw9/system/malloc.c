/**
 * @file malloc.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Request heap storage, record accounting information, returning pointer
 * to assigned memory region.
 *
 * @param size
 *      number of bytes requested
 *
 * @return
 *      pointer to region on success, or @c NULL on failure
 */
void *malloc(ulong size)
{
    irqmask im;
    im = disable();
   
    struct memblock *pmem;

    /* we don't allocate 0 bytes. */
    if (0 == size)
    {
        return NULL;
    }

     /** TODO:
      *      1) Make room for accounting info
      *      2) Acquire memory with getmem syscall
      *         - handle possible error (SYSERR) from getmem...
      *      3) Set accounting info in pmem
      *      4) Return proper pointer to base of free memory region
      */
	ulong *node = getmem(size + 8); //Make room for accounting info by doing +8 and getting the memory requested (size)
	// Handle porrrible error by checking for (void *)SYSERR and if it is returned from getmem then malloc()
	// returns (void *)SYSERR
	if(node == (void *)SYSERR){
		return (void *)SYSERR;
	}

	pmem->next = &node;
	pmem->length = size + 8;
	
	restore(im);
	return (void *)(node);
}
