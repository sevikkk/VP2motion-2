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
// $Id: kpthread.h,v 1.1.4.1 2007/08/20 06:08:21 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file kpthread.h
//! Kernel pthread declarations and definitions
//----------------------------------------------------------------------------------------------------//
#ifndef _KPTHREAD_H
#define _KPTHREAD_H

#include <os_config.h>
#include <config/config_cparam.h>
#include <config/config_param.h>
#include <sys/ktypes.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

// Defines

#define PTHREAD_INVALID          0xFF
#define PTHREAD_MUTEX_INVALID    0xFFFF
// Sched state
#define PTHREAD_STATE_ALIVE     1
#define PTHREAD_STATE_EXIT      2
#define PTHREAD_STATE_DETACHED  3
#define PTHREAD_STATE_BLOCKED   4


// pthreads - Kernel implementation
int       sys_pthread_create (pthread_t *thread, const pthread_attr_t *attr, void *(*start_func)(void*), void *param);
void      sys_pthread_exit (void *retval) ;
int       sys_pthread_join (pthread_t target, void **retval);
pthread_t sys_pthread_self (void);
int       sys_pthread_detach (pthread_t thread);
int       sys_pthread_equal (pthread_t thread_1, pthread_t thread_2);
int       sys_pthread_mutex_init (pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int       sys_pthread_mutex_destroy (pthread_mutex_t *mutex);
int       sys_pthread_mutex_lock (pthread_mutex_t *mutex);
int       sys_pthread_mutex_trylock (pthread_mutex_t *mutex);
int       sys_pthread_mutex_unlock (pthread_mutex_t *mutex);
int       sys_pthread_getschedparam (pthread_t thread, int *policy, struct sched_param *param);
int       sys_pthread_setschedparam (pthread_t thread, int policy, struct sched_param *param);

#ifdef __cplusplus
}       
#endif 

#endif /* _KPTHREAD_H */
