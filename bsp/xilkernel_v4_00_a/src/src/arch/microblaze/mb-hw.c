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
// $Id: mb-hw.c,v 1.1.4.2.20.1 2008/08/06 20:39:06 vasanth Exp $
//
//////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------------------------------//
//! @file mb-hw.c
//! Microblaze hardware specific initialization
//----------------------------------------------------------------------------------------------------//
#include <os_config.h>
#include <sys/process.h>
#include <config/config_param.h>
#include <config/config_cparam.h>
#include <xbasic_types.h>
#include <xstatus.h>
#include <xparameters.h>
#include <mb_interface.h>
#include <sys/decls.h>
#include <microblaze_exceptions_g.h>

#ifdef CONFIG_TIMER_PIT
#include <xtmrctr_l.h>
#endif
#ifdef CONFIG_INTC
#include <xintc.h>
#endif

#define TIMER_COUNTER_0         0

//----------------------------------------------------------------------------------------------------//
// Data
//----------------------------------------------------------------------------------------------------// 
#ifdef CONFIG_INTC
XIntc sys_intc;
#endif

extern pid_t current_pid;
extern char did_resched;
extern char timer_need_refresh;
extern unsigned char sched_partial_tick;

//----------------------------------------------------------------------------------------------------//
// Declarations
//----------------------------------------------------------------------------------------------------//
void    hw_init();
void    int_system_init();
void    setup_initial_context (process_struct *pcb, pid_t parent, unsigned int start_addr, unsigned int stackaddr);
void    setup_usr_irq_context (pid_t pid);
void    restore_kernel_context ();
void    init_idle_task ();
void    pit_initialize(unsigned int interval);
unsigned 
int     pit_get_value();
void    pit_set_interval( unsigned int interval);
void    pit_reset();
void    pit_continue();
void    pit_disable ();
void    pit_stop();
void    pit_handle_interrupt();
void    microblaze_report_exception (unsigned int esr, unsigned int ear, unsigned int pc);

extern void timer_int_handler(void *unused);
extern int  mpu_init ();
#ifdef CONFIG_INTC
extern XIntc_Config XIntc_ConfigTable[];
#endif

//----------------------------------------------------------------------------------------------------//
// Definitions
//----------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------//
//  @func - hw_init
//! @desc
//!   Microblaze hardware specific initialization
//!   - If an interrupt controller is present register interrupt controller handler
//!     as the main interrupt handler. Register timer with interrupt controller handler
//!   - Else, Register timer interrupt handler as the main interrupt handler
//!   - Configure PIT is timer is PIT.
//! @param
//!   - none
//! @return
//!   - nothing
//! @note
//!   - none
//----------------------------------------------------------------------------------------------------//
void hw_init()
{

#if (XPAR_MICROBLAZE_USE_MMU >= 2) && !defined (XILKERNEL_MB_MPU_DISABLE)
    if (!mpu_init ()) {
        DBG_PRINT ("XMK: MPU Initialization failed. Kernel will run without memory protection.\r\n");
    }
#endif

#ifdef CONFIG_INTC
    int_system_init();
#else    
    // Assume appropriate timer as the interrupting source
    // and register the system handler for the interrupt.
    // microblaze_register_handler((XInterruptHandler)timer_int_handler, NULL);
#endif

#ifdef CONFIG_TIMER_PIT
    pit_initialize(SYSTMR_INTERVAL);
#endif
}

#ifdef CONFIG_INTC
//----------------------------------------------------------------------------------------------------//
//  @func - int_system_init
//! @desc
//!   Initialize microblaze interrupt handling system in the prescence of an interrupt controller.
//! @param
//!   - none
//! @return
//!   - nothing
//! @note
//!   - none
//----------------------------------------------------------------------------------------------------//
void int_system_init ()
{
    XStatus status;
    status = XIntc_Initialize (&sys_intc, SYSINTC_DEVICE_ID);
    if (status != XST_SUCCESS) {
	DBG_PRINT ("XMK Error: XIntc_Initialize failed.\r\n");
	return;
    }

    // IntC ISR should service all interrupts
    XIntc_SetIntrSvcOption (SYSINTC_BASEADDR, XIN_SVC_ALL_ISRS_OPTION);

    // Now tie the INTC's interrupt handler to the Microblaze main interrupt handler.
    // Argument is the INTC's device ID.
    // microblaze_register_handler ((XInterruptHandler)intc_device_interrupt_handler, SYSINTC_DEVICE_ID);


    // Register XMK timer interrupt handler to the timer interrupt
    status = XIntc_Connect (&sys_intc, SYSTMR_INTR_ID, timer_int_handler, NULL);
    if (status != XST_SUCCESS) {
	DBG_PRINT ("XMK Error: init_int_system: XIntc_Connect failed.\r\n");
	return;
    }
    // Enable timer interrupt
    XIntc_Enable (&sys_intc, SYSTMR_INTR_ID);

    // Start the interrupt controller
    status = XIntc_Start (&sys_intc, XIN_REAL_MODE);
    if (status != XST_SUCCESS) {
	DBG_PRINT ("XMK Error: init_int_system: XIntc_Start failed.\r\n");
	return;
    }
}

