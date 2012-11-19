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
// $Id: config_init.h,v 1.1.4.1 2007/08/20 06:08:21 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file config_init.h 
//! This contains the values for system definition. This can be  generated by 
//! libgen and can also be modified by the user for customization.
//! - The structures are declared in sys/init.h
//! - If any changes are made to this file by the user, then they also need
//!   to change the corresponding fields in config_cparam.h file. 
//----------------------------------------------------------------------------------------------------//
#include <sys/init.h>
#include <os_config.h>


//----------------------------------------------------------------------------------------------------//
//! se_process_table contains the information about separate executable processes that 
//! need to be created during system init. This information is used by se_process_init().
//----------------------------------------------------------------------------------------------------//
struct _process_init se_process_table[] = {
   {0x0, 0 } 
} ;

#ifdef CONFIG_SHM
//----------------------------------------------------------------------------------------------------//
//! shm_config - User defined information about Shared Mem's are defined
//! here. It contains information about each shard mem in the system. This
//! is used by shm_init().
//----------------------------------------------------------------------------------------------------//
struct _shm_init shm_config[] = {
  { 100 },
} ;
#endif

#ifdef CONFIG_MALLOC
//----------------------------------------------------------------------------------------------------//
//! malloc_config - User defined information about dynamic memory blocks are 
//! defined here. This is used by malloc_init() to initialize the structures.
//----------------------------------------------------------------------------------------------------//
const struct _malloc_init malloc_config[] = {
  { 4, 10 },
  { 8, 10 },
} ;
#endif