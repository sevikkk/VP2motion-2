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
// $Id: arch.h,v 1.1.2.1.14.2 2008/08/06 20:39:06 vasanth Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file arch.h
//! PPC405 specific definitions, constants, declarations
//----------------------------------------------------------------------------------------------------//
#ifndef _PPC_ARCH_H
#define _PPC_ARCH_H

#define STACK_ALIGN                8

#ifndef __ASM__
#define SSTACK_PTR_ADJUST ((int)-8)

//! Process Management Data Structures
//! Context data structure

//////////////////////////////////////////////////////////////////////////////////////
//
// Context map:
//
//    +------------+  + 0
//    |     MSR    | 
//    +------------+  + 4
//    |     PC     | 
//    +------------+  + 8
//    |     LR     | 
//    +------------+  + 12
//    |     CTR    | 
//    +------------+  + 16
//    |     XER    | 
//    +------------+  + 20
//    |     CR     | 
//    +------------+  + 24
//    |     r0     | 
//    |      .     |
//    |      .     |
//    |      .     |
//    |     r31    | 
//    +------------+  + 152
//
//////////////////////////////////////////////////////////////////////////////////////
struct _process_context { 
    unsigned int regs[38]; 	// Process context information store
    char isrflag;
} __attribute__ ((packed, aligned(STACK_ALIGN)));

#define PCONTEXT_SIZ  (sizeof(struct _process_context))

#else

#define SSTACK_PTR_ADJUST      (-8)

#endif

#define CTX_INDEX_MSR           0
#define CTX_INDEX_PC            1
#define CTX_INDEX_LR            2
#define CTX_INDEX_CTR           3
#define CTX_INDEX_XER           4
#define CTX_INDEX_CR            5
#define CTX_INDEX_GPR(r)        (6 + r)

// Various offsets within the context structure
#define CTX_OFFSET              (0)                        // Offset of context structure within process structure
#define CTX_MSR_FIELD           (0)
#define CTX_PC_FIELD            (CTX_MSR_FIELD + 4)
#define CTX_LR_FIELD            (CTX_PC_FIELD  + 4)
#define CTX_CTR_FIELD           (CTX_LR_FIELD  + 4)
#define CTX_XER_FIELD           (CTX_CTR_FIELD + 4)
#define CTX_CR_FIELD            (CTX_XER_FIELD + 4)
#define CTX_GPR_FIELD           (CTX_CR_FIELD  + 4)
#define CTX_GPR_REG_FIELD(reg)  (CTX_GPR_FIELD + (reg * 4))
#define CTX_SIZE                (38 * 4)
#define ISRFLAG_OFFSET          (CTX_OFFSET + CTX_SIZE)

#define ISRFLAG_SYSTEM_CALL     0
#define ISRFLAG_NON_CRITICAL    1
#define ISRFLAG_CRITICAL        2
#define ISRFLAG_NEW_PROC        3

// Various stack sizes used in the kernel
#define PROCESS_STARTUP_STACKSZ    400

#endif /* _PPC_ARCH_H */
