/* $Id: xspi_g.c,v 1.1.2.2 2008/07/30 15:48:43 sadanan Exp $ */
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
* @file xspi_g.c
*
* This file contains a configuration table that specifies the configuration of
* SPI devices in the system.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rpm  10/11/01 First release
* 1.00b jhl  03/14/02 Repartitioned driver for smaller files.
* 1.00b rpm  04/24/02 Condensed config typedef - got rid of versions and
*                     multiple base addresses.
* 1.11a wgr  03/22/07 Converted to new coding style.
* 1.12a sv   03/17/08 Updated the code to support 16/32 bit transfer width.
* 2.00a sv   07/30/08 Updated the code to support 16/32 bit transfer width.
*
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "xspi.h"
#include "xparameters.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/************************** Variable Prototypes ******************************/

/**
 * This table contains configuration information for each SPI device
 * in the system.
 */
XSpi_Config XSpi_ConfigTable[XPAR_XSPI_NUM_INSTANCES] = {
	{
	 XPAR_SPI_0_DEVICE_ID,	/* Device ID for instance */
	 XPAR_SPI_0_BASEADDR,	/* Device base address */
	 XPAR_SPI_0_FIFO_EXIST,	/* Does device have FIFOs? */
	 XPAR_SPI_0_SLAVE_ONLY,	/* Is the device slave only? */
	 XPAR_SPI_0_NUM_SS_BITS,	/* Number of slave select bits */
 	 XPAR_SPI_0_NUM_TRANSFER_BITS	/* Transfer Data width */
	}
	,
	{
	 XPAR_SPI_1_DEVICE_ID,	/* Device ID for instance */
	 XPAR_SPI_1_BASEADDR,	/* Device base address */
	 XPAR_SPI_1_FIFO_EXIST,	/* Does device have FIFOs? */
	 XPAR_SPI_1_SLAVE_ONLY,	/* Is the device slave only? */
	 XPAR_SPI_1_NUM_SS_BITS,/* Number of slave select bits */
  	 XPAR_SPI_1_NUM_TRANSFER_BITS	/* Transfer Data width */
	 }
};
