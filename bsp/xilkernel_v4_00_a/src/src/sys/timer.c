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
// $Id: timer.c,v 1.1.4.3 2007/09/27 02:34:40 vasanth Exp $
//
//////////////////////////////////////////////////////////////////////////////

/*****************************************************************************
 * File: timer.c
 * Description:
 * Timer functionality
 *****************************************************************************/
#include <os_config.h>
#include <sys/timer.h>
#include <sys/process.h>
#include <sys/decls.h>
#include <sys/xtrace.h>

#ifdef CONFIG_TIME
#define get_tmr_list_shadow(tmrlist) ((tmrlist == active_tmrs_a)?(active_tmrs_b):(active_tmrs_a))

static soft_tmr_t soft_tmrs[MAX_TMRS] ;
static int* active_tmrs;                   // Pointer to the list of timer IDs that are currently active
static int  active_tmrs_a[MAX_TMRS];       // Actual timer ID lists A & B. (One is the shadow of the other)
static int  active_tmrs_b[MAX_TMRS];       
static int nactive;
extern unsigned int kernel_ticks;
extern struct _queue ready_q[];		
extern process_struct ptable[];
extern process_struct *current_process;
extern signed char resched;

static void            handle_timeout (pid_t pid);
static int             get_free_tmr ();
static unsigned int
                ms_to_ticks (unsigned int ms);
static unsigned int    
                ticks_to_ms (unsigned int ticks);
static void     update_active_tmrs_list (pid_t pid);

void soft_tmr_init ()
{
    int i;

    for (i=0; i < MAX_TMRS; i++) {
	soft_tmrs[i].timeout = 0;
	soft_tmrs[i].pid = -1;
	active_tmrs_a[i] = -1;
	active_tmrs_b[i] = -1;
    }
    
    active_tmrs = active_tmrs_a;                                        // Set the current active timer list to "list A"
    nactive = 0;
    kernel_ticks = 0;
}

static unsigned int ms_to_ticks (unsigned int ms)
{
    return  ((ms * SYSTMR_CLK_FREQ_KHZ) / SYSTMR_INTERVAL);
}

static unsigned int ticks_to_ms (unsigned int ticks)
{
    return  (ticks * (SYSTMR_INTERVAL / SYSTMR_CLK_FREQ_KHZ));
}

static void handle_timeout (signed char pid)
{

    if (ptable[pid].blockq) {
#if SCHED_TYPE == SCHED_RR
        pdelq (ptable[pid].blockq, pid);
#elif SCHED_TYPE == SCHED_PRIO
        prio_pdelq (ptable[pid].blockq, pid);
#endif
        ptable[pid].timeout = 1;
        ptable[pid].blockq = NULL;
    } 
    
    ptable[pid].state = PROC_READY;
#if SCHED_TYPE == SCHED_RR
    penq (&ready_q[0], pid, 0);
#else /* SCHED_TYPE == SCHED_PRIO */
    penq (&ready_q[ptable[pid].priority], pid, 0);
#endif
    
    resched = 1;
}


static void update_active_tmrs_list (pid_t pid)
{
    int i;
    int* tmrshadow;
    int curactive = 0;
    
    tmrshadow = get_tmr_list_shadow(active_tmrs);                       // Get the list that can act as a shadow to the current list
    for (i=0; i < nactive; i++) {
        if (soft_tmrs[active_tmrs[i]].pid == pid) {                      // This timer has been removed and is no longer active. Drop it
            soft_tmrs[active_tmrs[i]].pid = -1;                         // and also make it available for future allocations
            continue;
        }
        
        tmrshadow[curactive] = active_tmrs[i];                      // Append timer id to newly formed list
        curactive++;
    }
    
    active_tmrs = tmrshadow;
    nactive = curactive;
}

static int get_free_tmr ()
{
    int i;
    
    if (nactive == MAX_TMRS)                                            // Break early if no timers left
	return -1;
    
    for (i = 0; i < MAX_TMRS; i++) {
	if (soft_tmrs[i].pid == -1)
	    break;
    }
    
    return i;
}

int add_tmr (pid_t pid, unsigned int ms)
{
    int tmr;
    unsigned int ticks;

    if ((tmr = get_free_tmr ()) < 0) {
        DPRINTF ("XMK: add_tmr -> Out of timers\r\n");
	return -1;
    }

    ticks = ms_to_ticks (ms);
    ticks = (ticks == 0) ? 1 : ticks;                                   // Bump it up a little

    soft_tmrs[tmr].pid = pid;
    soft_tmrs[tmr].timeout = ticks;
    active_tmrs[nactive] = tmr;
    nactive++;

    return 0;    
}

unsigned int remove_tmr (pid_t pid)
{
    int i;
    unsigned int remain = 0;

    for (i = 0; i < MAX_TMRS; i++) {
        if (soft_tmrs[i].pid == pid) {
            remain = ticks_to_ms (soft_tmrs[i].timeout);
            update_active_tmrs_list (pid);
            break;
        }
    }

    return remain;
}

// Timer lists A&B.
// One acts as a shadow to the other always. On every timer update event,
// the current list is operated on, while the shadow list keeps getting updates
void soft_tmr_handler ()
{
    int i;
    int* tmrshadow;
    int curactive = 0;

    tmrshadow = get_tmr_list_shadow(active_tmrs);                       // Get the list that can act as a shadow to the current list
    for (i=0; i < nactive; i++) {
	soft_tmrs[active_tmrs[i]].timeout--;
        if (soft_tmrs[active_tmrs[i]].timeout == 0) {                   // Timer expired. Unblock the process
	    handle_timeout (soft_tmrs[active_tmrs[i]].pid);
	    soft_tmrs[active_tmrs[i]].pid = -1;
	}
	else {
	    tmrshadow[curactive] = active_tmrs[i];                      // Append timer id to newly formed list
	    curactive++;
	}
    }
    
    active_tmrs = tmrshadow;
    nactive = curactive;
}

//----------------------------------------------------------------------------------------------------//
//  @func - sys_xget_clock_ticks
//! @desc 
//!   Return kernel ticks elapsed since system start (does not convey overflow information)
//! @param
//!   - None
//! @return
//!   - Number of kernel ticks elapsed
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
unsigned int sys_xget_clock_ticks ()
{
    return kernel_ticks;
}

//----------------------------------------------------------------------------------------------------//
//  @func - sys_time
//! @desc 
//!   Return number of seconds elapsed since kernel start.
//! @param
//!   - timer is a pointer to the time_t structure to return the result in
//! @return
//!   - Number of seconds elapsed since kernel start
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
time_t sys_time (time_t *timer)
{
    time_t secs = 0;

    secs = ((kernel_ticks * SYSTMR_INTERVAL)/ SYSTMR_CLK_FREQ);
    
    if (timer)
        *timer = secs;

    return secs;
}

//----------------------------------------------------------------------------------------------------//
//  @func - sys_sleep
//! @desc 
//!   Suspend current process for specified number of ticks
//!   1 tick = one timer interrupt received by the kernel
//! @param
//!   - ms is the number of milliseconds to sleep this task.
//! @return
//!   - Number of milliseconds unslept for. 0 - complete success
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
unsigned sys_sleep (unsigned int ms)
{
    pid_t cpid = sys_get_currentPID();
    int tmr;
    
    if (add_tmr (cpid, ms) == -1)
        return ms;

    current_process->state = PROC_DELAY;
    suspend ();

    // Awaken here after timeout

    return 0;
}
#endif /* CONFIG_TIME */
