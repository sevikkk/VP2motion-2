////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004 Xilinx, Inc.  All rights reserved.
//
// Xilinx, Inc.
// XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
// COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
// ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
// STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
// IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
// FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
// XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
// THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
// ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
// FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Id: bufmalloc.c,v 1.1.4.1 2007/08/20 06:08:22 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------------------------------//
//! @file bufmalloc.c  
//! Block memory allocation support
//----------------------------------------------------------------------------------------------------//
#include <os_config.h>
#include <stdio.h>
#include <config/config_cparam.h>
#include <config/config_param.h>
#include <sys/arch.h>
#include <sys/types.h>
#include <sys/decls.h>
#include <sys/init.h>
#include <errno.h>
#include <sys/bufmalloc.h>

#ifdef CONFIG_BUFMALLOC
//----------------------------------------------------------------------------------------------------//
// Declarations  
//----------------------------------------------------------------------------------------------------//

#define    MEM_TO_BLK(membufp, mem)             (((unsigned int)mem - (unsigned int)membufp->memptr)/(membufp->blksiz))
#define    MEM_WITHIN_BUF(membufp, mem)         (((mem >= membufp->memptr) && (mem < membufp->limit))?1:0)     

typedef struct membuf_info_s {
    char        active;
    void        *memptr;
    void        *limit;
    void        *freep;
    int         nblks;
    int         nfree;
    size_t      blksiz;
} membuf_info_t;

void    bufmalloc_init ();
void*   get_mbufblk    (membuf_info_t *mbufptr);
int     sys_bufcreate  (membuf_t *mbuf, void *memptr, int nblks, size_t blksiz);
int     sys_bufdestroy (membuf_t mbuf);
void*   sys_bufmalloc  (membuf_t mbuf, size_t siz);
void    sys_buffree    (membuf_t mbuf, void *mem);
extern  void bufmalloc_mem_init ();
//----------------------------------------------------------------------------------------------------//
// Data  
//----------------------------------------------------------------------------------------------------//
membuf_info_t   mbufheap[N_MBUFS];

//----------------------------------------------------------------------------------------------------//
// Definitions  
//----------------------------------------------------------------------------------------------------//
void bufmalloc_init ()
{
    int i;
    for (i = 0; i < N_MBUFS; i++)
        mbufheap[i].active = 0;

    bufmalloc_mem_init ();
}

void* get_mbufblk (membuf_info_t *mbufptr)
{
    void *ret;

    if (!mbufptr->nfree) 
        return NULL;
    
    ret = mbufptr->freep;
    mbufptr->freep = (*(void**)ret);
    mbufptr->nfree--;
    
    return ret;
}

//----------------------------------------------------------------------------------------------------//
//  @func - sys_bufcreate
//! @desc
//!   Free the memory allocated by bufmalloc
//! @param
//!   - mem is the address of the memory block, that was got from bufmalloc
//! @return
//!   - Nothing
//! @note
//!   - WARNING. ALIGNMENT requirements have to be met or exception handlers required.
//----------------------------------------------------------------------------------------------------//
int sys_bufcreate (membuf_t *mbuf, void *memptr, int nblks, size_t blksiz)
{
    int i;
    membuf_info_t *mbufptr;
    void **cur, **next;

    if ((mbuf == NULL) || (memptr == NULL) || (nblks <= 0) || (blksiz < sizeof (void*))) {
        kerrno = EINVAL;
        return -1;
    }

    mbufptr = &mbufheap[0];
    for (i = 0; i < N_MBUFS; i++) {                                                     // Find first free membuf descriptor
        if (!mbufptr->active)
            break;
        mbufptr++;
    }

    if (i == N_MBUFS) {
        kerrno = EAGAIN;
        return -1;
    }

    mbufptr->active  = 1;
    mbufptr->memptr  = memptr;
    mbufptr->nblks   = nblks;
    mbufptr->blksiz  = blksiz;
    mbufptr->nfree   = nblks;
    mbufptr->freep   = memptr;
    mbufptr->limit   = (void*)((unsigned int)memptr + (nblks * blksiz));
    *mbuf = (membuf_t)i;                                                                // Return membuf identifier

    cur  = (void**)memptr;
    next = (void**)((unsigned int)memptr + blksiz);
    for (i = 0; i < (nblks - 1); i++) {                                                 // Initialize free list in the the memory block
        *cur = (void*)next;                                                             // Alignment constraints should be met here
        cur  = next;                                                                    // Otherwise, unaligned exceptions will occur
        next = (void**)((unsigned int)next + blksiz);
    }
    *cur = (void*) NULL;
    return 0;
}

int sys_bufdestroy (membuf_t mbuf)
{
    if (mbuf >= 0 && mbuf < N_MBUFS)
        mbufheap[mbuf].active = 0;
    else {
        kerrno = EINVAL;
        return -1;
    }
        
    return 0;
}

void* sys_bufmalloc (membuf_t mbuf, size_t siz)
{
    membuf_info_t   *mbufptr;
    void* ret = NULL;
    int i;

    if ((mbuf != MEMBUF_ANY) && ((mbuf < 0 || mbuf > N_MBUFS)))
        return NULL;
    
    if (mbuf == MEMBUF_ANY) {
        mbufptr = &mbufheap[0];
        for (i = 0; i < N_MBUFS; i++) {
            if (mbufptr->active && mbufptr->blksiz >= siz) {                            // Get first pool that can fit the request
                ret = get_mbufblk (mbufptr);
                if (ret)
                    break;
            }
            mbufptr++;
        }
    } else {
        mbufptr = &mbufheap[mbuf];
        
        if (!mbufptr->active) {
            kerrno = EINVAL;
            return NULL;
        }
        ret = get_mbufblk (mbufptr);
    }
    
    if (ret == NULL)
        kerrno = EAGAIN;
    return ret;
}

void sys_buffree (membuf_t mbuf, void *mem)
{
    membuf_info_t   *mbufptr;
    void** newblk;
    int blk, i;

    if ((mbuf != MEMBUF_ANY) && ((mbuf < 0 || mbuf > N_MBUFS)))
        return;
    
    if (mbuf == MEMBUF_ANY) {
        mbufptr = &mbufheap[0];
        for (i = 0; i < N_MBUFS; i++) {
            if (mbufptr->active && MEM_WITHIN_BUF (mbufptr, mem)) 
                break;
            mbufptr++;
        }
        if (i == N_MBUFS)
            return;
    } else
        mbufptr = &mbufheap[mbuf];

    blk = MEM_TO_BLK (mbufptr, mem);
    if (blk < 0 || blk > mbufptr->nblks)
        return;
    
    newblk = (void**)((unsigned int)mbufptr->memptr + 
                      (mbufptr->blksiz * blk));                                         // Forcing it to be a valid chain offset within the block

    *newblk = mbufptr->freep;
    mbufptr->freep = (void*)newblk;
    mbufptr->nfree++;
}
#endif /* CONFIG_BUFMALLOC */
