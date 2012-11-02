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
// $Id: libintr.c,v 1.1.4.1 2007/08/20 06:08:23 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file libintr.c
//! API to manipulate interrupt controller and register user handlers for 
//! interrupts in the system
//----------------------------------------------------------------------------------------------------//
#include <os_config.h>
#include <sys/syscall.h>
#include <sys/intr.h>

extern void* make_syscall (void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, int syscall_num);

#ifdef CONFIG_INTC
unsigned int register_int_handler (int_id_t intr_id, void (*handler)(void*), void *callback)
{
    return (unsigned int) make_syscall ((void*) (unsigned int)intr_id, (void*)handler, (void*)callback, NULL, NULL, SC_REGISTER_INT_HANDLER);
}

void unregister_int_handler (int_id_t intr_id)
{
    make_syscall ((void*) (unsigned int) intr_id, NULL, NULL, NULL, NULL, SC_UNREGISTER_INT_HANDLER);
}

void enable_interrupt (int_id_t intr_id)
{
    make_syscall ((void*) (unsigned int) intr_id, NULL, NULL, NULL, NULL, SC_ENABLE_INTERRUPT);
}

void disable_interrupt (int_id_t intr_id)
{
    make_syscall ((void*) (unsigned int)intr_id, NULL, NULL, NULL, NULL, SC_DISABLE_INTERRUPT);
}

void acknowledge_interrupt (int_id_t intr_id)
{
    make_syscall ((void*) (unsigned int)intr_id, NULL, NULL, NULL, NULL, SC_ACKNOWLEDGE_INTERRUPT);
}
#endif /* CONFIG_INTC */
