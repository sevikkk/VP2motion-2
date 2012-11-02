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
// $Id: libpthread.c,v 1.1.4.1 2007/08/20 06:08:23 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------------------------------//
//! @file libpthread.c
//! This contains system call wrapper functions for pthread functionality.
//----------------------------------------------------------------------------------------------------//
#include <stdio.h>
#include <os_config.h>
#include <sys/process.h>
#include <sys/syscall.h>
#include <config/config_param.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ksched.h>
#include <sys/kpthread.h>
#include <sys/ktypes.h>

extern void* make_syscall (void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, int syscall_num);

#ifdef CONFIG_PTHREAD_SUPPORT
int pthread_create (pthread_t *thread, const pthread_attr_t *attr, void *(*start_func)(void *), void *param)
{
    return (int) make_syscall ((void*)thread, (void*)attr, (void*)start_func, (void*)param, NULL, SC_PTHREAD_CREATE);
}

void pthread_exit (void *retval)
{
    make_syscall ((void*)retval, NULL, NULL, NULL, NULL, SC_PTHREAD_EXIT);
}

int pthread_join (pthread_t thread, void **retval)
{
    return (int) make_syscall ((void*)thread, (void*)retval, NULL, NULL, NULL, SC_PTHREAD_JOIN);
}

pthread_t pthread_self (void)
{
    return (pthread_t) make_syscall (NULL, NULL, NULL, NULL, NULL, SC_PTHREAD_SELF);
}

int pthread_detach (pthread_t thread)
{
    return (int) make_syscall ((void*)thread, NULL, NULL, NULL, NULL, SC_PTHREAD_DETACH);
}

int pthread_equal (pthread_t thread_1, pthread_t thread_2)
{
    return (int) make_syscall ((void*)thread_1, (void*)thread_2, NULL, NULL, NULL, SC_PTHREAD_EQUAL);
}

int pthread_attr_init (pthread_attr_t *attr)
{
    if (attr == NULL) 
	return EINVAL;
   
    attr->contentionscope = PTHREAD_SCOPE_SYSTEM;
    attr->schedparam.sched_priority = PRIO_LOWEST;
    attr->detachstate = PTHREAD_CREATE_DETACHED;
    attr->stackaddr = NULL;
    attr->stacksize = 0;
    return 0;
}

int pthread_attr_destroy (pthread_attr_t *attr)
{
    if (attr == NULL) 
	return EINVAL;
    
    attr->contentionscope = PTHREAD_INVALID;
    attr->schedparam.sched_priority = PTHREAD_INVALID;
    attr->detachstate = PTHREAD_INVALID;
    attr->stackaddr = NULL;
    attr->stacksize = 0;
    return 0;
}

#if SCHED_TYPE == SCHED_PRIO
int pthread_attr_setschedparam (pthread_attr_t *attr, const struct sched_param *spar)
{
    if (attr == NULL || spar == NULL)
	return EINVAL;

    if (spar->sched_priority < PRIO_HIGHEST || spar->sched_priority > PRIO_LOWEST)
	return ENOTSUP;

    attr->schedparam.sched_priority = spar->sched_priority;
    return 0;
}

int pthread_attr_getschedparam (const pthread_attr_t *attr, struct sched_param *spar)
{
    if (attr == NULL || spar == NULL)
	return EINVAL;

    spar->sched_priority = attr->schedparam.sched_priority;
    return 0;
}
#endif

int pthread_attr_getdetachstate (const pthread_attr_t *attr, int *dstate)
{
    if (attr == NULL || ((attr->detachstate != PTHREAD_CREATE_DETACHED) && (attr->detachstate != PTHREAD_CREATE_JOINABLE)))
	return EINVAL;

    *dstate = attr->detachstate;
    return 0;
}

int pthread_attr_setdetachstate (pthread_attr_t *attr, int dstate)
{
    if (attr == NULL || ((dstate != PTHREAD_CREATE_DETACHED) && (dstate != PTHREAD_CREATE_JOINABLE))) 
	return EINVAL;

    attr->detachstate = dstate;

    return 0;
}


int pthread_attr_getstack (const pthread_attr_t *attr, void **stackaddr, size_t *stacksize)
{
    if (attr == NULL)
	return EINVAL;

    *stackaddr = attr->stackaddr;
    *stacksize = attr->stacksize;

    return 0;
}

int pthread_attr_setstack (pthread_attr_t *attr, void *stackaddr, size_t stacksize)
{
#ifdef MB_XILKERNEL
#define SALIGN  0x3
#else
#define SALIGN  0x7
#endif

    if (attr == NULL) 
	return EINVAL;

    if ((unsigned int)stackaddr & SALIGN)
        return EINVAL;

    attr->stackaddr = stackaddr;
    attr->stacksize = stacksize;

    return 0;
} 

#if SCHED_TYPE == SCHED_PRIO
int pthread_getschedparam (pthread_t thread, int *policy, struct sched_param *param)
{
    return (int) make_syscall ((void*)thread, (void*)policy, (void*)param, NULL, NULL, SC_PTHREAD_GETSCHEDPARAM);
}

int pthread_setschedparam (pthread_t thread, int policy, struct sched_param *param)
{
    return (int) make_syscall ((void*)thread, (void*)policy, (void*)param, NULL, NULL, SC_PTHREAD_SETSCHEDPARAM);    
}
#endif

#ifdef CONFIG_PTHREAD_MUTEX
int pthread_mutex_init (pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
    return (int) make_syscall ((void*)mutex, (void*)attr, NULL, NULL, NULL, SC_PTHREAD_MUTEX_INIT);
}

int pthread_mutex_destroy (pthread_mutex_t *mutex)
{
    return (int) make_syscall ((void*)mutex, NULL, NULL, NULL, NULL, SC_PTHREAD_MUTEX_DESTROY);
}

int pthread_mutex_lock (pthread_mutex_t *mutex)
{
    return (int) make_syscall ((void*)mutex, NULL, NULL, NULL, NULL, SC_PTHREAD_MUTEX_LOCK);
}

int pthread_mutex_trylock (pthread_mutex_t *mutex)
{
    return (int) make_syscall ((void*)mutex, NULL, NULL, NULL, NULL, SC_PTHREAD_MUTEX_TRYLOCK);
}

int  pthread_mutex_unlock (pthread_mutex_t *mutex)
{
    return (int) make_syscall ((void*)mutex, NULL, NULL, NULL, NULL, SC_PTHREAD_MUTEX_UNLOCK);
}

int  pthread_mutexattr_init (pthread_mutexattr_t *attr)
{
    // Verify parameter
    if (attr == NULL)
	return EINVAL;
    attr->type = PTHREAD_MUTEX_DEFAULT;
    return 0;
}

int  pthread_mutexattr_destroy (pthread_mutexattr_t *attr)
{
    // Verify parameter
    if (attr == NULL)
	return EINVAL;
    
    attr->type = PTHREAD_INVALID;
    return 0;
}

int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type)
{
    if (attr == NULL)
        return EINVAL;

    *type = attr->type;
    return 0;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    if (attr == NULL)
        return EINVAL;

    if (type != PTHREAD_MUTEX_DEFAULT && type != PTHREAD_MUTEX_RECURSIVE)
        return EINVAL;

    attr->type = type;
    return 0;
}
#endif /* CONFIG_PTHREAD_MUTEX */
#endif /* CONFIG_PTHREAD_SUPPORT */
