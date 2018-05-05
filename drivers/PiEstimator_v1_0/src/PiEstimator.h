
#ifndef PIESTIMATOR_H
#define PIESTIMATOR_H


/****************** Include Files ********************/
#include "xil_types.h"
#include "xstatus.h"

#define PIESTIMATOR_S00_AXI_SLV_REG0_OFFSET 0x00
#define PIESTIMATOR_S00_AXI_SLV_REG1_OFFSET 0x04
#define PIESTIMATOR_S00_AXI_SLV_REG2_OFFSET 0x08
#define PIESTIMATOR_S00_AXI_SLV_REG3_OFFSET 0x0C
#define PIESTIMATOR_S00_AXI_SLV_REG4_OFFSET 0x10
#define PIESTIMATOR_S00_AXI_SLV_REG5_OFFSET 0x14
#define PIESTIMATOR_S00_AXI_SLV_REG6_OFFSET 0x18
#define PIESTIMATOR_S00_AXI_SLV_REG7_OFFSET 0x1C

#define PIESTIMATOR_INNERPOINTS_OFFSET PIESTIMATOR_S00_AXI_SLV_REG0_OFFSET
#define PIESTIMATOR_TOTALPOINTS_OFFSET PIESTIMATOR_S00_AXI_SLV_REG1_OFFSET
#define PIESTIMATOR_TIMER_OFFSET       PIESTIMATOR_S00_AXI_SLV_REG2_OFFSET
#define PIESTIMATOR_SEED_X_OFFSET      PIESTIMATOR_S00_AXI_SLV_REG3_OFFSET
#define PIESTIMATOR_SEED_Y_OFFSET      PIESTIMATOR_S00_AXI_SLV_REG4_OFFSET
#define PIESTIMATOR_ITERATIONS_OFFSET  PIESTIMATOR_S00_AXI_SLV_REG5_OFFSET
#define PIESTIMATOR_STATUS_OFFSET      PIESTIMATOR_S00_AXI_SLV_REG6_OFFSET
#define PIESTIMATOR_CTRL_OFFSET        PIESTIMATOR_S00_AXI_SLV_REG7_OFFSET


/**************************** Type Definitions *****************************/
/**
 *
 * Write a value to a PIESTIMATOR register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the PIESTIMATORdevice.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void PIESTIMATOR_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data)
 *
 */
#define PIESTIMATOR_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/**
 *
 * Read a value from a PIESTIMATOR register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the PIESTIMATOR device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	u32 PIESTIMATOR_mReadReg(u32 BaseAddress, unsigned RegOffset)
 *
 */
#define PIESTIMATOR_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))

/************************** Function Prototypes ****************************/
/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the PIESTIMATOR instance to be worked on.
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
XStatus PIESTIMATOR_Reg_SelfTest(void * baseaddr_p);

double PiEstimator_runSimulation(u32 *baseAddrs, u32 *seedsX, u32 *seedsY,
      u8 numSimulators, u32 iterations, u64 *data);

#endif // PIESTIMATOR_H
