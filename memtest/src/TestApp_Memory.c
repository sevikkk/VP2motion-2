/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * Xilinx EDK 10.1.03 EDK_K_SP3.6
 *
 * This file is a sample test application
 *
 * This application is intended to test and/or illustrate some 
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * XPS project when you run the "Generate Libraries" menu item
 * in XPS.
 *
 * Your XPS project directory is at:
 *    /home/seva/src/xc2p_pe1/
 */

// Located in: ppc405_0/include/xparameters.h
#include "xparameters.h"

#include "stdio.h"

#include "xutil.h"
#include "xstatus.h"

#define MEMBS (64*1024*1024)
#define MEMCHUNKS 32
#define MEMCH (MEMBS/MEMCHUNKS)
int main(void)
{
  /* Testing MPMC Memory (SDR_SDRAM_CUSTOM)*/
   int i = 0, j=0, fails = 0;
   XStatus status;

   while (1) {
		for (i=0; i<MEMCHUNKS; i++) {
			xil_printf("Starting MemoryTest for SDR_SDRAM_CUSTOM (%d/%d #%d) (failed: %d):\r\n", i, MEMCHUNKS, j, fails);
			print("  Running 32-bit test...");
			status = XUtil_MemoryTest32((Xuint32*)(XPAR_SDR_SDRAM_CUSTOM_MPMC_BASEADDR + MEMCH*i), MEMCH/4, 0xAAAA5555, XUT_ALLMEMTESTS);
			if (status == XST_SUCCESS) {
				print("PASSED!\r\n");
			}
			else {
				print("FAILED!\r\n");
				fails++;
			}
			print("  Running 16-bit test...");
			status = XUtil_MemoryTest16((Xuint16*)(XPAR_SDR_SDRAM_CUSTOM_MPMC_BASEADDR + MEMCH*i), MEMCH/2, 0xAA55, XUT_ALLMEMTESTS);
			if (status == XST_SUCCESS) {
				print("PASSED!\r\n");
			}
			else {
				print("FAILED!\r\n");
				fails++;
			}
			print("  Running 8-bit test...");
			status = XUtil_MemoryTest8((Xuint8*)(XPAR_SDR_SDRAM_CUSTOM_MPMC_BASEADDR + MEMCH*i), MEMCH, 0xA5, XUT_ALLMEMTESTS);
			if (status == XST_SUCCESS) {
				print("PASSED!\r\n");
			}
			else {
				print("FAILED!\r\n");
				fails++;
			};
		};
		j++;
	}

	return 0;
}
