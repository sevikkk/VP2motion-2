//////////////////////////////////////////////////////////////////////////////
// Filename:          /home/seva/src/xc2p_pe1/drivers/motion_v1_00_a/src/motion.c
// Version:           1.00.a
// Description:       motion Driver Source File
// Date:              Tue Oct  9 01:16:29 2012 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////


/***************************** Include Files *******************************/

#include "motion.h"

/************************** Function Definitions ***************************/

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
void MOTION_EnableInterrupt(void * baseaddr_p)
{
  Xuint32 baseaddr;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Enable all interrupt source from user logic.
   */
  MOTION_mWriteReg(baseaddr, MOTION_INTR_IPIER_OFFSET, 0x0000000F);

  /*
   * Enable all possible interrupt sources from device.
   */
  MOTION_mWriteReg(baseaddr, MOTION_INTR_DIER_OFFSET,
    INTR_TERR_MASK
    | INTR_DPTO_MASK
    | INTR_IPIR_MASK
    );

  /*
   * Set global interrupt enable.
   */
  MOTION_mWriteReg(baseaddr, MOTION_INTR_DGIER_OFFSET, INTR_GIE_MASK);
}

/**
 *
 * Example interrupt controller handler for MOTION device.
 * This is to show example of how to toggle write back ISR to clear interrupts.
 *
 * @param   baseaddr_p is the base address of the MOTION device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void MOTION_Intr_DefaultHandler(void * baseaddr_p)
{
  Xuint32 baseaddr;
  Xuint32 IntrStatus;
Xuint32 IpStatus;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Get status from Device Interrupt Status Register.
   */
  IntrStatus = MOTION_mReadReg(baseaddr, MOTION_INTR_DISR_OFFSET);

  xil_printf("Device Interrupt! DISR value : 0x%08x \n\r", IntrStatus);

  /*
   * Verify the source of the interrupt is the user logic and clear the interrupt
   * source by toggle write baca to the IP ISR register.
   */
  if ( (IntrStatus & INTR_IPIR_MASK) == INTR_IPIR_MASK )
  {
    xil_printf("User logic interrupt! \n\r");
    IpStatus = MOTION_mReadReg(baseaddr, MOTION_INTR_IPISR_OFFSET);
    MOTION_mWriteReg(baseaddr, MOTION_INTR_IPISR_OFFSET, IpStatus);
  }

}