#endif /* CONFIG_INTC */

//----------------------------------------------------------------------------------------------------//
//  @func - xmk_enter_kernel
//! @desc
//!   Lock kernel by turning off ALL system interrupts
//! @return
//!   - Nothing
//! @note
//!   - Locks kernel against ALL interrupts.
//----------------------------------------------------------------------------------------------------//
inline void xmk_enter_kernel ()
{
    microblaze_disable_interrupts();
}

//----------------------------------------------------------------------------------------------------//
//  @func - xmk_leave_kernel
//! @desc
//!   Unlock kernel by turning on system timer interrupts
//! @return
//!   - Nothing
//! @note
//!   - Unlocks kernel with respect to ALL interrupts 
//----------------------------------------------------------------------------------------------------//
inline void xmk_leave_kernel( void ) 
{
    microblaze_enable_interrupts(); 
}

/* //! Setup context for user-level interrupt handler */
/* inline void setup_usr_irq_context (pid_t pid) */
/* { */
/*     asm volatile ("or   r2, r0, %0" : : "r" (ptable[pid].pcontext.regs[2])); */
/*     asm volatile ("or   r13, r0, %0" : : "r" (ptable[pid].pcontext.regs[13])); */
/* }  */

/* //! Restore kernel context */
/* inline void restore_kernel_context () */
/* { */
/*     asm volatile ("or   r2, r0, %0" : : "r" (ptable[0].pcontext.regs[2])); */
/*     asm volatile ("or   r13, r0, %0" : : "r" (ptable[0].pcontext.regs[13])); */
/* } */


void setup_initial_context (process_struct *pcb, pid_t parent, unsigned int start_addr, unsigned int stackaddr)
{
    pcb->pcontext.regs[1] = stackaddr;
    pcb->pcontext.regs[14] = start_addr;
    pcb->pcontext.regs[2] = ptable[parent].pcontext.regs[2];
    pcb->pcontext.regs[13] = ptable[parent].pcontext.regs[13];
    pcb->pcontext.regs[32] = ptable[parent].pcontext.regs[32];          // Inherit MSR
    pcb->pcontext.isrflag = ISRFLAG_NEW_PROC;
}

void init_idle_task ()
{
    unsigned int msr;

    idle_task_pid = proc_create (PRIO_LOWEST);                          // Idle task (PID 0).
    ptable[idle_task_pid].state = PROC_RUN;                             // Idle task assumed to be running as soon as the kernel starts
    ptable[idle_task_pid].pcontext.regs[1] = (unsigned int)kernel_sp;
    ptable[idle_task_pid].pcontext.regs[14] = (unsigned int)idle_task;
    asm volatile ("or   %0, r0, r2" : "=r"  (ptable[idle_task_pid].pcontext.regs[2]));
    asm volatile ("or   %0, r0, r13" : "=r" (ptable[idle_task_pid].pcontext.regs[13]));
    asm volatile ("mfs  %0, rmsr" : "=r" (msr));
    ptable[idle_task_pid].pcontext.regs[32] = msr;
    SET_CURRENT_PROCESS (idle_task_pid);
}

#ifdef CONFIG_TIMER_PIT
//----------------------------------------------------------------------------------------------------//
//  @func - pit_initialize
//! @desc
//!   Initialize the PIT timer in the system
//! @param
//!   - interval is the interval to interrupt the system at.
//! @return
//!   - nothing
//! @note
//!   - none
//----------------------------------------------------------------------------------------------------//
void pit_initialize (unsigned int interval)
{

    // Set the load register value. This is the number of cycles the timer counts before interrupting.
    // -2 because the actual number of cycles will be +2
    pit_set_interval (interval);
    
    // Load the interval value from the load register and reset the interrupt occurred mask
    XTmrCtr_mSetControlStatusReg (SYSTMR_BASEADDR, TIMER_COUNTER_0,
				 XTC_CSR_INT_OCCURED_MASK | XTC_CSR_LOAD_MASK );

    // Configure the timer for generate mode, auto-reload mode, down count, interrupts enabled.
    // Also, start the timer.
    XTmrCtr_mSetControlStatusReg (SYSTMR_BASEADDR, TIMER_COUNTER_0,
				   XTC_CSR_ENABLE_TMR_MASK | XTC_CSR_ENABLE_INT_MASK |
				   XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_DOWN_COUNT_MASK);
}

