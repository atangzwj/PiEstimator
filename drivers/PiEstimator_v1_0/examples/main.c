/*
 * main.c
 *
 *  Created on: Apr 4, 2018
 *      Author: Arvin Tang
 */

#include "PiEstimator.h"
#include "xil_cache.h"
#include "xil_io.h"
#include "xparameters.h"

#define PIESTIMATOR_NUM_INSTANCES XPAR_PIESTIMATOR_NUM_INSTANCES
#define PIESTIMATOR_0_BASEADDR    XPAR_PIESTIMATOR_0_S00_AXI_BASEADDR

#define ITERATIONS 1000000

#ifdef __MICROBLAZE__
#define CLK_FREQ_HZ XPAR_CPU_M_AXI_DP_FREQ_HZ
#else
#define CLK_FREQ_HZ 100000000 // FCLK0 frequency not found in xparameters.h
#endif

u32 lfsr = 0xCC00AA00;

void initSeeds(u32 *seeds);
u32  getRand();
void enableCaches();
void disableCaches();

int main() {
   u32 baseAddrs[PIESTIMATOR_NUM_INSTANCES];
   for (int i = 0; i < PIESTIMATOR_NUM_INSTANCES; i++) {
      baseAddrs[i] = PIESTIMATOR_0_BASEADDR + i * 0x10000;
   }

   u32 seedsX[PIESTIMATOR_NUM_INSTANCES];
   u32 seedsY[PIESTIMATOR_NUM_INSTANCES];
   initSeeds(seedsX);
   initSeeds(seedsY);
   u32 iterations = ITERATIONS;
   u64 data[3];

   double pi = PiEstimator_runSimulation(baseAddrs, seedsX, seedsY,
         PIESTIMATOR_NUM_INSTANCES, iterations, data);

   u32 num_msw = (data[0] >> 32) & 0xFFFFFFFF;
   u32 num_lsw = data[0] & 0xFFFFFFFF;
   u32 denom_msw = (data[1] >> 32) & 0xFFFFFFFF;
   u32 denom_lsw = data[1] & 0xFFFFFFFF;

   u32 pi_int = (u32) pi;
   u32 pi_frac = ((u32) ((pi * 10000000 + 5) / 10)) % 1000000;

   double timer = data[2] * 1000000.0 / CLK_FREQ_HZ; // Time in microseconds
   u32 timer_int = (u32) timer;
   u32 timer_frac = ((u32) ((timer * 10000 + 5) / 10) % 1000);

   xil_printf("num = 0x%08x", num_msw);
   xil_printf("%08x  ", num_lsw);
   xil_printf("denom = 0x%08x", denom_msw);
   xil_printf("%08x\n\r", denom_lsw);

   xil_printf("pi = %d.%06d\n\r", pi_int, pi_frac);
   xil_printf("timer = %d.%03d us\n\r", timer_int, timer_frac);
   return 0;
}

void initSeeds(u32 *seeds) {
   for (int i = 0; i < PIESTIMATOR_NUM_INSTANCES; i++) {
      seeds[i] = getRand();
   }
}

u32 getRand() {
   int iters = 0;
   for (int i = 0; i < 3 && !iters; i++) {
      iters = (lfsr >> (i * 12)) & 0xFFF;
   }
   if (!iters) {
      iters = 1 << 12;
   }
   for (int i = 0; i < iters; i++) {
      u32 bit = (~(lfsr ^ (lfsr >> 1) ^ (lfsr >> 21) ^ (lfsr >> 31))) & 0x1;
      lfsr = (lfsr << 1) | bit;
   }
   return lfsr;
}

void enableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheEnable();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheEnable();
#endif
#endif
}

void disableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheDisable();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheDisable();
#endif
#endif
}
