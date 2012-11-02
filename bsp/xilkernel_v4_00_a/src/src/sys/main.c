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
// $Id: main.c,v 1.1.4.1 2007/08/20 06:08:22 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file main.c
//! Initialises the system by calling sys_init(), hw_init() and blocks the kernel
//! On first timer interrupt the first process gets scheduled.
//----------------------------------------------------------------------------------------------------//

#include <os_config.h>

#ifdef MB_XILKERNEL
#include <sys/process.h>
#endif

#ifdef PPC_XILKERNEL
#include <config/config_param.h>
#include <config/config_cparam.h>
#include <sys/ktypes.h>
#include <xparameters.h>
#include <sys/process.h>
#include <xpseudo_asm.h>
#include <xexception_l.h>
#include <xtime_l.h>
#include <sys/syscall.h>
#ifdef CONFIG_CACHE
#include <xcache_l.h>
#endif
#endif /* PPC_XILKERNEL */
#include <sys/decls.h>
#include <sys/init.h>
#include <pthread.h>
#include <xmk.h>

//----------------------------------------------------------------------------------------------------//
// Declarations
//----------------------------------------------------------------------------------------------------//
#ifdef CONFIG_STATIC_ELF_PROCESS_SUPPORT
extern struct _process_init se_process_table[] ;
#endif

#ifdef CONFIG_PTHREAD_SUPPORT
extern pthread_attr_t default_attr;

#ifdef CONFIG_STATIC_PTHREAD_SUPPORT
extern struct _elf_pthread_init kb_pthread_table[];
#endif
#endif

extern void idle_task (void);
extern void init_idle_task ();
void xilkernel_init ();
void xilkernel_start ();

//----------------------------------------------------------------------------------------------------//
// Definitions
//----------------------------------------------------------------------------------------------------//


//----------------------------------------------------------------------------------------------------//
//  @func - xilkernel_main
//! @desc 
//!   Entry point of the kernel
//! @return
//!   - Nothing.
//! @note
//!   - Routine does not return. (Caller does not get back control)
//----------------------------------------------------------------------------------------------------//
void xilkernel_main()
{
    DBG_PRINT("XMK: Starting kernel.\r\n");
    
    xilkernel_init ();
    xilkernel_start ();
}

//----------------------------------------------------------------------------------------------------//
//  @func - xilkernel_init
//! @desc 
//!   Initialize the system - This function is called at the start of system. 
//!   It initializes the system. 
//!   - Initializes the process vector table.
//!   - Creates the Idle process (pid - 0). 
//!   - Creates the static set of processes.
//! @return
//!   - Nothing.
//----------------------------------------------------------------------------------------------------//
void xilkernel_init()
{
    unsigned int i = 0 ;
    int ret;

    DBG_PRINT("XMK: Initializing Hardware.\r\n");
    hw_init();                                                  // Do hardware specific initialization

    DBG_PRINT("XMK: System initialization.\r\n");
    for( ; i < MAX_PROCESS_CONTEXTS; i++ ) {
        ptable[i].is_allocated = 0 ;
        ptable[i].pcontext.isrflag = 0;
    }

#ifdef MB_XILKERNEL
    kernel_sp = (void*)((unsigned int)&_stack + SSTACK_PTR_ADJUST); 
#elif defined(PPC_XILKERNEL)
    kernel_sp = (void*)((unsigned int)&__stack + SSTACK_PTR_ADJUST); 
#endif
    readyq_init();
    
#ifdef CONFIG_PTHREAD_SUPPORT
    pthread_init();
#endif
#ifdef CONFIG_SEMA
    sem_heap_init();
#endif
#ifdef CONFIG_MSGQ
    msgq_init();
#endif
#ifdef CONFIG_SHM
    shm_init();
#endif
#ifdef CONFIG_BUFMALLOC
    bufmalloc_init ();
#endif

    init_idle_task ();
                                                                     
#ifdef CONFIG_STATIC_ELF_PROCESS_SUPPORT
    ret = se_process_init() ;                                           // Create statically specified separate executable processes
#endif

#ifdef CONFIG_STATIC_PTHREAD_SUPPORT
    ret = kb_pthread_init ();                                           // Create statically specified kernel bundled threads
#endif

#ifdef CONFIG_TIME
    soft_tmr_init ();
#endif
}

//----------------------------------------------------------------------------------------------------//
//  @func - xilkernel_start
//! @desc
//!   Start the kernel by enabling interrupts and starting to execute the idle task.
//! @return
//!   - Nothing.
//! @note
//!   - Routine does not return.
//! @desc 
//----------------------------------------------------------------------------------------------------//
void xilkernel_start ()
{
    DBG_PRINT("XMK: Process scheduling starts.\r\n");
#if defined (MB_XILKERNEL)
    microblaze_enable_interrupts();
#elif defined (PPC_XILKERNEL)
    XExc_mEnableExceptions (XEXC_NON_CRITICAL);	
#endif

    idle_task ();                                                       // Does not return
}

#ifdef CONFIG_STATIC_ELF_PROCESS_SUPPORT
//----------------------------------------------------------------------------------------------------//
//  @func - se_process_init
//! @desc
//!   Create the statically specified set of separate executable processes.
//! @return
//!   - 0 on success
//!   - -1 on error 
//! @note
//!   - Used only in the case of separate executable process support
//----------------------------------------------------------------------------------------------------//
int se_process_init()
{
    struct _process_init *pinit ;
    unsigned int i = 0 ;

    // Atleast a single process should be loaded during initialisation.
    pinit = se_process_table;
    for( i = 0; i < N_INIT_PROCESS; i++) {
	if (sys_elf_process_create((void*)pinit->p_start_addr, pinit->priority) < 0) {
	    DBG_PRINT ("XMK: se_process_init: sys_process_create failed.\r\n");
	    return -1;
	}
	pinit++ ;
    }
    return 0;
}
#endif /* CONFIG_STATIC_ELF_PROCESS_SUPPORT */

#ifdef CONFIG_PTHREAD_SUPPORT
#ifdef CONFIG_STATIC_PTHREAD_SUPPORT
//----------------------------------------------------------------------------------------------------//
//  @func - kb_pthread_init
//! @desc
//!   Create the statically specified pthreads that do not have an ELF file associated with it. 
//!   Threads in kernel space. Stack allocated from BSS memory pool.
//! @return
//!   - 0 on success
//!   - error code from sys_pthread_create
//! @note
//!   - None
//----------------------------------------------------------------------------------------------------//
int kb_pthread_init() 
{
    struct _elf_pthread_init *pinit ;
    unsigned int i = 0 ;
    pthread_t tid;
    pthread_attr_t attr = default_attr;
    int ret;

    // Load the system processes to run during init. 
    pinit = kb_pthread_table;

    for( i=0; i<N_INIT_SELF_PTHREADS; i++) {
#if SCHED_TYPE == SCHED_PRIO
	attr.schedparam.sched_priority = pinit->priority;
#endif
	ret = sys_pthread_create (&tid, &attr, (void*)pinit->start_func, NULL);
	if (ret != 0) {
	    DBG_PRINT("XMK: kb_pthread_init: sys_pthread_create failed.\r\n");
	    return -1;
	}
	pinit++;
    }

    return 0;
}
#endif /* CONFIG_STATIC_PTHREAD_SUPPORT */
#endif /* CONFIG_PTHREAD_SUPPORT */

