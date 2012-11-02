/* $Id: xspi_intr_example.c,v 1.1.2.1 2008/07/28 09:49:50 svemula Exp $ */
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
*       (c) Copyright 2008  Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
* @file xspi_intr_example.c
*
*
* This file contains a design example using the Spi driver (XSpi) and the Spi
* device using the interrupt mode.
*
* This example works with a PPC/MicroBlaze processor.
*
* @note
*
* None.
*
*<pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- ---------------------------------------------------------
* 1.01a sv   05/29/06 First release for Test App Integration
*                     for Interrupt examples
* 1.11a sdm  03/03/08 Minor changes to comply to Doxygen and coding guidelines
*</pre>
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"	/* XPAR parameters */
#include "xspi.h"		/* SPI device driver */
#include "xintc.h"		/* Interrupt controller device driver */

#ifdef __MICROBLAZE__
#include "mb_interface.h"
#else
#include "xexception_l.h"
#endif

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#ifndef TESTAPP_GEN
#define SPI_DEVICE_ID		XPAR_SPI_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_INTC_0_DEVICE_ID
#define SPI_IRPT_INTR		XPAR_INTC_0_SPI_0_VEC_ID
#endif

/*
 *  This is the size of the buffer to be transmitted/received in this example.
 */
#define BUFFER_SIZE			 12


/**************************** Type Definitions *******************************/

/*
 * The following data type is used to send and receive data on the SPI
 * interface.
 */
typedef u8 DataBuffer[BUFFER_SIZE];


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

int SpiIntrExample(XIntc *IntcInstancePtr, XSpi *SpiInstancePtr,
					u16 SpiDeviceId, u16
					SpiIntrId);

void SpiIntrHandler(void *CallBackRef, u32 StatusEvent, u32 ByteCount);

static int SpiSetupIntrSystem(XIntc *IntcInstancePtr, XSpi *SpiInstancePtr,
					u16 SpiIntrId);

static void SpiDisableIntrSystem(XIntc *IntcInstancePtr, u16 SpiIntrId);

/************************** Variable Definitions *****************************/

/*
 * The instances to support the device drivers are global such that the
 * are initialized to zero each time the program runs.
 */
#ifndef TESTAPP_GEN
static XIntc IntcInstance;	 /* The instance of the Interrupt Controller */
static XSpi  SpiInstance;	 /* The instance of the SPI device */
#endif

/*
 * The following variables are shared between non-interrupt processing and
 * interrupt processing such that they must be global.
 */
volatile int TransferInProgress;

/*
 * The following variable tracks any errors that occur during interrupt
 * processing
 */
int Error;

/*
 * The following variables are used to read and write to the  Spi device, they
 * are global to avoid having large buffers on the stack.
 */
u8 ReadBuffer[BUFFER_SIZE];
u8 WriteBuffer[BUFFER_SIZE];


/*****************************************************************************/
/**
*
* Main function to call the Spi interrupt example.
*
* @param	None
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None
*
******************************************************************************/
#ifndef TESTAPP_GEN
int main(void)
{
	int Status;

	/*
	 * Run the Spi Interrupt example.
	 */
	Status = SpiIntrExample(&IntcInstance,
				&SpiInstance,
				SPI_DEVICE_ID,
				SPI_IRPT_INTR);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}
#endif

