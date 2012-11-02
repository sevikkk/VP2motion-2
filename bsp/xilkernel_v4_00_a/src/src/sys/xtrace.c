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
// $Id: xtrace.c,v 1.1.4.1 2007/09/27 02:32:55 vasanth Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file xtrace.c
//!
//! XMK API for tracing kernel level and custom user events. 
//! Requires trace memory in the system accessible to program.
//! @note - This feature is for internal use only.
//----------------------------------------------------------------------------------------------------//
#include <os_config.h>
#include <sys/types.h>
#include <sys/decls.h>
#include <sys/xtrace.h>

#ifdef CONFIG_XTRACE

//----------------------------------------------------------------------------------------------------//
// Declarations
//----------------------------------------------------------------------------------------------------//
const char *xtrace_event_name[] = {
    "SCHED",
    "SEM",
    "TIMER",
    "CUSTOM"
};

const char *xtrace_action_name[] = {
    "resched",
    "wait",
    "timewait",
    "wait_timeout",
    "wait_unblock",
    "acquire",
    "post",
    "post_unblock",
    "timeout",
    "tick",
    "add",
    "remove",
    "action1",
    "action2",
    "action3",
    "action4",
    "action5",
    "action6"
};

extern pid_t current_pid;

//----------------------------------------------------------------------------------------------------//
// Data
//----------------------------------------------------------------------------------------------------// 
unsigned int    xtrace_first, xtrace_last, xtrace_count;
xtrace_pkt_t    *xtracebuf    = (xtrace_pkt_t *)(CONFIG_XTRACE_MEM_START + 8);
unsigned int    xtrace_off;

//----------------------------------------------------------------------------------------------------//
// Definitions
//----------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------//
//  @func - xtrace_log_event 
//!
//! 
//! @param
//!   - 
//! @return
//!   - Nothing
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
void xtrace_log_event (enum xtrace_event event, enum xtrace_action action, 
                       unsigned int resource, unsigned int custom0, unsigned int custom1)
{
    if (xtrace_off)
        return;

    xtracebuf[xtrace_last].event = event;
    xtracebuf[xtrace_last].pid   = current_pid;
    xtracebuf[xtrace_last].resource = resource;
    xtracebuf[xtrace_last].action = action;
    xtracebuf[xtrace_last].custom[0] = custom0;
    xtracebuf[xtrace_last].custom[1] = custom1;
    
    if (xtrace_count != CONFIG_XTRACE_MAX_COUNT)
        xtrace_count++;
    
    xtrace_last++;
    if (xtrace_last == CONFIG_XTRACE_MAX_COUNT) 
        xtrace_last = 0;
    
    if (xtrace_last == xtrace_first) {
        xtrace_first++;
        if (xtrace_first == CONFIG_XTRACE_MAX_COUNT)
            xtrace_first = 0;
    } 
}

//----------------------------------------------------------------------------------------------------//
//  @func - xtrace_print_log 
//!
//! 
//! @param
//!   - 
//! @return
//!   - Nothing
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
void xtrace_print_log (int count, int last)
{
    int i, start, end;
    
    DPRINTF ("XTrace Log\r\n");
    DPRINTF ("\tTotal events recorded: %d.\r\n", xtrace_count);

    if (count == 0)
        count = xtrace_count;
    
    if (last) {
        start = (xtrace_last - count - 1);
        if (start < 0)
            start = (CONFIG_XTRACE_MAX_COUNT + start);
    } else 
        start = xtrace_first;
    
    end = (start + count) % CONFIG_XTRACE_MAX_COUNT;
    
    DPRINTF ("\tDumping (%d) %s events next --\r\n", count, (last ? "trailing" : "beginning"));
    
    i = start;
    while (i != end) {
        if (xtracebuf[i].event >= XCUSTOM_EVENT) {
            DPRINTF ("E%04d: [%6s]-(%15s) PID(%03d) on (%03d). custom - [0x%x], [0x%x]\r\n",
                     i,
                     xtrace_event_name[xtracebuf[i].event],
                     xtrace_action_name[xtracebuf[i].action],
                     xtracebuf[i].pid,
                     xtracebuf[i].resource,
                     xtracebuf[i].custom[0],
                     xtracebuf[i].custom[1]);
        } else {
            DPRINTF ("E%04d: [%6s]-(%15s) PID(%03d) on (%03d).\r\n",
                     i,
                     xtrace_event_name[xtracebuf[i].event],
                     xtrace_action_name[xtracebuf[i].action],
                     xtracebuf[i].pid,
                     xtracebuf[i].resource);
        }
        
        if (++i == CONFIG_XTRACE_MAX_COUNT)
            i = 0;
    }
    
    while (1);
}
    
#endif /* CONFIG_XTRACE */

