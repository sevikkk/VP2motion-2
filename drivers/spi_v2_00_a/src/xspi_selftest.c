/* $Id: xspi_selftest.c,v 1.1.2.2 2008/07/30 15:48:43 sadanan Exp $ */
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
* @file xspi_selftest.c
*
* This component contains the implementation of selftest functions for the
* XSpi driver component.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00b jhl  2/27/02  First release
* 1.00b rpm  04/25/02 Collapsed IPIF and reg base addresses into one
* 1.11a wgr  03/22/07 Converted to new coding style.
* 1.12a sv   03/17/08 Updated the code to support 16/32 bit transfer width.
* 2.00a sv   07/30/08 Updated the code to support 16/32 bit transfer width.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xspi.h"
#include "xspi_i.h"

/************************** Constant Definitions *****************************/

#define XSP_SR_RESET_STATE      0x5	   /* Default to Tx/Rx reg empty */
#define XSP_CR_RESET_STATE      0x180

#define XSP_HALF_WORD_TESTBYTE	0x2200	   /* Test Byte for Half Word */
#define XSP_WORD_TESTBYTE	0xAA005500 /* Test Byte for Word */

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

static int LoopbackTest(XSpi * InstancePtr);

/************************** Variable Definitions *****************************/


/*****************************************************************************/
/**
*
* Runs a self-test on the driver/device. The self-test is destructive in that
* a reset of the device is performed in order to check the reset values of
* the registers and to get the device into a known state. A simple loopback
* test is also performed to verify that transmit and receive are working
* properly. The device is changed to master mode for the loopback test, since
* only a master can initiate a data transfer.
*
* Upon successful return from the self-test, the device is reset.
*
* @param	InstancePtr is a pointer to the XSpi instance to be worked on.
*
* @return
* 		- XST_SUCCESS if successful
*		- XST_REGISTER_ERROR indicates a register did not read or write
*		correctly
* 		- XST_LOOPBACK_ERROR if a loopback error occurred.
*
* @note		None.
*
******************************************************************************/
int XSpi_SelfTest(XSpi * InstancePtr)
{
	int Result;
	u32 Register;

	XASSERT_NONVOID(InstancePtr != NULL);
	XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

	/*
	 * Reset the SPI device to leave it in a known good state
	 */
	XSpi_Reset(InstancePtr);

	/*
	 * All the SPI registers should be in their default state right now.
	 */
	Register = (u32) XSpi_mGetControlReg(InstancePtr);
	if (Register != XSP_CR_RESET_STATE) {
		return XST_REGISTER_ERROR;
	}

	Register = (u32) XSpi_mGetStatusReg(InstancePtr);
	if (Register != XSP_SR_RESET_STATE) {
		return XST_REGISTER_ERROR;
	}

	/*
	 * Each supported slave select bit should be set to 1.
	 */
	Register = XSpi_mGetSlaveSelectReg(InstancePtr);
	if (Register != InstancePtr->SlaveSelectMask) {
		return XST_REGISTER_ERROR;
	}

	/*
	 * If configured with FIFOs, the occupancy values should be 0.
	 */
	if (InstancePtr->HasFifos) {
		Register =
			(u32) XIo_In8(InstancePtr->BaseAddr +
			XSP_TFO_OFFSET);
		if (Register != 0) {
			return XST_REGISTER_ERROR;
		}
		Register =
			(u32) XIo_In8(InstancePtr->BaseAddr +
			XSP_RFO_OFFSET);
		if (Register != 0) {
			return XST_REGISTER_ERROR;
		}
	}

	/*
	 * Run an internal loopback test on the SPI.
	 */
	Result = LoopbackTest(InstancePtr);
	if (Result != XST_SUCCESS) {
		return Result;
	}

	/*
	 * Reset the SPI device to leave it in a known good state
	 */
	XSpi_Reset(InstancePtr);

	return XST_SUCCESS;
}

