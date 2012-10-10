//////////////////////////////////////////////////////////////////////////////
// Filename:          /home/seva/src/xc2p_pe1/drivers/motion_v1_00_a/src/motion.h
// Version:           1.00.a
// Description:       motion Driver Header File
// Date:              Tue Oct  9 01:16:29 2012 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////

#ifndef MOTION_H
#define MOTION_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xio.h"

/************************** Constant Definitions ***************************/


/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 * -- SLV_REG2 : user logic slave module register 2
 * -- SLV_REG3 : user logic slave module register 3
 * -- SLV_REG4 : user logic slave module register 4
 * -- SLV_REG5 : user logic slave module register 5
 * -- SLV_REG6 : user logic slave module register 6
 * -- SLV_REG7 : user logic slave module register 7
 * -- SLV_REG8 : user logic slave module register 8
 * -- SLV_REG9 : user logic slave module register 9
 * -- SLV_REG10 : user logic slave module register 10
 * -- SLV_REG11 : user logic slave module register 11
 * -- SLV_REG12 : user logic slave module register 12
 * -- SLV_REG13 : user logic slave module register 13
 * -- SLV_REG14 : user logic slave module register 14
 * -- SLV_REG15 : user logic slave module register 15
 * -- SLV_REG16 : user logic slave module register 16
 * -- SLV_REG17 : user logic slave module register 17
 * -- SLV_REG18 : user logic slave module register 18
 * -- SLV_REG19 : user logic slave module register 19
 * -- SLV_REG20 : user logic slave module register 20
 * -- SLV_REG21 : user logic slave module register 21
 * -- SLV_REG22 : user logic slave module register 22
 * -- SLV_REG23 : user logic slave module register 23
 * -- SLV_REG24 : user logic slave module register 24
 * -- SLV_REG25 : user logic slave module register 25
 * -- SLV_REG26 : user logic slave module register 26
 * -- SLV_REG27 : user logic slave module register 27
 * -- SLV_REG28 : user logic slave module register 28
 * -- SLV_REG29 : user logic slave module register 29
 * -- SLV_REG30 : user logic slave module register 30
 * -- SLV_REG31 : user logic slave module register 31
 */
#define MOTION_USER_SLV_SPACE_OFFSET (0x00000000)
#define MOTION_SLV_REG0_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000000)
#define MOTION_SLV_REG1_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000004)
#define MOTION_SLV_REG2_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000008)
#define MOTION_SLV_REG3_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x0000000C)
#define MOTION_SLV_REG4_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000010)
#define MOTION_SLV_REG5_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000014)
#define MOTION_SLV_REG6_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000018)
#define MOTION_SLV_REG7_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x0000001C)
#define MOTION_SLV_REG8_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000020)
#define MOTION_SLV_REG9_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000024)
#define MOTION_SLV_REG10_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000028)
#define MOTION_SLV_REG11_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x0000002C)
#define MOTION_SLV_REG12_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000030)
#define MOTION_SLV_REG13_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000034)
#define MOTION_SLV_REG14_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000038)
#define MOTION_SLV_REG15_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x0000003C)
#define MOTION_SLV_REG16_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000040)
#define MOTION_SLV_REG17_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000044)
#define MOTION_SLV_REG18_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000048)
#define MOTION_SLV_REG19_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x0000004C)
#define MOTION_SLV_REG20_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000050)
#define MOTION_SLV_REG21_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000054)
#define MOTION_SLV_REG22_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000058)
#define MOTION_SLV_REG23_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x0000005C)
#define MOTION_SLV_REG24_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000060)
#define MOTION_SLV_REG25_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000064)
#define MOTION_SLV_REG26_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000068)
#define MOTION_SLV_REG27_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x0000006C)
#define MOTION_SLV_REG28_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000070)
#define MOTION_SLV_REG29_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000074)
#define MOTION_SLV_REG30_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x00000078)
#define MOTION_SLV_REG31_OFFSET (MOTION_USER_SLV_SPACE_OFFSET + 0x0000007C)

/**
 * Software Reset Space Register Offsets
 * -- RST : software reset register
 */
#define MOTION_SOFT_RST_SPACE_OFFSET (0x00000100)
#define MOTION_RST_REG_OFFSET (MOTION_SOFT_RST_SPACE_OFFSET + 0x00000000)

/**
 * Software Reset Masks
 * -- SOFT_RESET : software reset
 */
#define SOFT_RESET (0x0000000A)

/**
 * Interrupt Controller Space Offsets
 * -- INTR_DISR  : device (peripheral) interrupt status register
 * -- INTR_DIPR  : device (peripheral) interrupt pending register
 * -- INTR_DIER  : device (peripheral) interrupt enable register
 * -- INTR_DIIR  : device (peripheral) interrupt id (priority encoder) register
 * -- INTR_DGIER : device (peripheral) global interrupt enable register
 * -- INTR_ISR   : ip (user logic) interrupt status register
 * -- INTR_IER   : ip (user logic) interrupt enable register
 */
