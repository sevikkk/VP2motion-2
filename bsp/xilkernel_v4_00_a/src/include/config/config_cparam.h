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
// $Id: config_cparam.h,v 1.1.4.1 2007/08/20 06:08:21 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file config_cparam.h
//! This contains the configuration parameter's for Message Queue, 
//! Shared Memory and Dynamic Buffer mgmt routines. The following fields are 
//! defined  based on the values in sys/init.c
//----------------------------------------------------------------------------------------------------//
#ifndef CONFIG_CPARAM_H
#define CONFIG_CPARAM_H

#include <os_config.h>
#include <config/config_param.h>

/************************************************************************/
/* Memory sizes for the various memory blocks			        */
/************************************************************************/

// The total Memory needed for all PID queue's in the system. This includes
// ready queue and semaphore wait queues.

#ifdef CONFIG_SEMA
#define PID_QUEUE_MSIZE  ((N_PRIO*MAX_READYQ)+(MAX_SEM*MAX_SEM_WAITQ))
#else
#define PID_QUEUE_MSIZE  (N_PRIO*MAX_READYQ)
#endif

#ifdef CONFIG_PTHREAD_MUTEX
#define PTHREAD_MUTEX_QUEUE_MSIZE (MAX_PTHREAD_MUTEX * MAX_PTHREAD_MUTEX_WAITQ)
#endif

#ifdef CONFIG_PTHREAD_SUPPORT
// The total memory needed for all the thread context. Calculated based on
// definition in config_param.h
#define PTHREAD_STACK_MSIZE (MAX_PTHREADS * PTHREAD_STACK_SIZE)
#endif

#ifdef CONFIG_SHM
// Total Memory size for the various Shared Memory
// This is how it is calculated from (struct _shm_init):	
// -# Add msize of all the shared memory to get the total msize
#ifndef SHM_MSIZE 
#define SHM_MSIZE 	 100
#endif
#endif

#ifdef CONFIG_MALLOC
// Total Memory size for the Dynamic buffer management	
// This is how it is calculated from (struct _malloc_init):
//  -#	msize for a single memory block = mem_bsize * n_blocks
//  -#	Add msize of all the Memory blocks to get the total msize
 
#ifndef MALLOC_MSIZE 
#define MALLOC_MSIZE 	 120
#endif
#endif

/************************************************************************/
/* Maximum number of various elements - See config_init.h		*/
/************************************************************************/
#ifndef N_INIT_PROCESS 
#define N_INIT_PROCESS 	0	
#endif

#ifndef N_INIT_SELF_PTHREADS
#define N_INIT_SELF_PTHREADS   0
#endif
#ifndef N_INIT_MELF_PTHREADS
#define N_INIT_MELF_PTHREADS   0
#endif

#ifdef CONFIG_SHM
#ifndef N_SHM 
#define N_SHM		1	
#endif
#endif

#ifdef CONFIG_MALLOC
#ifndef N_MALLOC_BLOCKS 
#define N_MALLOC_BLOCKS	2	
#endif
#ifndef TOT_MALLOC_BLOCKS 
#define TOT_MALLOC_BLOCKS 20	// Total number of memory blocks in the 
				// system. This is:			
				//  - Sum of n_blocks field of all elements  in malloc_config[]	 
#endif
#endif

#endif
