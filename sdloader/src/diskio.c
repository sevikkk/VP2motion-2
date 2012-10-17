/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2012        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include <sys/types.h>
#include <inttypes.h>
#include "diskio.h"		/* FatFs lower layer API */
#include "mmc.h"		/* FatFs lower layer API */
#include <stdio.h>

/* Definitions of physical drive number for each media */
#define SD		0


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (drv) {
	case SD:
		result = mmc_init();
		return result;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
)
{
	int result, i;

	switch (drv) {
	case SD :
		// translate the arguments here
		/* xil_printf("readblock: %d %d\r\n",sector, count); */
		for (i=0; i<count; i++) {
			result = mmc_read_block(sector + i, buff + MMC_BLOCK_SIZE*i);
			if (result != 0)
				return RES_ERROR;
		};
		/* print_block(buff, count*MMC_BLOCK_SIZE); */

		return RES_OK;
	}
	return RES_PARERR;
}

