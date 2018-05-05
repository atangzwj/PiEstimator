

/************ Include Files ************/

#include "PiEstimator.h"
#include "sleep.h"


/************ Function Prototypes ************/

void clear(u32 baseAddr);
void disable(u32 baseAddr);
void enable(u32 baseAddr);
u32  isDone(u32 baseAddr);
void setSeeds(u32 baseAddr, u32 seedX, u32 seedY);
void setIterations(u32 baseAddr, u32 iterations);
u32  getInnerPoints(u32 baseAddr);
u32  getTotalPoints(u32 baseAddr);
u32  getTimer(u32 baseAddr);


/************ Function Definitions ************/

double PiEstimator_runSimulation(u32 *baseAddrs, u32 *seedsX, u32 *seedsY,
      u8 numSimulators, u32 iterations, u64 *data) {
   disable(baseAddrs[0]);
   clear(baseAddrs[0]);
   for (u8 i = 0; i < numSimulators; i++) {
      setSeeds(baseAddrs[i], seedsX[i], seedsY[i]);
      setIterations(baseAddrs[i], iterations);
   }
   enable(baseAddrs[0]);

   // Wait for all simulators to finish
   for (u8 i = 0; i < numSimulators; i++) {
      u8 done = 0;
      do {
         done = isDone(baseAddrs[i]);
      } while (!done);
   }

   u64 innerPoints  = 0;
   u64 totalPoints  = 0;
   u64 timerReading = 0;
   for (u8 i = 0; i < numSimulators; i++) {
      innerPoints += getInnerPoints(baseAddrs[i]);
      totalPoints += getTotalPoints(baseAddrs[i]);

      u32 timerTemp = getTimer(baseAddrs[i]);
      if (timerTemp > timerReading) {
         timerReading = timerTemp;
      }
   }
   data[0] = innerPoints;
   data[1] = totalPoints;
   data[2] = timerReading;

   return 4.0 * innerPoints / totalPoints;
}

void clear(u32 baseAddr) {
   Xil_Out32(baseAddr + PIESTIMATOR_CTRL_OFFSET, 0x0);
   usleep(1);
   Xil_Out32(baseAddr + PIESTIMATOR_CTRL_OFFSET, 0x2);
   usleep(1);
   Xil_Out32(baseAddr + PIESTIMATOR_CTRL_OFFSET, 0x0);
}

void disable(u32 baseAddr) {
   Xil_Out32(baseAddr + PIESTIMATOR_CTRL_OFFSET, 0x0);
}

void enable(u32 baseAddr) {
   Xil_Out32(baseAddr + PIESTIMATOR_CTRL_OFFSET, 0x1);
   usleep(1);
   Xil_Out32(baseAddr + PIESTIMATOR_CTRL_OFFSET, 0x0);
}

u32 isDone(u32 baseAddr) {
   return Xil_In32(baseAddr + PIESTIMATOR_STATUS_OFFSET) & 0x2;
}

void setSeeds(u32 baseAddr, u32 seedX, u32 seedY) {
   Xil_Out32(baseAddr + PIESTIMATOR_SEED_X_OFFSET, seedX);
   Xil_Out32(baseAddr + PIESTIMATOR_SEED_Y_OFFSET, seedY);
}

void setIterations(u32 baseAddr, u32 iterations) {
   Xil_Out32(baseAddr + PIESTIMATOR_ITERATIONS_OFFSET, iterations);
}

u32 getInnerPoints(u32 baseAddr) {
   return Xil_In32(baseAddr + PIESTIMATOR_INNERPOINTS_OFFSET);
}

u32 getTotalPoints(u32 baseAddr) {
   return Xil_In32(baseAddr + PIESTIMATOR_TOTALPOINTS_OFFSET);
}

u32 getTimer(u32 baseAddr) {
   return Xil_In32(baseAddr + PIESTIMATOR_TIMER_OFFSET);
}
