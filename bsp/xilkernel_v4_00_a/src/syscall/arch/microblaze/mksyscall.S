/*///////////////////////////////////////////////////////////////////////////
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
// $Id: mksyscall.S,v 1.1.4.1 2007/08/20 06:08:23 tshui Exp $
//
//////////////////////////////////////////////////////////////////////////////*/

/*----------------------------------------------------------------------------------------//
//! @file
//! mksyscall.S - System call stub	
-----------------------------------------------------------------------------------------//*/
	
#include <os_config.h>
#include <sys/arch.h>

	
/*  make_syscall (param1, param2, param3, param4, param5, syscall_num);  
 *		   r5,     r6,     r7,     r8,     r9,     r10
 *               
 */
		
	.global make_syscall
	.section .text
	.align 2	
	.ent make_syscall
make_syscall:
	addi	r1, r1, -4;				
	sw	r15, r0, r1;			/* Save link register */
	bralid	r15, 8;				/* Make system call. Parameters in registers. System call number in r10 */
	nop; 
	lw	r15, r0, r1;			/* Restore link register */
	rtsd	r15, 8;
	addik	r1, r1, 4;
	.end make_syscall
