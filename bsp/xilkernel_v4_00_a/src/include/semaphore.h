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
// $Id: semaphore.h,v 1.1.4.1 2007/08/20 06:08:21 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file semaphore.h
//! XSI semaphore definitions and declarations
//----------------------------------------------------------------------------------------------------//
#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include <sys/fcntl.h>
#include <os_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SEM_NAME_MAX    25      // Length of symbolic name that can be associated with a semaphore

#define SEM_FAILED      -1      // Error indication

#ifndef SEM_NSEMS_MAX
#define SEM_NSEMS_MAX   10
#endif
 
#ifndef SEM_VALUE_MAX 
#define SEM_VALUE_MAX  200
#endif

typedef unsigned int sem_t;

int    sem_init(sem_t* sem, int pshared, unsigned value);
int    sem_wait(sem_t* sem);
/*int    sem_timedwait (sem_t *sem, const struct timespec *abs_timeout);*/
int    sem_timedwait (sem_t *sem, unsigned int ticks);
int    sem_trywait(sem_t* sem);
int    sem_getvalue(sem_t* sem, int* sval);
int    sem_post(sem_t* sem);
sem_t* sem_open(const char* name, int oflag, ...);
int    sem_unlink(const char* name);
int    sem_close(sem_t* sem);
int    sem_destroy(sem_t* sem);

#ifdef __cplusplus
}       
#endif 

#endif /* _SEMAPHORE_H */
