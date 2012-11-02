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
// $Id: elf.c,v 1.1.4.1 2007/08/20 06:08:22 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------------------------------//
//! @file elf.c
//! ELF file related routines (Creating processes out of ELF files)
//----------------------------------------------------------------------------------------------------//

#include <stdio.h>
#include <os_config.h>
#include <sys/init.h>
#include <config/config_param.h>
#include <config/config_cparam.h>
#include <sys/arch.h>
#include <sys/ktypes.h>
#include <sys/ksched.h>
#include <sys/process.h>
#include <sys/mem.h>
#include <sys/queue.h>
#include <sys/ksemaphore.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/decls.h>
#include <sys/stats.h>

#ifdef CONFIG_ELF_PROCESS

#ifdef CONFIG_PTHREAD_SUPPORT 
#include <sys/kpthread.h>
#endif 

//----------------------------------------------------------------------------------------------------//
// Data
//----------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------//
// Declarations
//----------------------------------------------------------------------------------------------------//
extern process_struct ptable[] ;    
extern signed char current_pid;
extern char process_startup_stack[];

//----------------------------------------------------------------------------------------------------//
// Definitions
//----------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------//
//  @func - sys_elf_process_create
//! @desc 
//!   Process creation.
//!   - Reserves a pid for the process.
//!   - Initilaizes the process structure. Also loads the r15 with the start
//!     address of the process.
//!   - Places the process in the READY_Q.
//!   - If SCHED_PRIO, calls the process_scheduler for scheduling.
//!   
//! @param
//!   - pstart_addr is the start address of the process
//!   - priority is the priority of the process
//! @return
//!   - PID of the new process.
//!   - -1 on Error. Max. process exceeded.
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
pid_t sys_elf_process_create (void* pstart_addr, unsigned int priority)
{
    pid_t pid;
    process_struct *pcb;
    unsigned int stackaddr;

    pid = proc_create (priority);

    if (pid == -1)
        return -1;

    pcb = &ptable[pid];

    stackaddr = ((unsigned int)process_startup_stack                    // Temporary stack for new launched process in case it is interrupted 
                 + PROCESS_STARTUP_STACKSZ + SSTACK_PTR_ADJUST);        // before its own stack is setup

    setup_initial_context (&ptable[pid], current_pid, pstart_addr, stackaddr);
    
    return pid;
}


//----------------------------------------------------------------------------------------------------//
//  @func - sys_elf_exit
//! @desc
//!   Remove the process entry from the process table. 
//!   - Set the is_allocated flag to '0'. Set the current_pid to -1. so that the
//!     next process gets scheduled.
//! @return
//!   - 0 on Success
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
int sys_elf_exit()
{
    process_invalidate (&ptable[current_pid]);
    suspend ();
    return 0 ;
}

#endif /* CONFIG_ELF_PROCESS */
