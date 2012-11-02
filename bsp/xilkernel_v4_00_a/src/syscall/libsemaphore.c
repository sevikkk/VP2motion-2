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
// $Id: libsemaphore.c,v 1.1.4.1 2007/08/20 06:08:23 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file libsemaphore.c
//! This contains system call wrapper functions for POSIX Semaphore routines
//----------------------------------------------------------------------------------------------------//
#include <os_config.h>
#include <sys/syscall.h>
#include <semaphore.h>
#include <stdarg.h>
#include <sys/ksemaphore.h>

extern void* make_syscall (void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, int syscall_num);

#ifdef CONFIG_SEMA
int sem_init(sem_t* sem, int pshared, unsigned value)
{
    return (int) make_syscall ((void*)sem, (void*)pshared, (void*)value, NULL, NULL, SC_SEM_INIT);
}

int sem_trywait(sem_t* sem)
{
    return (int) make_syscall ((void*)sem, NULL, NULL, NULL, NULL, SC_SEM_TRYWAIT);
}

int sem_wait(sem_t* sem)
{
    return (int) make_syscall ((void*)sem, NULL, NULL, NULL, NULL, SC_SEM_WAIT);
}

#ifdef CONFIG_TIME
int sem_timedwait (sem_t* sem, unsigned int ticks)
{
    return (int) make_syscall ((void*)sem, (void*)ticks, NULL, NULL, NULL, SC_SEM_TIMED_WAIT);
}
#endif

int sem_getvalue(sem_t* sem, int* sval)
{
    return (int) make_syscall ((void*)sem, (void*)sval, NULL, NULL, NULL, SC_SEM_GETVALUE);
}

int sem_post(sem_t* sem)
{
    return (int) make_syscall ((void*)sem, NULL, NULL, NULL, NULL, SC_SEM_POST);
}

#ifdef CONFIG_NAMED_SEMA
sem_t* sem_open(const char* name, int oflag, ...)
{
    mode_t mode;
    unsigned value;
    va_list varptr;
    
    if( !(oflag & O_CREAT) )                         // Other flags unsupported
	return (sem_t*)SEM_FAILED;

    if (!(oflag & O_EXCL)) {

	va_start( varptr, oflag );
	
	mode = va_arg( varptr, mode_t );
	value = va_arg( varptr, unsigned );
	
	va_end (varptr);
    }

    return (sem_t*) make_syscall ((void*)name, (void*)oflag, (void*)mode, (void*)value, NULL, SC_SEM_OPEN);
}

int sem_unlink(const char* name)
{
    return (int) make_syscall ((void*)name, NULL, NULL, NULL, NULL, SC_SEM_UNLINK);
}

int sem_close(sem_t* sem)
{
    return (int) make_syscall ((void*)sem, NULL, NULL, NULL, NULL, SC_SEM_CLOSE);
}
#endif

int sem_destroy(sem_t* sem)
{
    return (int) make_syscall ((void*)sem, NULL, NULL, NULL, NULL, SC_SEM_DESTROY);
}
#endif /* CONFIG_SEMA */
