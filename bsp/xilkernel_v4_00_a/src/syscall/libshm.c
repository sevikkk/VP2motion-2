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
// $Id: libshm.c,v 1.1.4.1 2007/08/20 06:08:23 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------------------------------//
//! @file libshm.c
//! This contains system call wrapper functions for Shared Memory.
//----------------------------------------------------------------------------------------------------//

#include <os_config.h>
#include <sys/syscall.h>
#include <sys/shm.h>
#include <sys/kshm.h>

extern void* make_syscall (void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, int syscall_num);

#ifdef CONFIG_SHM
int shmget (key_t key, size_t size, int shmflg ) 
{
    return (int) make_syscall ((void*)key, (void*)size, (void*)shmflg, NULL, NULL, SC_SHMGET);
}

int shmctl (int shmid, int cmd, struct shmid_ds *buf) 
{
    return (int) make_syscall ((void*)shmid, (void*)cmd, (void*)buf, NULL, NULL, SC_SHMCTL);
}

void* shmat (int shmid, const void *shmaddr, int shmflg ) 
{
    return (void*) make_syscall ((void*)shmid, (void*)shmaddr, (void*)shmflg, NULL, NULL, SC_SHMAT);
}

int shmdt( const void *shmaddr ) 
{
    return (int) make_syscall ((void*)shmaddr, NULL, NULL, NULL, NULL, SC_SHMDT);
}
#endif /* CONFIG_SHM */
