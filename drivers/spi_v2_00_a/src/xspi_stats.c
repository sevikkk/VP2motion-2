/* $Id: xspi_stats.c,v 1.1.2.2 2008/07/30 15:48:44 sadanan Exp $ */
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
*       (c) Copyright 2002-2008 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xspi_stats.c
*
* This component contains the implementation of statistics functions for the
* XSpi driver component.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00b jhl  03/14/02 First release
* 1.00b rpm  04/25/02 Changed macro naming convention
* 1.11a wgr  03/22/07 Converted to new coding style.
* 1.12a sv   03/28/08 Removed the call to the Macro for clearing statistics.
* 1.12a sv   07/30/08 Removed the call to the Macro for clearing statistics.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xspi.h"
#include "xspi_i.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/


/*****************************************************************************/
/**
*
* Gets a copy of the statistics for an SPI device.
*
* @param	InstancePtr is a pointer to the XSpi instance to be worked on.
* @param	StatsPtr is a pointer to a XSpi_Stats structure which will get a
*		copy of current statistics.
*
* @return	None.
*
* @note		Statistics are not updated in polled mode of operation.
*
******************************************************************************/
void XSpi_GetStats(XSpi * InstancePtr, XSpi_Stats * StatsPtr)
{
	XASSERT_VOID(InstancePtr != NULL);
	XASSERT_VOID(StatsPtr != NULL);
	XASSERT_VOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	StatsPtr->ModeFaults = InstancePtr->Stats.ModeFaults;
	StatsPtr->XmitUnderruns = InstancePtr->Stats.XmitUnderruns;
	StatsPtr->RecvOverruns = InstancePtr->Stats.RecvOverruns;
	StatsPtr->SlaveModeFaults = InstancePtr->Stats.SlaveModeFaults;
	StatsPtr->BytesTransferred = InstancePtr->Stats.BytesTransferred;
	StatsPtr->NumInterrupts = InstancePtr->Stats.NumInterrupts;
}

/*****************************************************************************/
/**
*
* Clears the statistics for the SPI device.
*
* @param	InstancePtr is a pointer to the XSpi instance to be worked on.
*
* @return	None.
*
* @note		Statistics are not updated in polled mode of operation.
*
******************************************************************************/
void XSpi_ClearStats(XSpi * InstancePtr)
{
	XASSERT_VOID(InstancePtr != NULL);
	XASSERT_VOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	InstancePtr->Stats.ModeFaults = 0;
	InstancePtr->Stats.XmitUnderruns = 0;
	InstancePtr->Stats.RecvOverruns = 0;
	InstancePtr->Stats.SlaveModeFaults = 0;
	InstancePtr->Stats.BytesTransferred = 0;
	InstancePtr->Stats.NumInterrupts = 0;

}
