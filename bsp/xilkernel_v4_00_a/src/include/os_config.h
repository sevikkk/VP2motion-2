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
// $Id: os_config.h,v 1.1.4.1 2007/08/20 06:08:21 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file os_config.h 
//! This file caontains all the configuration #define's for the system. The
//! different modules of the kernel can be configured by changing this file
//! and recompiling the kernel. Some of the modules that are configurable are:
//!  - Process Management
//!  - Thread Management
//!  - Scheduling type
//!  - Semaphore
//!  - Message Queue
//!  - Shared Memory
//!  - Dynamic Buffer Management
//!
//!  By recompiling, the kernel gets compiled with the defaults for each 
//!  modules. To further configure the kernel changes may have to be made in the
//!  following files:
//!  - config_param.h
//!  - config_cparam.h
//!  - config_init.h
//----------------------------------------------------------------------------------------------------//

#ifndef _OS_CONFIG_H
#define _OS_CONFIG_H

//! Timer ticks value used for scheduling.
#define TIMER_TICKS 2	

//! Process Management.
//! Further configuration of this module can be done by modifying 
//! config_param.h. The initial system processes are specified in sys/init.c file.
#define MAX_PROCS 10	// Max. number of Processesing contexts in the system 

//! Max. number of Processes in each Priority ready queue. This determines the size
//! of the Ready Queue. This is determined by the type of application.
#define MAX_READYQ 10	

#define CONFIG_KILL     1           //! Include kill() function 
#define CONFIG_YIELD    1           //! Include yield() function 

//! Type of process scheduling. There two types of scheduling supported and 
//! can be configured during system build.
#define SCHED_TYPE 3	//! SCHED_PRIO 


//! ELF Process Management
//! Further configuration of this module can be done by modifying 
//! config_param.h. 
#define CONFIG_ELF_PROCESS      1	// Support ELF process functionality

//! Thread Management
//! Further configuration of this module can be done by modifying 
//! config_param.h. 
#define CONFIG_PTHREAD_SUPPORT  1	//! Support thread functionality

//! Semaphore
//! Further configuration of this module can be done by modifying 
//! config_param.h. 

//! Include the Semapore module 
#define CONFIG_SEMA 	1

//! pthread mutex
//! Further configuration of this module can be done by modifying 
//! config_param.h. 

//! Include the Mutex module 
#define CONFIG_PTHREAD_MUTEX 	1

//! Message Queue
//! Further configuration of this module can be done by modifying 
//! config_cparam.h and sys/init.c.

//! Include the Message Queue functionality. 
#define CONFIG_MSGQ 	1

//! Shared Memory
//! Further configuration of this module can be done by modifying 
//! config_cparam.h and sys/init.c.
//! Include the Shared Memory Functionality 
#define CONFIG_SHM 	1

//! Dynamic Buffer Management
//! Further configuration of this module can be done by modifying 
//! config_cparam.h and sys/init.c.
//! Include the Dynamic buffer management functionality.
#define CONFIG_MALLOC 	1

//! Cache Enable
//! For PPC only
//! Further configuration can be done by modifying sys/main.c
//! Enable caches 
#define CONFIG_CACHE 1

//! Configure timer functionality
#define CONFIG_TIMERS 1

#endif /* _OS_CONFIG_H */
