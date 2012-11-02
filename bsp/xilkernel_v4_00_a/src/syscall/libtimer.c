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
// $Id: libtimer.c,v 1.1.4.1 2007/08/20 06:08:23 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file libtimer.c
//! System call wrappers for timers
//----------------------------------------------------------------------------------------------------//
#include <os_config.h>
#include <sys/syscall.h>
#include <sys/timer.h>

extern void* make_syscall (void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, int syscall_num);

#ifdef CONFIG_TIME
unsigned int xget_clock_ticks ()
{
    return (unsigned int) make_syscall (NULL, NULL, NULL, NULL, NULL, SC_TMR_GETCLOCKTICKS);
}

time_t time (time_t *timer)
{
    return (time_t) make_syscall ((void*)timer, NULL, NULL, NULL, NULL, SC_TMR_TIME);
}

unsigned sleep (unsigned ticks)
{
    return (unsigned) make_syscall ((void*)ticks, NULL, NULL, NULL, NULL, SC_TMR_SLEEP);
}
#endif /* CONFIG_TIME */