//----------------------------------------------------------------------------------------------------//
//  @func - pit_reset
//! @desc
//!   Enable and start PIT timer with configured PIT interval loaded to the PIT counter
//! @param
//!   - none
//! @return
//!   - nothing
//! @note
//!   - none
//----------------------------------------------------------------------------------------------------//
void pit_reset ()
{
    unsigned int control_reg;
    
    // Obtain control register value from PIT device
    control_reg = XTmrCtr_mGetControlStatusReg (SYSTMR_BASEADDR, TIMER_COUNTER_0);

    // Refresh budget only if the timer ran out
    if (timer_need_refresh) {
        // Re-load the interval value from load register and reset the interrupt occurred mask
        XTmrCtr_mSetControlStatusReg (SYSTMR_BASEADDR, TIMER_COUNTER_0, 
                                      control_reg | XTC_CSR_LOAD_MASK | XTC_CSR_INT_OCCURED_MASK);
      
        // Remove the load mask and also enable the timer
        XTmrCtr_mSetControlStatusReg (SYSTMR_BASEADDR, TIMER_COUNTER_0, 
                                    (control_reg & (~XTC_CSR_LOAD_MASK)) | XTC_CSR_ENABLE_TMR_MASK);

        // Reset scheduler/timer state
        timer_need_refresh = 0;
        sched_partial_tick = 0;
        did_resched        = 0;

    } else { 
        // FIXME! 
        //  - did_resched seems bogus. We will be here only if we do a resched
        //    seems redundant. Leaving it in, because it is harmless if redundant
        if (did_resched) {
            did_resched = 0;
            sched_partial_tick = 1;
        }

        // Enable the timer. No need to reset interrupt. That would have been covered by previous conditional
        XTmrCtr_mSetControlStatusReg (SYSTMR_BASEADDR, TIMER_COUNTER_0,
                                      (control_reg | XTC_CSR_ENABLE_TMR_MASK));
    }
}
 
//----------------------------------------------------------------------------------------------------//
//  @func - pit_set_interval
//! @desc
//!   Set the count down value for the PIT device that is clocking Xilkernel. Also stores the
//!   interval value in a global pit_timer_interval. This stored value is unused currently, but
//!   in future can be used to do computations on clock budget for a process.
//! @param
//!   - interval
//! @return
//!   - nothing
//! @note
//!   - none
//----------------------------------------------------------------------------------------------------//
void pit_set_interval (unsigned int interval)
{
    XTmrCtr_mWriteReg (SYSTMR_BASEADDR, TIMER_COUNTER_0,
                       XTC_TLR_OFFSET, interval);
}

//----------------------------------------------------------------------------------------------------//
//  @func - pit_disable
//! @desc
//!   Disable the PIT device so that it can no longer run and interrupt the system.
//! @param
//!   - none
//! @return
//!   - nothing
//! @note
//!   - Assumes timer was previously configured.
//----------------------------------------------------------------------------------------------------//
void pit_disable ()
{
    unsigned int control_reg;

    // Obtain control register value from PIT device
    control_reg = XTmrCtr_mGetControlStatusReg (SYSTMR_BASEADDR, TIMER_COUNTER_0);

    // Remove the enable mask and update the status register
    control_reg &= ~(XTC_CSR_ENABLE_TMR_MASK);
    XTmrCtr_mSetControlStatusReg (SYSTMR_BASEADDR, TIMER_COUNTER_0, control_reg);
}

//----------------------------------------------------------------------------------------------------//
//  @func - pit_continue
//! @desc
//!   Enable the PIT device so that it can continue counting
//! @param
//!   - none
//! @return
//!   - nothing
//! @note
//!   - Assumes timer was appropriately configured and eventually stopped, previously.
//----------------------------------------------------------------------------------------------------//
/* void pit_continue () */
/* { */
/*     unsigned int control_reg; */
/*     control_reg = XTimerCtr_mReadReg(SYSTMR_BASEADDR, TIMER_COUNTER_0, XTC_TCSR_OFFSET); */

/*     // remove the reset condition such that the timer counter starts running */
/*     XTmrCtr_mWriteReg(SYSTMR_BASEADDR, TIMER_COUNTER_0, */
/* 		      XTC_TCSR_OFFSET, */
/* 		      control_reg | XTC_CSR_ENABLE_TMR_MASK); */
/* } */

#else
void pit_reset () {}
void pit_disable () {}

#endif /* CONFIG_TIMER_PIT */


#ifdef MICROBLAZE_EXCEPTIONS_ENABLED
void microblaze_report_exception (unsigned int esr, unsigned int ear, unsigned int pc)
{
    DPRINTF ("XMK: pid (%d) performed illegal operation @ PC (0x%x),\r\n\tfault code: (0x%x).\r\n\tfault address: (0x%x).\r\n", current_pid, pc, esr, ear);
    DPRINTF ("XMK: Terminating (%d).\r\n", current_pid);
}
#endif