/*****************************************************************************/
/*
*
* Runs an internal loopback test on the SPI device. This is done as a master
* with a enough data to fill the FIFOs if FIFOs are present. If the device is
* configured as a slave-only, this function returns successfully even though
* no loopback test is performed.
*
* This function does not restore the device context after performing the test
* as it assumes the device will be reset after the call.
*
* @param	InstancePtr is a pointer to the XSpi instance to be worked on.
*
* @return
* 		- XST_SUCCESS if loopback was performed successfully or not
*		performed at all if device is slave-only.
*		- XST_LOOPBACK_ERROR if loopback failed.
*
* @note		None.
*
******************************************************************************/
static int LoopbackTest(XSpi * InstancePtr)
{
	u8 StatusReg;
	u16 ControlReg;
	u32 Index;
	u32 Data;
	u32 NumSent = 0;
	u32 NumRecvd = 0;

	/*
	 * Cannot run as a slave-only because we need to be master in order to
	 * initiate a transfer. Still return success, though.
	 */
	if (InstancePtr->SlaveOnly) {
		return XST_SUCCESS;
	}

	/*
	 * Setup the control register to enable master mode and the loopback so
	 * that data can be sent and received
	 */
	ControlReg = XSpi_mGetControlReg(InstancePtr);
	XSpi_mSetControlReg(InstancePtr, ControlReg |
				XSP_CR_LOOPBACK_MASK | XSP_CR_MASTER_MODE_MASK);
	/*
	 * We do not need interrupts for this loopback test
	 */
	XSpi_mIntrGlobalDisable(InstancePtr);

	/*
	 * Send data up to the maximum size of the transmit register, which is
	 * one byte without FIFOs.  We send data 4 times just to exercise the
	 * device through more than one iteration.
	 */
	for (Index = 0; Index < 4; Index++) {
		Data = Index;

		/*
		 * Fill the transmit register
		 */
		StatusReg = XSpi_mGetStatusReg(InstancePtr);
		while ((StatusReg & XSP_SR_TX_FULL_MASK) == 0) {
			if(InstancePtr->DataWidth == XSP_DATAWIDTH_BYTE) {
				/*
				 * Data Transfer Width is Byte (8 bit).
				 */
				XIo_Out8(InstancePtr->BaseAddr + XSP_DTR_OFFSET,
						(u8) Data);
				NumSent++;
			} else if(InstancePtr->DataWidth ==
						XSP_DATAWIDTH_HALF_WORD) {
				/*
				 * Data Transfer Width is Half Word (16 bit).
				 */
				XIo_Out16(InstancePtr->BaseAddr +
						XSP_DTR_OFFSET - 1,
					(u16)(Data + XSP_HALF_WORD_TESTBYTE));
				NumSent+=2;
			} else if(InstancePtr->DataWidth == XSP_DATAWIDTH_WORD){
				/*
				 * Data Transfer Width is Word (32 bit).
				 */
				XIo_Out32(InstancePtr->BaseAddr +
						XSP_DTR_OFFSET - 3,
						(u32)(Data +
						XSP_WORD_TESTBYTE));
				NumSent+=4;
			}

			StatusReg = XSpi_mGetStatusReg(InstancePtr);
		}

		/*
		 * Start the transfer by not inhibiting the transmitter and
		 * enabling the device
		 */
		ControlReg = XSpi_mGetControlReg(InstancePtr) &
					(~XSP_CR_TRANS_INHIBIT_MASK);
		XSpi_mSetControlReg(InstancePtr, ControlReg |
		XSP_CR_ENABLE_MASK);

		/*
		 * Wait for the transfer to be done by polling the transmit
		 * empty status bit
		 */
		do {
			StatusReg = XSpi_mGetStatusReg(InstancePtr);
		}
		while ((StatusReg & XSP_SR_TX_EMPTY_MASK) == 0);

		/*
		 * Receive and verify the data just transmitted
		 */
		while ((StatusReg & XSP_SR_RX_EMPTY_MASK) == 0) {
			if(InstancePtr->DataWidth == XSP_DATAWIDTH_BYTE) {
				u8 Compare;
				Compare = XIo_In8(InstancePtr->BaseAddr +
						XSP_DRR_OFFSET);
				NumRecvd++;
				if (Compare != Index) {
					return XST_LOOPBACK_ERROR;
				}
			} else if(InstancePtr->DataWidth ==
					XSP_DATAWIDTH_HALF_WORD) {
				u16 Compare;
				Compare = XIo_In16(InstancePtr->BaseAddr +
						XSP_DRR_OFFSET - 1);
				NumRecvd+=2;
				if (Compare != (u16)(Index +
						XSP_HALF_WORD_TESTBYTE)) {
					return XST_LOOPBACK_ERROR;
				}
			} else if(InstancePtr->DataWidth == XSP_DATAWIDTH_WORD)
			{
				u32 Compare;
				Compare = XIo_In32(InstancePtr->BaseAddr +
						XSP_DRR_OFFSET - 3);
				NumRecvd+=4;
				if (Compare != (u32)(Index + XSP_WORD_TESTBYTE))
				{
					return XST_LOOPBACK_ERROR;
				}
			}


			StatusReg = XSpi_mGetStatusReg(InstancePtr);
		}

		/*
		 * Stop the transfer (hold off automatic sending) by inhibiting
		 * the transmitter and disabling the device
		 */
		ControlReg |= XSP_CR_TRANS_INHIBIT_MASK;
		XSpi_mSetControlReg(InstancePtr ,
					ControlReg & ~ XSP_CR_ENABLE_MASK);
	}

	/*
	 * One final check to make sure the total number of bytes sent equals
	 * the total number of bytes received.
	 */
	if (NumSent != NumRecvd) {
		return XST_LOOPBACK_ERROR;
	}

	return XST_SUCCESS;
}

