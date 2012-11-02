/* $Id: spi_intr_header.h,v 1.1.2.1 2008/07/28 09:49:50 svemula Exp $ */


#include "xbasic_types.h"
#include "xstatus.h"

int SpiIntrExample(XIntc *IntcInstancePtr, \
                       XSpi *SpiInstancePtr, \
                       u16 SpiDeviceId, \
                       u16 SpiIntrId);

