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
// $Id: mksyscall440.S,v 1.1.2.1 2007/11/06 20:02:47 vasanth Exp $
//
//////////////////////////////////////////////////////////////////////////////*/

/*----------------------------------------------------------------------------------------//
//! @file
//! mksyscall.S - System call stub	
-----------------------------------------------------------------------------------------//*/
	
#include <os_config.h>
#include <sys/arch.h>

	
/*  make_syscall (param1, param2, param3, param4, param5, syscall_num);  
 *		   r3,     r4,     r5,     r6,     r7,     r8
 *      
 *  System call handler located at system call exception                 
 */
		
	.global make_syscall
	.section .text
	.align 2
        .type make_syscall@function                	
make_syscall:
        stwu    1, -8(1);                       /* Make space on the stack and save backchain */
        mflr    0; 
        stw     0, 12(1);                       /* Save link register */
        mfivpr  11;                             /* Get the exception vector base */
        mfivor8 12; 
        add     0, 11, 12; 
        mtlr    0; 
        blrl;
        lwz     0, 12(1);
        mtlr    0; 
	addi	1, 1, 8;
        blr;
        
