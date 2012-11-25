#include "xparameters.h"

#include "stdio.h"
#include "xenv.h"

#include "xutil.h"
#include "xstatus.h"
#include "xspi.h"

static XSpi Spi_OSRAM;
static char osram_buf[2];

void osram_send(char c)
{
	XSpi_SetSlaveSelect(&Spi_OSRAM, 1);
	osram_buf[0] = c;
	XSpi_Transfer(&Spi_OSRAM, osram_buf, NULL, 1);
	XSpi_SetSlaveSelect(&Spi_OSRAM, 0);
}

int osram_init(void)
{
	XStatus s;

	s = XSpi_Initialize(&Spi_OSRAM, XPAR_XPS_SPI_OSRAM_DEVICE_ID);
	if (s != XST_SUCCESS) {
		xil_printf("osram_init: %d\r\n", s);
		return -1;
	};
	XSpi_SetOptions(&Spi_OSRAM,
			XSP_MASTER_OPTION | XSP_MANUAL_SSELECT_OPTION);
	XSpi_Start(&Spi_OSRAM);
	XSpi_mIntrGlobalDisable(&Spi_OSRAM);
	XSpi_SetSlaveSelect(&Spi_OSRAM, 0);

	return 0;
}
