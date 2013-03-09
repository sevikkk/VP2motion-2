#ifndef MOTION_H                /* prevent circular inclusions */
#define MOTION_H                /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <inttypes.h>

#include "xbasic_types.h"
#include "xstatus.h"
#include "xio.h"

#define MOTION_CMD_OFFSET                      0x00
#define MOTION_NEXT_XL_OFFSET                  0x04
#define MOTION_NEXT_XH_OFFSET                  0x08
#define MOTION_NEXT_V_OFFSET                   0x0C
#define MOTION_NEXT_A_OFFSET                   0x10
#define MOTION_NEXT_J_OFFSET                   0x14
#define MOTION_NEXT_DT_OFFSET                  0x18
#define MOTION_NEXT_STEPS_OFFSET               0x1C
#define MOTION_PRE_N_OFFSET                    0x20
#define MOTION_PULSE_N_OFFSET                  0x24
#define MOTION_POST_N_OFFSET                   0x28
#define MOTION_STEP_BIT_OFFSET                 0x2C
#define MOTION_STAT_XL_OFFSET                  0x30
#define MOTION_STAT_XH_OFFSET                  0x34
#define MOTION_STAT_V_OFFSET                   0x38
#define MOTION_STAT_A_OFFSET                   0x3C
#define MOTION_STAT_J_OFFSET                   0x40
#define MOTION_STAT_DT_OFFSET                  0x44
#define MOTION_STAT_STEPS_OFFSET               0x48
#define MOTION_STAT_STATUS_OFFSET              0x4C
#define MOTION_STAT_EMIN_XL_OFFSET             0x50
#define MOTION_STAT_EMIN_XH_OFFSET             0x54
#define MOTION_STAT_EMAX_XL_OFFSET             0x58
#define MOTION_STAT_EMAX_XH_OFFSET             0x5C
#define MOTION_STAT_EPROBE_XL_OFFSET           0x60
#define MOTION_STAT_EPROBE_XH_OFFSET           0x64
#define MOTION_STAT_MISSED_STEPS_OFFSET        0x68
#define MOTION_STOP_DELAY_OFFSET               0x6C
#define MOTION_PROBE_DELAY_OFFSET              0x70
#define MOTION_STAT_MIN_BOUNCE_OFFSET          0x74
#define MOTION_STAT_MAX_BOUNCE_OFFSET          0x78
#define MOTION_STAT_PROBE_BOUNCE_OFFSET        0x7C

#define MOTION_CMD_START_MASK        0x1
#define MOTION_CMD_SET_X_MASK        0x2
#define MOTION_CMD_SET_V_MASK        0x4
#define MOTION_CMD_SET_A_MASK        0x8
#define MOTION_CMD_SET_J_MASK        0x10

#define MOTION_STAT_STATUS_STOPPED_MASK        0x1

#define MOTION_STEP_BIT_SHIFT_MASK        0x3F
#define MOTION_STEP_BIT_ENABLE_MASK        0x80000000
#define MOTION_STATS_MUX_MASK 		   0x40000000
#define MOTION_STATS_MUX_START		   0x00000000
#define MOTION_STATS_MUX_CURRENT	   0x40000000

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
 *      void MOTION_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
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
 *      Xuint32 MOTION_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define MOTION_mReadReg(BaseAddress, RegOffset) \
        XIo_In32((BaseAddress) + (RegOffset))


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
 *      void MOTION_mReset(Xuint32 BaseAddress)
 *
 */
#define MOTION_mReset(BaseAddress) \
        XIo_Out32((BaseAddress)+(MOTION_RST_REG_OFFSET), SOFT_RESET)

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

void MOTION_Start(void * baseaddr_p);
void MOTION_SetDT(void * baseaddr_p, uint32_t val);
void MOTION_SetSteps(void * baseaddr_p, uint32_t val);
void MOTION_SetX(void * baseaddr_p, int64_t val);
void MOTION_SetV(void * baseaddr_p, int32_t val);
void MOTION_SetA(void * baseaddr_p, int32_t val);
void MOTION_SetJ(void * baseaddr_p, int32_t val);

#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */
