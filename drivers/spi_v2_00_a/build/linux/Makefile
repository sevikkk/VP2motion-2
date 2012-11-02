#
# Makefile for the Xilinx SPI driver
#

ifeq ($(CONFIG_PPC32),y)
EXTRA_CFLAGS		+= -I$(TOPDIR)/arch/ppc/platforms/xilinx_ocp
endif

list-multi		:= xilinx_spi.o

# The Linux adapter for the Xilinx driver code.
xilinx_spi-objs		+= xspi_adapter.o

# The Xilinx OS independent code.
xilinx_spi-objs		+= xspi.o xspi_sinit.o xspi_g.o xspi_options.o xspi_stats.o

obj-$(CONFIG_XILINX_SPI) := xilinx_spi.o

xilinx_spi.o: $(xilinx_spi-objs)
	$(LD) -r -o $@ $(xilinx_spi-objs)

include $(TOPDIR)/Rules.make