#define MOTION_INTR_CNTRL_SPACE_OFFSET (0x00000200)
#define MOTION_INTR_DISR_OFFSET (MOTION_INTR_CNTRL_SPACE_OFFSET + 0x00000000)
#define MOTION_INTR_DIPR_OFFSET (MOTION_INTR_CNTRL_SPACE_OFFSET + 0x00000004)
#define MOTION_INTR_DIER_OFFSET (MOTION_INTR_CNTRL_SPACE_OFFSET + 0x00000008)
#define MOTION_INTR_DIIR_OFFSET (MOTION_INTR_CNTRL_SPACE_OFFSET + 0x00000018)
#define MOTION_INTR_DGIER_OFFSET (MOTION_INTR_CNTRL_SPACE_OFFSET + 0x0000001C)
#define MOTION_INTR_IPISR_OFFSET (MOTION_INTR_CNTRL_SPACE_OFFSET + 0x00000020)
#define MOTION_INTR_IPIER_OFFSET (MOTION_INTR_CNTRL_SPACE_OFFSET + 0x00000028)

/**
 * Interrupt Controller Masks
 * -- INTR_TERR_MASK : transaction error
 * -- INTR_DPTO_MASK : data phase time-out
 * -- INTR_IPIR_MASK : ip interrupt requeset
 * -- INTR_RFDL_MASK : read packet fifo deadlock interrupt request
 * -- INTR_WFDL_MASK : write packet fifo deadlock interrupt request
 * -- INTR_IID_MASK  : interrupt id
 * -- INTR_GIE_MASK  : global interrupt enable
 * -- INTR_NOPEND    : the DIPR has no pending interrupts
 */
#define INTR_TERR_MASK (0x00000001UL)
#define INTR_DPTO_MASK (0x00000002UL)
#define INTR_IPIR_MASK (0x00000004UL)
#define INTR_RFDL_MASK (0x00000020UL)
#define INTR_WFDL_MASK (0x00000040UL)
#define INTR_IID_MASK (0x000000FFUL)
#define INTR_GIE_MASK (0x80000000UL)
#define INTR_NOPEND (0x80)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a MOTION register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the MOTION device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void MOTION_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define MOTION_mWriteReg(BaseAddress, RegOffset, Data) \
 	XIo_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a MOTION register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the MOTION device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 MOTION_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define MOTION_mReadReg(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from MOTION user logic slave registers.
 *
 * @param   BaseAddress is the base address of the MOTION device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void MOTION_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 MOTION_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define MOTION_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg1(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG1_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg2(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG2_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg3(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG3_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg4(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG4_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg5(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG5_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg6(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG6_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg7(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG7_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg8(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG8_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg9(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG9_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg10(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG10_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg11(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG11_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg12(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG12_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg13(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG13_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg14(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG14_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg15(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG15_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg16(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG16_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg17(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG17_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg18(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG18_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg19(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG19_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg20(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG20_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg21(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG21_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg22(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG22_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg23(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG23_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg24(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG24_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg25(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG25_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg26(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG26_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg27(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG27_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg28(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG28_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg29(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG29_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg30(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG30_OFFSET) + (RegOffset), (Xuint32)(Value))
#define MOTION_mWriteSlaveReg31(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (MOTION_SLV_REG31_OFFSET) + (RegOffset), (Xuint32)(Value))

#define MOTION_mReadSlaveReg0(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG0_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg1(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG1_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg2(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG2_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg3(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG3_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg4(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG4_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg5(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG5_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg6(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG6_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg7(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG7_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg8(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG8_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg9(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG9_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg10(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG10_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg11(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG11_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg12(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG12_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg13(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG13_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg14(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG14_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg15(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG15_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg16(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG16_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg17(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG17_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg18(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG18_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg19(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG19_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg20(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG20_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg21(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG21_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg22(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG22_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg23(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG23_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg24(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG24_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg25(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG25_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg26(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG26_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg27(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG27_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg28(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG28_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg29(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG29_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg30(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG30_OFFSET) + (RegOffset))
#define MOTION_mReadSlaveReg31(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (MOTION_SLV_REG31_OFFSET) + (RegOffset))

/**
 *
 * Write/Read 32 bit value to/from MOTION user logic memory (BRAM).
 *
 * @param   Address is the memory address of the MOTION device.
 * @param   Data is the value written to user logic memory.
 *
 * @return  The data from the user logic memory.
 *
 * @note
 * C-style signature:
 * 	void MOTION_mWriteMemory(Xuint32 Address, Xuint32 Data)
 * 	Xuint32 MOTION_mReadMemory(Xuint32 Address)
 *
 */
#define MOTION_mWriteMemory(Address, Data) \
 	XIo_Out32(Address, (Xuint32)(Data))
#define MOTION_mReadMemory(Address) \
 	XIo_In32(Address)

/**
 *
 * Reset MOTION via software.
 *
 * @param   BaseAddress is the base address of the MOTION device.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void MOTION_mReset(Xuint32 BaseAddress)
 *
 */
#define MOTION_mReset(BaseAddress) \
 	XIo_Out32((BaseAddress)+(MOTION_RST_REG_OFFSET), SOFT_RESET)

/************************** Function Prototypes ****************************/


/**
 *
 * Enable all possible interrupts from MOTION device.
 *
 * @param   baseaddr_p is the base address of the MOTION device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void MOTION_EnableInterrupt(void * baseaddr_p);

/**
 *
 * Example interrupt controller handler.
 *
 * @param   baseaddr_p is the base address of the MOTION device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void MOTION_Intr_DefaultHandler(void * baseaddr_p);

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the MOTION instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus MOTION_SelfTest(void * baseaddr_p);

#endif // MOTION_H
