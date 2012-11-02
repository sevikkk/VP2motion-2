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
// $Id: arch.h,v 1.1.4.1.22.2 2008/08/06 20:39:06 vasanth Exp $
//
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------//
//! @file arch.h
//! Microblaze specific definitions, constants, declarations
//----------------------------------------------------------------------------------------------------//
#ifndef _MB_ARCH_H
#define _MB_ARCH_H

#define STACK_ALIGN             4

#ifndef __ASM__

#define SSTACK_PTR_ADJUST ((int)0)
//! Process Management Data Structures
//! Context data structure
struct _process_context {
    unsigned int regs[33];    
    char         isrflag;

} __attribute__ ((packed, aligned(STACK_ALIGN)));

#define PCONTEXT_SIZ  (sizeof(struct _process_context))

#else 

#define SSTACK_PTR_ADJUST      (0)

#endif  /* __ASM__ */

// Various offsets within the context structure
#define CTX_OFFSET              (0)                        // Offset of context structure within process structure
#define CTX_REG_OFFSET(regnum)  (CTX_OFFSET + 4*(regnum))  // Offset of a register in the context save structure
#define CTX_SIZE                (33 * 4)
#define ISRFLAG_OFFSET          (CTX_OFFSET + CTX_SIZE)
#define ISRFLAG_SYSTEM_CALL     0
#define ISRFLAG_INTERRUPT       1
#define ISRFLAG_NEW_PROC        2


#define PROCESS_STARTUP_STACKSZ         400

#endif /* _MB_ARCH_H */
