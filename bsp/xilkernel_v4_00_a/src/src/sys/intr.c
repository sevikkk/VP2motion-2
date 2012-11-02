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
// $Id: intr.c,v 1.1.4.1 2007/08/20 06:08:22 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file intr.c
//!
//! XMK API for registering and managing interrupt handlers for device 
//! interrupts. Depends on an interrupt controller being present in the system
//! @note - We need to add more thread specific context switch (such as __errno) before invoking 
//!         user interrupt handlers
//----------------------------------------------------------------------------------------------------//
#include <os_config.h>

#ifdef CONFIG_INTC
#include <xparameters.h>
#include <sys/intr.h>
#include <xintc.h>
#include <sys/types.h>
#include <sys/decls.h>


//! Interrupt descriptor for each interrupt
typedef struct intr_desc_s {
    XInterruptHandler   handler;
    void                *callback;
    pid_t               pid;
} intr_desc_t;

//----------------------------------------------------------------------------------------------------//
// Declarations
//----------------------------------------------------------------------------------------------------//
unsigned int    sys_register_int_handler (int_id_t intr_id, void (*handler)(void*), void *callback);
void            sys_unregister_int_handler (int_id_t intr_id);
void            sys_enable_interrupt (int_id_t intr_id);
void            sys_disable_interrupt (int_id_t intr_id);
void            sys_acknowledge_interrupt (int_id_t intr_id);
void            irq_wrapper (void* intr_id);

extern XIntc sys_intc;
extern void setup_usr_irq_context (pid_t pid);
extern void restore_kernel_context ();
//----------------------------------------------------------------------------------------------------//
// Data
//----------------------------------------------------------------------------------------------------// 
#if 0
intr_desc_t     idt[XPAR_INTC_MAX_NUM_INTR_INPUTS]; 
#endif

//----------------------------------------------------------------------------------------------------//
// Definitions
//----------------------------------------------------------------------------------------------------//

#if 0                   // Not being used currently
//----------------------------------------------------------------------------------------------------//
//  @func - irq_wrapper
//! @desc
//!   Set and unset appropriate context before invoking user level interrupt handlers. The exact context
//!   to be saved and restored is architecture specific.
//! @param
//!   - intr_id is the zero based ID of the interrupt
//! @return
//!   - Nothing
//! @note
//!   - none
//----------------------------------------------------------------------------------------------------//
void irq_wrapper (void* intr_id)
{
    int_id_t id = (int_id_t)((unsigned int)(intr_id) & 0xff);
    setup_usr_irq_context (idt[id].pid);                        //! Setup context for user-level interrupt handler
    idt[id].handler (idt[id].callback);                         //! Invoke handler 
    restore_kernel_context ();                                  //! Restore kernel context
}
#endif 

//----------------------------------------------------------------------------------------------------//
//  @func - sys_register_int_handler
//! @desc
//!   Register a handler with the interrupt controller for the specified interrupt
//! @param
//!   - intr_id is the zero based ID of the interrupt
//!   - handler is the requested handler
//!   - callback is a pointer to a callback value
//! @return
//!   - return XST_SUCCESS (defined in xstatus.h) on success. 
//!   - return -1 in MB_XILKERNEL, if trying to register a handler for the system timer interrupt.
//!   - Other error codes on failure (look at xstatus.h for other error codes)
//! @note
//!   - none
//----------------------------------------------------------------------------------------------------//
unsigned int sys_register_int_handler (int_id_t intr_id, void (*handler)(void*), void *callback)
{
#ifdef MB_XILKERNEL
    // Disallow playing with timer interrupt
    if (intr_id == SYSTMR_INTR_ID )
	return (unsigned int)-1;
#endif

    return XIntc_Connect (&sys_intc, intr_id, (XInterruptHandler)handler, callback);
}

//----------------------------------------------------------------------------------------------------//
//  @func - sys_unregister_int_handler
//! @desc
//!   Unregister a handler with the interrupt controller for the specified interrupt
//! @param
//!   - intr_id is the zero based ID of the interrupt
//! @return
//!   - Nothing
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
void sys_unregister_int_handler (int_id_t intr_id)
{
#ifdef MB_XILKERNEL
    // Disallow playing with timer interrupt
    if (intr_id == SYSTMR_INTR_ID )
        return;
#endif

    XIntc_Disconnect (&sys_intc, intr_id);
}

//----------------------------------------------------------------------------------------------------//
//  @func - sys_enable_interrupt
//! @desc
//!   Enable interrupts from the specified interrupt source
//! @param
//!   - intr_id is the zero based ID of the interrupt
//! @return
//!   - Nothing
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
void sys_enable_interrupt (int_id_t intr_id)
{
#ifdef MB_XILKERNEL
    // Disallow playing with timer interrupt
    if (intr_id == SYSTMR_INTR_ID )
        return;
#endif
    XIntc_Enable (&sys_intc, intr_id);
}

//----------------------------------------------------------------------------------------------------//
//  @func - sys_disable_interrupt
//! @desc
//!   Disable interrupts from the specified interrupt source
//! @param
//!   - intr_id is the zero based ID of the interrupt
//! @return
//!   - Nothing
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
void sys_disable_interrupt (int_id_t intr_id)
{
#ifdef MB_XILKERNEL
    // Disallow playing with timer interrupt
    if (intr_id == SYSTMR_INTR_ID ) 
        return;
#endif
    XIntc_Disable (&sys_intc, intr_id);
}

//----------------------------------------------------------------------------------------------------//
//  @func - sys_acknowledge_interrupt
//! @desc
//!   Acknowledge interrupt from the specified interrupt source
//! @param
//!   - intr_id is the zero based ID of the interrupt
//! @return
//!   - Nothing
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
void sys_acknowledge_interrupt (int_id_t intr_id)
{
#ifdef MB_XILKERNEL
    // Disallow playing with timer interrupt
    if (intr_id == SYSTMR_INTR_ID ) 
        return;
#endif

    XIntc_Acknowledge (&sys_intc, intr_id);
}

#endif /* CONFIG_INTC */

