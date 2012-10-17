#include "xparameters.h"

#include "stdio.h"
#include "xenv.h"

#include "xutil.h"
#include "xstatus.h"

#include <sys/types.h>
#include <inttypes.h>

#include "mmc.h"
#include "osram.h"

#include "ff.h"

const char osram_Boot[26] = {
        0x03, 0x0f,
        0xc5, 0xe1, 0x96, 0xf2, 0x8c, 0xf0,
        0x45, 0x01, 0x76, 0x8a, 0x8c, 0x70,
        0x85, 0x01, 0x76, 0x8a, 0x8c, 0x70,
        0x05, 0x41, 0xe6, 0x42, 0x44, 0x30
};

char mmc_block[MMC_BLOCK_SIZE];

FATFS Fatfs;		/* File system object */
FIL Fil;			/* File object */

int main(void)
{
	XStatus s;
	int i;
	print("\r\n\r\nStart\r\n");

	s = osram_init();
	if (s != XST_SUCCESS)
		xil_printf("osram_init: %d\r\n", s);
	
	for (i=0; i<26; i++) {
		osram_send(osram_Boot[i]);
	};

	s = mmc_if_init();
	if (s != XST_SUCCESS)
		xil_printf("mmc_if_init: %d\r\n", s);

  /* Init the MMC */
  xil_printf("mmc_init: ");
  s = mmc_init();
  xil_printf("%d\r\n", s);

  /* Reset the MMC buffer */
  memset(mmc_block, 0xCA, MMC_BLOCK_SIZE);

  /* Read the first block */
  mmc_read_block(0, mmc_block);
  print_block(mmc_block, MMC_BLOCK_SIZE);
  mmc_read_block(1, mmc_block);
  print_block(mmc_block, MMC_BLOCK_SIZE);
  mmc_read_block(2, mmc_block);
  print_block(mmc_block, MMC_BLOCK_SIZE);

{
		FRESULT rc;				/* Result code */
		DIR dir;				/* Directory object */
		FILINFO fno;			/* File information object */
	
		f_mount(0, &Fatfs);		/* Register volume work area (never fails) */


	xil_printf("\r\nOpen root directory.\r\n");
	rc = f_opendir(&dir, "");
	if (rc) {
		xil_printf("opendir: %d\r\n", rc);
	};

	xil_printf("\r\nDirectory listing...\r\n");
	for (;;) {
		rc = f_readdir(&dir, &fno);		/* Read a directory item */
		if (rc || !fno.fname[0]) break;	/* Error or end of dir */
		/* print_block(&fno, sizeof(fno)); */
		if (fno.fattrib & AM_DIR)
			xil_printf("   <dir>  %s\r\n", fno.fname);
		else
			xil_printf("%9d  %s\r\n", fno.fsize, fno.fname);
	}
	if (rc) xil_printf("readdir: %d\r\n", rc);
	rc = f_open(&Fil, "sh.s3g", FA_READ);
	if (rc) xil_printf("open: %d\r\n", rc);

  }
  
	return 0;
}
