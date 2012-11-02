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
// $Id: sched.c,v 1.1.4.1 2007/08/20 06:08:22 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file sched.c
//! This file contains routines for process scheduling
//----------------------------------------------------------------------------------------------------//

#include <stdio.h>
#include <os_config.h>
#include <sys/ksched.h>
#include <sys/entry.h>
#include <config/config_param.h>
#include <config/config_cparam.h>
#include <sys/process.h>
#include <sys/decls.h>

//----------------------------------------------------------------------------------------------------//
// Data
//----------------------------------------------------------------------------------------------------//

   
// Ready Queue - Array of N_PRIO process queues
struct _queue ready_q[N_PRIO] ;		
signed char entry_mode = ENTRY_MODE_USER;       // Current entry mode into kernel
signed char resched = 0;                        // Indicates if rescheduling occurred elsewhere
char did_resched = 0;                           // Indicates if the kernel completed the rescheduling

#ifdef CONFIG_STATS
signed char sched_history[SCHED_HISTORY_SIZ];
int shp = 0;
#endif
//----------------------------------------------------------------------------------------------------//
// Declarations
//----------------------------------------------------------------------------------------------------//
extern unsigned int budget_ticks;
extern process_struct *ctx_save_process;
extern int save_context ();
extern void restore_context ();

void suspend ();
int scheduler ();

//----------------------------------------------------------------------------------------------------//
// Definitions
//----------------------------------------------------------------------------------------------------//

void readyq_init()
{
    unsigned int i = 0 ;

    for (;i < N_PRIO; i++ ) {
	alloc_q (&ready_q[i], MAX_READYQ, READY_Q, sizeof(char), i);
    }
}

int scheduler () 
{

#ifdef CONFIG_DEBUGMON
    debugmon_dump_sched_info ();
#endif

    if (current_process->state == PROC_DEAD) {
	ctx_save_process = NULL;
	prev_pid = -1;
    }
    else if (current_process->pcontext.isrflag == 1) {    // If entered the kernel through an ISR, context saved in ISR itself 
	ctx_save_process = NULL;
	prev_pid = current_pid;
    }
    else {
	ctx_save_process = current_process;
	prev_pid = current_pid;
    }

#if SCHED_TYPE == SCHED_RR
    sched_rr ();
#elif SCHED_TYPE == SCHED_PRIO
    sched_prio ();
#endif

    if( current_pid == -1 ) {	
	DBG_PRINT ("XMK: Unable to find schedulable process. Kernel Halt.\r\n") ;
	while(1);
    } 

    did_resched = 1;    // scheduler indicates that it completed the rescheduling
    resched = 0;        // scheduler always resets resched flag

#ifdef CONFIG_DEBUGMON 
    debugmon_stack_check ();
#endif

#ifdef CONFIG_STATS
    if (shp != SCHED_HISTORY_SIZ)
	sched_history[shp++] = current_pid;
#endif

    if (prev_pid == current_pid)
	return 1;

    return 0;
}

#if SCHED_TYPE == SCHED_RR
//----------------------------------------------------------------------------------------------------//
//  @func - sched_rr
//! @desc 
//!   Round Robin Scheduler.
//! @return
//!   - Nothing
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
void sched_rr ()
{
    signed char ready = -1;

    // Enqueue only currently running processes. Else,
    // If PROC_DEAD, no need to enqueue
    // If PROC_WAIT or PROC_TIMED_WAIT, already enqueued in appropriate wait queue
    // If PROC_DELAY, is in one of the timer queues
    // If idle_task, then does not need to enter the queue
    if (current_process->state == PROC_RUN) {
        ptable[current_pid].state = PROC_READY;
	if(current_pid != idle_task_pid)
	    penq (&ready_q[0], current_pid, 0);
    }
    
    SET_CURRENT_PROCESS (-1);

    while (ready_q[0].item_count != 0) {
	pdeq (&ready_q[0], &ready, 0);
	if (ptable[ready].state == PROC_DEAD) {   // Flush out dead processes
	    ready = -1;
	    continue;
	}
	else break;
    }

    if (ready == -1) 
	ready = idle_task_pid;  

#if 0 
    DBG_PRINT ("XMK: Scheduler: scheduled pid: ");
    putnum (ready);
    DBG_PRINT ("\r\n");
#endif

    ptable[ready].state = PROC_RUN;
    SET_CURRENT_PROCESS (ready);
}
#endif /* SCHED_TYPE == SCHED_RR */

#if SCHED_TYPE == SCHED_PRIO
//----------------------------------------------------------------------------------------------------//
//  @func - sched_prio
//! @desc 
//!   Pre-emptive strict priority scheduler
//! @return
//!   - Nothing
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
void sched_prio ()
{
    int i;
    signed char ready = -1;

    // Enqueue only currently running processes. Else,
    // If PROC_DEAD, no need to enqueue
    // If PROC_WAIT or PROC_TIMED_WAIT, already enqueued in appropriate wait queue
    // If PROC_DELAY, is in one of the timer queues
    // If idle_task, then does not need to enter the queue
    if (current_process->state == PROC_RUN) {
	ptable[current_pid].state = PROC_READY;
	if (current_pid != idle_task_pid)
	    penq (&ready_q[ptable[current_pid].priority], current_pid, 0);
    }
    
    SET_CURRENT_PROCESS (-1);

    for (i=0; i <= PRIO_LOWEST; i++) {
	while (ready_q[i].item_count != 0) {
	    pdeq (&ready_q[i], &ready, 0);
	    if (ptable[ready].state == PROC_DEAD) {   // Flush out dead processes
		ready = -1;
		continue;
	    }
	    else break;
	}
	
	if (ready != -1)
	    break;
    }

    if (ready == -1) 
	ready = idle_task_pid;  
  
#if 0 
    DBG_PRINT ("XMK: Scheduler: scheduled pid: ");
    putnum (ready);
    DBG_PRINT ("\r\n");
#endif

    ptable[ready].state = PROC_RUN;
    SET_CURRENT_PROCESS (ready);
}
#endif /* SCHED_TYPE == SCHED_PRIO */

//----------------------------------------------------------------------------------------------------//
//  @func - suspend
//! @desc 
//!   Suspend a process inside the kernel. A rescheduling followed by the corresponding context
//!   switch occurs within this routine.
//! @return
//!   - Nothing
//! @note
//!   - This routine is not expected to be invoked from within an ISR. i.e no suspension allowed in
//!     an ISR.
//----------------------------------------------------------------------------------------------------//
void suspend () 
{
#ifdef CONFIG_STATS
    current_process->active_ticks++;
    budget_ticks++;
#endif
    if (scheduler ()) {
	DBG_PRINT ("XMK: Scheduling error. Cannot suspend current process.\r\n");
	while (1)
	    ;
    }

    if (ctx_save_process != NULL) 
	if (save_context (ctx_save_process))    // Save context returns 0 during save
	    return;                             // When saved context is restored, returns a 1
    
    restore_context ();
}



