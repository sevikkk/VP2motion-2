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
// $Id: alloc.c,v 1.1.4.1.22.1 2008/02/21 00:18:44 vasanth Exp $
//
//////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------------------------------//
//! @file alloc.c
//! Kernel data structures to be allocated in the BSS section.
//----------------------------------------------------------------------------------------------------//

#include <os_config.h>
#include <config/config_param.h>
#include <sys/arch.h>

#if PTHREAD_STACK_SIZE < 1024
#define KERNEL_IRQ_STACKSZ 1024
#else
#define KERNEL_IRQ_STACKSZ PTHREAD_STACK_SIZE
#endif

//! kernel_irq_stack - Stack to be used on IRQs
char kernel_irq_stack[KERNEL_IRQ_STACKSZ] __attribute__ ((aligned (STACK_ALIGN)));
const char *kernel_irq_stack_ptr = (const char *)(kernel_irq_stack + KERNEL_IRQ_STACKSZ + SSTACK_PTR_ADJUST);

#ifdef CONFIG_ELF_PROCESS
//! process_startup_stack - Temporary initial stack to be used by processes 
//! before their own elf stacks are setup. This is potentially dangerous  
char process_startup_stack[PROCESS_STARTUP_STACKSZ] __attribute__ ((aligned (STACK_ALIGN)));
#endif

