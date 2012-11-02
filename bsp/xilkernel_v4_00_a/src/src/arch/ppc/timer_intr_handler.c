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
// $Id: timer_intr_handler.c,v 1.1.2.1 2007/11/06 20:02:47 vasanth Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file timer_intr_handler.c
//! Contains PPC system timer interrupt handler routine
//----------------------------------------------------------------------------------------------------//

#include <os_config.h>
#include <sys/ksched.h>
#include <xpseudo_asm.h>
#include <sys/decls.h>
#include <xtime_l.h>

extern void soft_tmr_handler ();
extern signed char resched;
extern process_struct *current_process;
unsigned int kernel_ticks;      //! Ticks since kernel startup
char timer_need_refresh  = 0;   //! Do we need a reset?
#ifdef CONFIG_STATS
unsigned int budget_ticks;
#endif
unsigned char sched_partial_tick;

//----------------------------------------------------------------------------------------------------//
//  @func - timer_int_handler
//! @desc
//!   System timer interrupt handler
//!   - Do a rescheduling operation
//!   - If a context switch occurs within this routine,
//!     - When returning out of process_scheduler, process executing will return in the appropriate
//!       context when it was switched out on a previous flow through this execution path.
//!   - Reset PIT interval to start a full time slice (if PIT in the system) (if INTC not present)
//! @param
//!   - none
//! @return
//!   - nothing
//! @note
//!   - May NOT return from this routine if a NEW process context is scheduled in the scheduler
//!   - A context switch does not occur within this routine if an INTC is present. The switch 
//!     occurs at the end of the INTC ISR.
//----------------------------------------------------------------------------------------------------//
void timer_int_handler (int irq_num)
{
    // Update global kernel ticks so far
    kernel_ticks++;
#ifdef CONFIG_STATS
    current_process->active_ticks++;
    budget_ticks++;
#endif
#ifdef CONFIG_TIME 
    soft_tmr_handler ();
#endif

    if (sched_partial_tick) {
#if SCHED_TYPE == SCHED_RR
        // Avoid one more starvation case. On a partial schedule tick,
        // you want to give the next full time quantum to the thread
        // which was scheduled partially and not any thread that 
        // timed out in soft_tmr_handler. 
        // Note: We do this only for RR scheduling where we don't 
        //       want starvation.
        resched = 0;
#endif
        sched_partial_tick = 0;
    } else {
        resched = 1;
    }

    timer_need_refresh = 1;
}