/*****************************************************************************/
/**
*
* This function does a minimal test on the Spi device and driver as a
* design example. The purpose of this function is to illustrate how to use
* the XSpi component using the interrupt mode.
*
* This function sends data and expects to receive the same data.
*
*
* @param	IntcInstancePtr is a pointer to the instance of the INTC
* 		component.
* @param	SpiInstancePtr is a pointer to the instance of Spi component.
* @param	SpiDeviceId is the Device ID of the Spi Device and is the
*		XPAR_<SPI_instance>_DEVICE_ID value from xparameters.h.
* @param	SpiIntrId is the interrupt Id and is typically
*		XPAR_<INTC_instance>_<SPI_instance>_VEC_ID value from
*		xparameters.h .
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note
*
* This function contains an infinite loop such that if interrupts are not
* working it may never return.
*
******************************************************************************/
int SpiIntrExample(XIntc *IntcInstancePtr, XSpi *SpiInstancePtr,
			u16 SpiDeviceId, u16 SpiIntrId)
{
	int Status;
	u32 Count;
	u8 Test;

	/*
	 * Initialize the SPI driver so that it's ready to use,
	 * specify the device ID that is generated in xparameters.h
	 */
	Status = XSpi_Initialize(SpiInstancePtr, SpiDeviceId);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XSpi_SelfTest(SpiInstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the Spi device to the interrupt subsystem such that
	 * interrupts can occur. This function is application specific.
	 */
	Status = SpiSetupIntrSystem(IntcInstancePtr,
					SpiInstancePtr,
				 	SpiIntrId);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Setup the handler for the SPI that will be called from the interrupt
	 * context when an SPI status occurs, specify a pointer to the SPI
	 * driver instance as the callback reference so the handler is able to
	 * access the instance data.
	 */
	XSpi_SetStatusHandler(SpiInstancePtr, SpiInstancePtr,
		 		(XSpi_StatusHandler) SpiIntrHandler);

	/*
	 * Set the Spi device as a master and in loopback mode.
	 */
	Status = XSpi_SetOptions(SpiInstancePtr, XSP_MASTER_OPTION |
 					XSP_LOOPBACK_OPTION);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	/*
	 * Start the SPI driver so that interrupts and the device are enabled.
	 */
	XSpi_Start(SpiInstancePtr);

	/*
	 * Initialize the write buffer with pattern to write, initialize the
	 * read buffer to zero so it can be verified after the read, the
	 * Test value that is added to the unique value allows the value to be
	 * changed in a debug environment.
	 */
	Test = 0x10;
	for (Count = 0; Count < BUFFER_SIZE; Count++) {
		WriteBuffer[Count] = (u8)(Count + Test);
		ReadBuffer[Count] = 0;
	}


	/*
	 * Transmit the data.
	 */
	TransferInProgress = TRUE;
	XSpi_Transfer(SpiInstancePtr, WriteBuffer, ReadBuffer, BUFFER_SIZE);

	/*
	 * Wait for the transmission to be complete.
	 */
	while (TransferInProgress);


	/*
	 * Disable the Spi interrupt.
	 */
	SpiDisableIntrSystem(IntcInstancePtr, SpiIntrId);

	/*
	 * Compare the data received with the data that was transmitted.
	 */
	for (Count = 0; Count < BUFFER_SIZE; Count++) {
		if (WriteBuffer[Count] != ReadBuffer[Count]) {
			return XST_FAILURE;
		}
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function is the handler which performs processing for the SPI driver.
* It is called from an interrupt context such that the amount of processing
* performed should be minimized. It is called when a transfer of SPI data
* completes or an error occurs.
*
* This handler provides an example of how to handle SPI interrupts and
* is application specific.
*
* @param	CallBackRef is the upper layer callback reference passed back
*		when the callback function is invoked.
* @param	StatusEvent is the event that just occurred.
* @param	ByteCount is the number of bytes transferred up until the event
*		occurred.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void SpiIntrHandler(void *CallBackRef, u32 StatusEvent, u32 ByteCount)
{
	/*
	 * Indicate the transfer on the SPI bus is no longer in progress
	 * regardless of the status event.
	 */
	TransferInProgress = XFALSE;

	/*
	 * If the event was not transfer done, then track it as an error.
	 */
	if (StatusEvent != XST_SPI_TRANSFER_DONE) {
		Error++;
	}
}


/*****************************************************************************/
/**
*
* This function setups the interrupt system such that interrupts can occur
* for the Spi device. This function is application specific since the actual
* system may or may not have an interrupt controller. The Spi device could be
* directly connected to a processor without an interrupt controller.  The
* user should modify this function to fit the application.
*
* @param	IntcInstancePtr is a pointer to the instance of the Intc device.
* @param	SpiInstancePtr is a pointer to the instance of the Spi device.
* @param	SpiIntrId is the interrupt Id and is typically
*		XPAR_<INTC_instance>_<SPI_instance>_VEC_ID value from
*		xparameters.h
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None
*
******************************************************************************/
static int SpiSetupIntrSystem(XIntc *IntcInstancePtr, XSpi *SpiInstancePtr,
					 u16 SpiIntrId)
{
	int Status;

#ifndef TESTAPP_GEN
	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	Status = XIntc_Initialize(IntcInstancePtr, INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif

	/*
	 * Connect a device driver handler that will be called when an interrupt
	 * for the device occurs, the device driver handler performs the
	 * specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(IntcInstancePtr, SpiIntrId,
	 			(XInterruptHandler) XSpi_InterruptHandler,
				(void *)SpiInstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

#ifndef TESTAPP_GEN
	/*
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts, specific real mode so that
	 * the SPI can cause interrupts through the interrupt controller.
	 */
	Status = XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
 	   return XST_FAILURE;
	}
#endif

	/*
	 * Enable the interrupt for the SPI device.
	 */
	XIntc_Enable(IntcInstancePtr, SpiIntrId);

#ifndef TESTAPP_GEN

#ifdef __MICROBLAZE__
		/*
		 * Enable the microblaze Interrupts.
		 */
		microblaze_enable_interrupts();
#endif /* __MICROBLAZE__ */

#ifdef __PPC__
	/*
 	* Initialize the PPC exception table.
	 */
	XExc_Init();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	XExc_RegisterHandler(XEXC_ID_NON_CRITICAL_INT,
			 	(XExceptionHandler)
			 	XIntc_InterruptHandler,
			 	IntcInstancePtr);

	/*
	 * Enable non-critical exceptions.
	 */
	XExc_mEnableExceptions(XEXC_NON_CRITICAL);

#endif /* _PPC_ */

#endif /* TESTAPP_GEN */

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function disables the interrupts that occur for the Spi device.
*
* @param	IntcInstancePtr is the pointer to the instance of the INTC
*		component.
* @param	SpiIntrId is the interrupt Id and is typically
*		XPAR_<INTC_instance>_<SPI_instance>_VEC_ID value from
*		xparameters.h
*
* @return	None
*
* @note		None
*
******************************************************************************/
static void SpiDisableIntrSystem(XIntc *IntcInstancePtr, u16 SpiIntrId)
{

	/*
	 * Disconnect and disable the interrupt for the Spi device.
	 */
	XIntc_Disconnect(IntcInstancePtr, SpiIntrId);
}




