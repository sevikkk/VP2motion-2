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
#include "xgpio.h"
#include "xspi.h"
#include "xstatus.h"

#include "xmk.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "netif/xadapter.h"

#define THREAD_STACKSIZE 16384
#define DEFAULT_THREAD_PRIORITY 1

void print_ip(char *msg, struct ip_addr *ip)
{
	print(msg);
	xil_printf("%d.%d.%d.%d\r\n", ip4_addr1(ip), ip4_addr2(ip),
		   ip4_addr3(ip), ip4_addr4(ip));
}

void
print_ip_settings(struct ip_addr *ip, struct ip_addr *mask, struct ip_addr *gw)
{

	print_ip("Board IP: ", ip);
	print_ip("Netmask : ", mask);
	print_ip("Gateway : ", gw);
}

struct netif server_netif;

void network_thread(void *p)
{
	struct netif *netif;
	struct ip_addr ipaddr, netmask, gw;

	/* the mac address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] =
	    { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	sys_sleep(50);
	print("Network thread\r\n");
	netif = &server_netif;

	/* initliaze IP addresses to be used */
	IP4_ADDR(&ipaddr, 192, 168, 0, 113);
	IP4_ADDR(&netmask, 255, 255, 255, 0);
	IP4_ADDR(&gw, 192, 168, 0, 200);

	/* print out IP settings of the board */
	print("\r\n\r\n");
	print("-----lwIP Socket Mode Demo Application ------\r\n");
	print_ip_settings(&ipaddr, &netmask, &gw);

	/* Add network interface to the netif_list, and set it as default */
	if (!xemac_add
	    (netif, &ipaddr, &netmask, &gw, mac_ethernet_address,
	     XPAR_EMACLITE_0_BASEADDR)) {
		//if (!xemac_add(netif, NULL, NULL, NULL, mac_ethernet_address, XPAR_EMACLITE_0_BASEADDR)) {
		xil_printf("Error adding N/W interface\r\n");
		return;
	}
	netif_set_default(netif);

	/* specify that the network if is up */
	netif_set_up(netif);

	/* start packet receive thread - required for lwIP operation */
	sys_thread_new("xemacif_input_thread",
		       (void (*)(void *))xemacif_input_thread, netif,
		       THREAD_STACKSIZE, 1);

	return;
};

//====================================================

XGpio Leds;

void leds_thread(void *unused)
{
	int i = 0;

	XGpio_Initialize(&Leds, XPAR_LEDS_DEVICE_ID);
	XGpio_SetDataDirection(&Leds, 1, 0x0);

	for (;;) {
		XGpio_DiscreteWrite(&Leds, 1, i);
		sys_sleep(1000);
		i++;
	};
};

static XSpi Spi_OSRAM;
static char osram_buf[10];
static char osram_buf2[10];

void osram_thread(void *unused)
{
	int i = 0;
	XStatus s;

	s = XSpi_Initialize(&Spi_OSRAM, XPAR_XPS_SPI_OSRAM_DEVICE_ID);
	s = XSpi_SetOptions(&Spi_OSRAM, XSP_MASTER_OPTION | XSP_MANUAL_SSELECT_OPTION);
	s = XSpi_Start(&Spi_OSRAM);
	XSpi_mIntrGlobalDisable(&Spi_OSRAM);
	s = XSpi_SetSlaveSelect (&Spi_OSRAM,0);

	s = XSpi_SetSlaveSelect (&Spi_OSRAM,1);
	osram_buf[0] = 0x3;
	s = XSpi_Transfer(&Spi_OSRAM, osram_buf, osram_buf2, 1);
	s = XSpi_SetSlaveSelect (&Spi_OSRAM,0);

	XSpi_SetSlaveSelect (&Spi_OSRAM,1);
	osram_buf[0] = 0xf;
	s = XSpi_Transfer(&Spi_OSRAM, osram_buf, osram_buf2, 1);
	XSpi_SetSlaveSelect (&Spi_OSRAM,0);

	XSpi_SetSlaveSelect (&Spi_OSRAM,1);
	osram_buf[0] = 0xc5;
	s = XSpi_Transfer(&Spi_OSRAM, osram_buf, osram_buf2, 1);
	XSpi_SetSlaveSelect (&Spi_OSRAM,0);

	for (;;) {
		XSpi_SetSlaveSelect (&Spi_OSRAM,1);
		osram_buf[0] = (char)(((i<<3 ) | 1) & 0xff);
		s = XSpi_Transfer(&Spi_OSRAM, osram_buf, osram_buf2, 1);
		XSpi_SetSlaveSelect (&Spi_OSRAM,0);

		XSpi_SetSlaveSelect (&Spi_OSRAM,1);
		osram_buf[0] = (char)(((i<<3 ) | 6) & 0xff);
		s = XSpi_Transfer(&Spi_OSRAM, osram_buf, osram_buf2, 1);
		XSpi_SetSlaveSelect (&Spi_OSRAM,0);

		sys_sleep(100);
		i++;
	};
};

void *main_thread(void *unused)
{
	lwip_init();
	sys_thread_new("NW_THREAD", network_thread, NULL, THREAD_STACKSIZE, 2);
	sys_thread_new("LEDS_THREAD", leds_thread, NULL, THREAD_STACKSIZE, 10);
	sys_thread_new("OSRAM_THREAD", osram_thread, NULL, THREAD_STACKSIZE, 3);
};

int main(void)
{
	XCache_EnableICache(0xc0000001);
	XCache_EnableDCache(0xc0000001);

	print("Start\r\n");
	xilkernel_main();
	print("Stop\r\n");
	return 0;
}
