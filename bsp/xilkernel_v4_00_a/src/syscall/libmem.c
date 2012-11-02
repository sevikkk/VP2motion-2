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
// $Id: libmem.c,v 1.1.4.1 2007/08/20 06:08:23 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file libmem.c
//! This contains system call wrapper functions for Dynamic buffer management.
//----------------------------------------------------------------------------------------------------//
#include <os_config.h>
#include <sys/syscall.h>
#include <sys/bufmalloc.h>

extern void* make_syscall (void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, int syscall_num);

#ifdef CONFIG_BUFMALLOC
int bufcreate (membuf_t *mbuf, void *memptr, int nblks, size_t blksiz)
{
    return (int)make_syscall ((void*)mbuf, memptr, (void*)nblks, (void*)blksiz, NULL, SC_BUFCREATE);
}

int bufdestroy (membuf_t mbuf)
{
    return (int)make_syscall ((void*)mbuf, NULL, NULL, NULL, NULL, SC_BUFDESTROY);
}

void* bufmalloc (membuf_t mbuf, size_t siz)
{
    return (void*)make_syscall ((void*)mbuf, (void*)siz, NULL, NULL, NULL, SC_BUFMALLOC);
}

void buffree (membuf_t mbuf, void *mem)
{
    make_syscall ((void*)mbuf, (void*)mem, NULL, NULL, NULL, SC_BUFFREE);
}
#endif /* CONFIG_MALLOC */
