/* $Id: xspi_l.h,v 1.1.2.2 2008/07/30 15:48:43 sadanan Exp $ */
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
* @file xspi_l.h
*
* This header file contains identifiers, Register Definitions and  basic driver
* functions (or macros) that can be used to access the device.
* Refer xspi.h for information about the driver.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00b rpm  04/24/02 First release
* 1.11a wgr  03/22/07 Converted to new coding style.
* 1.11a sv   02/22/08 Added the definition of LSB-MSB first option.
* 1.12a sv   03/28/08 Removed macros in _l.h file, moved the
*                     interrupt register definitions from _i.h to _l.h.
* 2.00a sv   07/30/08 Removed macros in _l.h file, moved the
*                     interrupt register definitions from _i.h to _l.h.
* </pre>
*
******************************************************************************/

#ifndef XSPI_L_H		/* prevent circular inclusions */
#define XSPI_L_H		/* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#include "xbasic_types.h"
#include "xio.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Constant Definitions *****************************/

/**
 * XSPI register offsets
 */
/** @name Register Map
 *
 * Register offsets for the XSpi device.
 * @{
 */
/**
 * Register offsets for the SPI. Each register except the CR & SSR is 8 bits,
 * so add 3 to the word-offset to get the LSB (in a big-endian system).
 */
#define XSP_DGIER_OFFSET	 0x1C 		/**< Global Intr Enable Reg */
#define XSP_IISR_OFFSET		 0x20 		/**< Interrupt status Reg */
#define XSP_IIER_OFFSET		 0x28 		/**< Interrupt Enable Reg */
#define XSP_SRR_OFFSET	 	 0x40 		/**< Software Reset register */
#define XSP_CR_OFFSET		(0x60 + 0x2)	/**< 16-bit Control */
#define XSP_SR_OFFSET		(0x64 + 3)	/**< 8 Bit Status Register */
#define XSP_DTR_OFFSET		(0x68 + 3)	/**< Data transmit */
#define XSP_DRR_OFFSET		(0x6C + 3)	/**< Data receive */
#define XSP_SSR_OFFSET		 0x70		/**< 32-bit slave select */
#define XSP_TFO_OFFSET		(0x74 + 3)	/**< 8 bit Tx FIFO occupancy */
#define XSP_RFO_OFFSET		(0x78 + 3)	/**< 8 bit Rx FIFO occupancy */

/* @} */


/**
 * @name Global Interrupt Enable Register (GIER) mask(s)
 * @{
 */
#define XSP_GINTR_ENABLE_MASK	0x80000000	/**< Global interrupt enable */

/* @} */


/** @name SPI Device Interrupt Status/Enable Registers
 *
 * <b> Interrupt Status Register (IPISR) </b>
 *
 * This register holds the interrupt status flags for the Spi device.
 *
 * <b> Interrupt Enable Register (IPIER) </b>
 *
 * This register is used to enable interrupt sources for the Spi device.
 * Writing a '1' to a bit in this register enables the corresponding Interrupt.
 * Writing a '0' to a bit in this register disables the corresponding Interrupt.
 *
 * ISR/IER registers have the same bit definitions and are only defined once.
 * @{
 */
#define XSP_INTR_MODE_FAULT_MASK	0x1	/**< Mode fault error */
#define XSP_INTR_SLAVE_MODE_FAULT_MASK	0x2	/**< Selected as slave while
						 * disabled */
#define XSP_INTR_TX_EMPTY_MASK		0x4	/**< DTR/TxFIFO is empty */
#define XSP_INTR_TX_UNDERRUN_MASK	0x8	/**< DTR/TxFIFO was underrun */
#define XSP_INTR_RX_FULL_MASK		0x10	/**< DRR/RxFIFO is full */
#define XSP_INTR_RX_OVERRUN_MASK	0x20	/**< DRR/RxFIFO was overrun */
#define XSP_INTR_TX_HALF_EMPTY_MASK	0x40	/**< TxFIFO is half empty */

