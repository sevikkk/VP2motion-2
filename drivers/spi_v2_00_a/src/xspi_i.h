/* $Id: xspi_i.h,v 1.1.2.2 2008/07/30 15:48:43 sadanan Exp $ */
/******************************************************************************
*
*       XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*       AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*       SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*       OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*       APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*       THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*       AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*       FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*       WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*       IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*       REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*       INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*       FOR A PARTICULAR PURPOSE.
*
*       (c) Copyright 2001-2008 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xspi_i.h
*
* This header file contains internal identifiers. It is intended for internal
* use only.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rpm  10/11/01 First release
* 1.00b jhl  03/14/02 Repartitioned driver for smaller files.
* 1.00b rpm  04/24/02 Moved register definitions to xspi_l.h
* 1.11a wgr  03/22/07 Converted to new coding style.
* 1.12a sv   03/28/08 Removed the Macro for statistics, moved the interrupt
*                     register definitions and bit definitions to _l.h.
* 2.00a sv   07/30/08 Removed the Macro for statistics, moved the interrupt
*                     register definitions and bit definitions to _l.h.
* </pre>
*
******************************************************************************/

#ifndef XSPI_I_H		/* prevent circular inclusions */
#define XSPI_I_H		/* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#include "xbasic_types.h"
#include "xspi_l.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void XSpi_Abort(XSpi * InstancePtr);

/************************** Variable Definitions *****************************/

extern XSpi_Config XSpi_ConfigTable[];

#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */
