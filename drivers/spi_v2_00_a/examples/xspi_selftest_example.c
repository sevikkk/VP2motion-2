/* $Id: xspi_selftest_example.c,v 1.1.2.1 2008/07/28 09:49:50 svemula Exp $ */
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
*       (c) Copyright 2008 Xilinx Inc.
*       All rights reserved.
*
*******************************************************************************/
/******************************************************************************/
/**
* @file xspi_selftest_example.c
*
* This file contains a example for using the SPI Hardware and driver.
*
* @note
*
* None
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a sv   05/16/05 Initial release for TestApp integration.
* 1.11a sdn  03/03/08 Minor changes to comply to coding guidelines
* </pre>
*
*******************************************************************************/

/***************************** Include Files **********************************/

#include "xparameters.h"
#include "xspi.h"
#include "xspi_l.h"


/************************** Constant Definitions ******************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define SPI_DEVICE_ID       XPAR_SPI_0_DEVICE_ID

/**************************** Type Definitions ********************************/


/***************** Macros (Inline Functions) Definitions **********************/


/************************** Function Prototypes *******************************/

int SpiSelfTestExample(u16 DeviceId);

/************************** Variable Definitions ******************************/

XSpi Spi; /* The instance of the SPI device */


/******************************************************************************/
/**
* Main function to call the example. This function is not included if the
* example is generated from the TestAppGen test tool.
*
* @param	None
*
* @return	XST_SUCCESS if successful, XST_FAILURE if unsuccessful
*
* @note		None
*
*******************************************************************************/
#ifndef TESTAPP_GEN
int main(void)
{
	int Status;

	/*
	 * Call the example , specify the device ID that is generated in
	 * xparameters.h
	 */
	Status = SpiSelfTestExample(SPI_DEVICE_ID);
	if (Status != XST_SUCCESS) {
	return XST_FAILURE;
	}

	return XST_SUCCESS;
}
#endif


/*****************************************************************************/
/**
*
* This function does a selftest and loopback test on the SPI device and
* XSpi driver as an example. The purpose of this function is to illustrate
* how to use the XSpi component.
*
*
* @param	DeviceId is the XPAR_<SPI_instance>_DEVICE_ID value from
*		xparameters.h
*
* @return	XST_SUCCESS if successful, XST_FAILURE if unsuccessful
*
* @note		None
*
****************************************************************************/
int SpiSelfTestExample(u16 DeviceId)
{
	int Status;

	/*
	 * Initialize the SPI driver so that it is  ready to use.
	 */
	Status = XSpi_Initialize(&Spi, DeviceId);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly
	 */
	Status = XSpi_SelfTest(&Spi);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

