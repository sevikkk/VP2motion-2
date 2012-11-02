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
// $Id: debugmon.c,v 1.1.4.1 2007/08/20 06:08:21 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file debugmon.c
//! Kernel inbuilt debug monitor routines for Microblaze
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

#ifdef CONFIG_DEBUGMON

extern signed char idle_task_pid;
extern unsigned int budget_ticks;
extern process_struct ptable[] ;	 
extern signed char current_pid ;	 
extern process_struct *current_process;
extern process_struct *ctx_save_process;
extern signed char prev_pid; 
extern struct _queue ready_q[] ;		
extern signed char entry_mode;  
extern signed char resched;     

void debugmon_dump_proc_info ()
{
    int i, j;

    xmk_enter_kernel ();
    DBG_PRINT ("DEBUGMON: kernel_dump_proc_info ----> \r\n");
    for (i = 0; i<MAX_PROCESS_CONTEXTS; i++) {
        if (!ptable[i].is_allocated)
            continue;

        DBG_PRINT ("=============================>\r\n");
        DBG_PRINT ("pid: ");
        putnum (ptable[i].pid);
        DBG_PRINT ("\r\nstate: ");
        putnum (ptable[i].state);
        DBG_PRINT ("\r\nisrflag: ");
        putnum (ptable[i].pcontext.isrflag);

        for (j=0; j<33; j++) {
            DBG_PRINT ("\r\nregs[");
            putnum (j);
            DBG_PRINT ("]: ");
            putnum (ptable[i].pcontext.regs[j]);
        }
        DBG_PRINT ("\r\n=============================>\r\n\r\n");
    }
    DBG_PRINT ("DEBUGMON: kernel_dump_proc_info ENDS\r\n");    
    while (1);
}


void debugmon_stack_check ()
{
    unsigned int cur_sp;
    unsigned int saddr, ssize;

    if (current_process->thread != NULL && current_process->thread->thread_attr.stackaddr != NULL) {
        cur_sp = current_process->pcontext.regs[1];
        saddr  = (unsigned int) current_process->thread->thread_attr.stackaddr;
        ssize  = current_process->thread->thread_attr.stacksize;

        if ((cur_sp > saddr) || (cur_sp <= (saddr - ssize))) {
            DBG_PRINT ("DEBUGMON: Stack check failed for PID: ");
            putnum (current_process->pid);
            DBG_PRINT (", SP: ");
            putnum (current_process->pcontext.regs[1]);
            DBG_PRINT (", Base: ");
            putnum (saddr);
            DBG_PRINT (", Limit: ");
            putnum ((saddr - ssize));
            DBG_PRINT ("\r\n");
            debugmon_dump_proc_info ();
        }
    }
}

void debugmon_dump_sched_info ()
{
    int i;
    char *qp;

    print ("Scheduler: current_pid: ");
    putnum (current_pid);
    print (", prev_pid: ");
    putnum (prev_pid);
    print ("\r\n");

    print ("ready_q[0].item_count: ");
    putnum(ready_q[0].item_count);
    print ("\r\n");
    qp = (char*)ready_q[0].items;
    qp += ready_q[0].qfront;

    print ("Items: ( ");
    for (i=0; i<ready_q[0].item_count; i++) {
	putnum(*qp++);
	print (" ");
    }
    print (")\r\n");
}

#endif /* CONFIG_DEBUGMON */