/**
 * Mask for all the interrupts in the IP Interrupt Registers.
 */
#define XSP_INTR_ALL 	(XSP_INTR_MODE_FAULT_MASK | \
			XSP_INTR_SLAVE_MODE_FAULT_MASK | \
			XSP_INTR_TX_EMPTY_MASK | XSP_INTR_TX_UNDERRUN_MASK | \
			XSP_INTR_RX_FULL_MASK | \
			XSP_INTR_TX_HALF_EMPTY_MASK | \
			XSP_INTR_RX_OVERRUN_MASK)


/**
 * The interrupts we want at startup. We add the TX_EMPTY interrupt in later
 * when we're getting ready to transfer data.  The others we don't care
 * about for now.
 */
#define XSP_INTR_DFT_MASK	(XSP_INTR_MODE_FAULT_MASK |	\
				XSP_INTR_TX_UNDERRUN_MASK |	\
				XSP_INTR_RX_OVERRUN_MASK |	\
				XSP_INTR_SLAVE_MODE_FAULT_MASK)
/* @} */

/**
 * SPI Software Reset Register (SRR) mask.
 */
#define XSP_SRR_RESET_MASK			0xA


/** @name SPI Control Register (CR) masks
 *
 * @{
 */
#define XSP_CR_LOOPBACK_MASK		0x1   /**< Local loopback mode */
#define XSP_CR_ENABLE_MASK		0x2   /**< System enable */
#define XSP_CR_MASTER_MODE_MASK		0x4   /**< Enable master mode */
#define XSP_CR_CLK_POLARITY_MASK	0x8   /**< Clock polarity high or low */
#define XSP_CR_CLK_PHASE_MASK		0x10  /**< Clock phase 0 or 1 */
#define XSP_CR_TXFIFO_RESET_MASK	0x20  /**< Reset transmit FIFO */
#define XSP_CR_RXFIFO_RESET_MASK	0x40  /**< Reset receive FIFO */
#define XSP_CR_MANUAL_SS_MASK		0x80  /**< Manual slave sel assert */
#define XSP_CR_TRANS_INHIBIT_MASK	0x100 /**< Master transaction inhibit */

/**
 * LSB/MSB first data format select. The default data format is MSB first.
 * The LSB first data format is not available in all versions of the Xilinx Spi
 * Device whereas the MSB first data format is supported by all the versions of
 * the Xilinx Spi Devices. Please check the HW specification to see if this
 * feature is supported or not.
 */
#define XSP_CR_LSB_MSB_FIRST_MASK  0x200

/* @} */


/** @name Status Register (SR) masks
 *
 * @{
 */
#define XSP_SR_RX_EMPTY_MASK		0x1  /**< Receive Reg/FIFO is empty */
#define XSP_SR_RX_FULL_MASK		0x2  /**< Receive Reg/FIFO is full */
#define XSP_SR_TX_EMPTY_MASK		0x4  /**< Transmit Reg/FIFO is empty */
#define XSP_SR_TX_FULL_MASK		0x8  /**< Transmit Reg/FIFO is full */
#define XSP_SR_MODE_FAULT_MASK		0x10 /**< Mode fault error */

/* @} */

/** @name SPI Transmit FIFO Occupancy (TFO) mask
 *
 * @{
 */
/* The binary value plus one yields the occupancy.*/
#define XSP_TFO_MASK		0x1F

/* @} */

/** @name SPI Receive FIFO Occupancy (RFO) mask
 *
 * @{
 */
/* The binary value plus one yields the occupancy.*/
#define XSP_RFO_MASK			0x1F
/* @} */

/** @name Data Width Definitions
 *
 * @{
 */
#define XSP_DATAWIDTH_BYTE	8   /**< Tx/Rx Reg is Byte Wide */
#define XSP_DATAWIDTH_HALF_WORD	16  /**< Tx/Rx Reg is Half Word (16 bit) Wide */
#define XSP_DATAWIDTH_WORD	32  /**< Tx/Rx Reg is Word (32 bit)  Wide */

/* @} */



/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */

