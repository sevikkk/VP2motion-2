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
// $Id: libmsgq.c,v 1.1.4.1 2007/08/20 06:08:23 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file libmsgq.c
//! This contains system call wrapper functions for Message Queue.
//----------------------------------------------------------------------------------------------------//
#include <os_config.h>
#include <sys/syscall.h>
#include <sys/msg.h>
#include <sys/kmsg.h>

extern void* make_syscall (void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, int syscall_num);

#ifdef CONFIG_MSGQ
int msgget(key_t key, int msgflg)
{
    return (int) make_syscall ((void*)key, (void*)msgflg, NULL, NULL, NULL, SC_MSGGET);
}

int msgctl(int msqid, int cmd, struct msqid_ds *buf)
{
    return (int) make_syscall ((void*)msqid, (void*)cmd, (void*)buf, NULL, NULL, SC_MSGCTL);
}

int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg)
{
    return (int) make_syscall ((void*)msqid, (void*)msgp, (void*)msgsz, (void*)msgflg, NULL, SC_MSGSND);
}

ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg)
{
    return (ssize_t) make_syscall ((void*)msqid, (void*)msgp, (void*)msgsz, (void*)msgtyp, (void*)msgflg, SC_MSGRCV);
}
#endif /* CONFIG_MSGQ */
