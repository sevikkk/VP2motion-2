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
// $Id: libprocess.c,v 1.1.4.1 2007/08/20 06:08:23 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file libprocess.c
//! This contains system call wrapper functions for Process Management.
//----------------------------------------------------------------------------------------------------//
#include <os_config.h>
#include <sys/process.h>
#include <sys/syscall.h>
#include <sys/stats.h>
#include <sys/ktypes.h>

reent_t  *lreent = NULL;

extern void* make_syscall (void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, int syscall_num);

reent_t* get_reentrancy ()
{
    return (reent_t*) make_syscall (NULL, NULL, NULL, NULL, NULL, SC_GET_REENTRANCY);
}

#ifdef CONFIG_ELF_PROCESS
pid_t elf_process_create (void* start, int prio) 
{
    return (int) make_syscall (start, (void*)prio, NULL, NULL, NULL, SC_PROCESS_CREATE);
}

int elf_process_exit ()
{
    return (int) make_syscall ( NULL, NULL, NULL, NULL, NULL, SC_PROCESS_EXIT);
}
#endif

#ifdef CONFIG_KILL
int kill (pid_t pid)
{
    return (int) make_syscall ((void*)(int)pid, NULL, NULL, NULL, NULL, SC_PROCESS_KILL);
}
#endif

int process_status (pid_t pid, p_stat *ps)
{
    return (int) make_syscall ((void*)(int)pid, (void*)ps, NULL, NULL, NULL, SC_PROCESS_STATUS);
}

pid_t get_currentPID () 
{
    return (int) make_syscall (NULL, NULL, NULL, NULL, NULL, SC_PROCESS_GETPID);    
}

#ifdef CONFIG_YIELD
int yield () 
{
    return (int) make_syscall (NULL, NULL, NULL, NULL, NULL, SC_PROCESS_YIELD);    
}
#endif /* CONFIG_YIELD */

#ifdef CONFIG_STATS
int get_kernel_stats (kstats_t *stats)
{
    return (int) make_syscall ((void*)stats, NULL, NULL, NULL, NULL, SC_GET_KERNEL_STATS);
}
#endif


int*   __errno ()
{
    if (lreent == NULL)
        lreent = get_reentrancy ();

    return &(lreent->_errno);
}

