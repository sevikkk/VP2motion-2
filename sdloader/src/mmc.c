#include "xparameters.h"

#include "stdio.h"
#include "xenv.h"
#include <sys/types.h>
#include <inttypes.h>

#include "xutil.h"
#include "xstatus.h"
#include "xspi.h"
#include "mmc.h"

static XSpi Spi_SD;
static char sd_buf[512];

#define printf xil_printf
#define uint8_t u8
#define uint16_t u16

#define MMC_CMD_SIZE 6

uint8_t mmc_cmd[MMC_CMD_SIZE];

#define MMC_TIMEOUT  0xFF

uint8_t spi_receive_byte(void)
{
  uint8_t byte = 0;

	sd_buf[0] = 0xff;
	XSpi_Transfer(&Spi_SD, sd_buf, sd_buf, 1);
  
  return sd_buf[0];
}

void spi_send(uint8_t* data, uint16_t length)
{
  XSpi_Transfer(&Spi_SD, data, NULL, length);
}

void spi_receive(uint8_t* data, uint16_t length)
{
	uint16_t i;
  for(i = 0; i < length; i++)
  {
    sd_buf[i] = 0xff;
  };
  XSpi_Transfer(&Spi_SD, sd_buf, data, length);
}

void SET_CS(void) {
	XSpi_SetSlaveSelect (&Spi_SD,0);
};

void CLEAR_CS(void) {
	XSpi_SetSlaveSelect (&Spi_SD,1);
};

uint8_t mmc_response(uint8_t response)
{
  uint16_t count = 0xFFF;
  uint8_t result;

  while(count > 0)
  {
    result = spi_receive_byte();
    if (result == response)
      break;
    count--;
  }

  if (count == 0)
    return 1; /* Failure, loop was exited due to timeout */

  return 0; /* Normal, loop was exited before timeout */
}

int mmc_init(void)
{
  uint8_t i;
  uint8_t byte;

  /* Init the CS output */

  SET_CS();
  {
    /* Init the card in SPI mode by sending 80 clks */
    for(i = 0; i < 10; i++)
      byte = spi_receive_byte();
  }
  CLEAR_CS();

  printf("send CMD0\r\n");

  /* Send CMD0 GO_IDLE_STATE */
  mmc_cmd[0] = 0x40;
  mmc_cmd[1] = 0x00;
  mmc_cmd[2] = 0x00;
  mmc_cmd[3] = 0x00;
  mmc_cmd[4] = 0x00;
  mmc_cmd[5] = 0x95;
  spi_send(mmc_cmd, MMC_CMD_SIZE);

  if (mmc_response(0x01))
  {
    printf("CMD0 timeout\r\n");
    return -1;
  }

  printf("send dummy clocks\r\n");

  /* Send some dummy clocks after GO_IDLE_STATE */
  spi_receive_byte();
  SET_CS();
  spi_receive_byte();
  spi_receive_byte();
  CLEAR_CS();

  printf("send CMD1\r\n");

  /* Send CMD1 SEND_OP_COND until response is 0 */
  i = MMC_TIMEOUT;
  do
  {
    mmc_cmd[0] = 0x41;
    mmc_cmd[1] = 0x00;
    mmc_cmd[2] = 0x00;
    mmc_cmd[3] = 0x00;
    mmc_cmd[4] = 0x00;
    mmc_cmd[5] = 0xFF;
    spi_send(mmc_cmd, MMC_CMD_SIZE);
    i--;
  } while((mmc_response(0x00) != 0) && (i > 0));

  if (i == 0)
  {
    printf("CMD1 timeout\r\n");
    return -1;
  }

  printf("send dummy clocks\r\n");

  /* Send some dummy clocks after GO_IDLE_STATE */
  spi_receive_byte();
  SET_CS();
  spi_receive_byte();
  spi_receive_byte();
  CLEAR_CS();

  printf("send CMD16\r\n");

  /* Send CMD16 SET_BLOCKLEN to set the block length */
  mmc_cmd[0] = 0x50;
  mmc_cmd[1] = 0x00;   /* 4 bytes from here is the block length */
                       /* LSB is first */
                       /* 00 00 00 10 set to 16 bytes */
                       /* 00 00 02 00 set to 512 bytes */
  mmc_cmd[2] = 0x00;
  /* high block length bits - 512 bytes */
  mmc_cmd[3] = 0x02;
  /* low block length bits */
  mmc_cmd[4] = 0x00;
  mmc_cmd[5] = 0xFF; /* checksum is no longer required but we always send 0xFF */
  spi_send(mmc_cmd, MMC_CMD_SIZE);

  if ((mmc_response(0x00)) == 1)
  {
    printf("CMD16 timeout\r\n");
  }

  spi_receive_byte();
  SET_CS();
  spi_receive_byte();
  spi_receive_byte();

  printf("mmc_init end\r\n");
  return 0;
}

int mmc_read_block(uint32_t block_number, uint8_t* block_address)
{
  uint16_t checksum;
  uint16_t varh, varl;

  varl = (block_number & 0x007F) << 9;
  varh = (block_number >> 7) & 0xffff;

  //printf("read block %d %d %d\r\n", block_number, varl, varh);

  CLEAR_CS();
  {
    /* send MMC CMD17(READ_SINGLE_BLOCK) to read the data from MMC card */
    mmc_cmd[0] = 0x51;
    /* high block address bits, varh HIGH and LOW */
    mmc_cmd[1] = (varh >> 0x08);
    mmc_cmd[2] = (varh & 0xFF);
    /* low block address bits, varl HIGH and LOW */
    mmc_cmd[3] = (varl >> 0x08);
    mmc_cmd[4] = (varl & 0xFF);
    /* checksum is no longer required but we always send 0xFF */
    mmc_cmd[5] = 0xFF;
    spi_send(mmc_cmd, MMC_CMD_SIZE);

    //printf("mmc_cmd: ");
    //print_block(mmc_cmd, 16);

    /* if mmc_response returns 1 then we failed to get a 0x00 response */
    if ((mmc_response(0x00)) == 1)
    {
      printf("read block timeout\r\n");
      return -1;
    }

    /* wait for data token */
    if ((mmc_response(0xFE)) == 1)
    {
      printf("data token missing\r\n");
      return -1;
    }

    /* printf("receive block\r\n"); */

    /* Get the block of data based on the length */
    spi_receive(block_address, MMC_BLOCK_SIZE);

    /* CRC bytes that are not needed */
    checksum = spi_receive_byte();
    checksum = checksum << 0x08 | spi_receive_byte();
  }
  SET_CS();
  
  spi_receive_byte();
  
  //printf("block received\r\n");
  return 0;
}

void print_block(uint8_t *block_address, uint16_t length)
{
  uint16_t i, j;
  uint8_t byte;
  
  for(i = 0; i < length / 16; i++)
  {
    printf("%04x: ", i * 16);
    
    for(j = 0; j < 16; j++)
    {
      printf("%02x", block_address[i * 16 + j]);
    }
    
    printf(" ");
    for(j = 0; j < 16; j++)
    {
      byte = block_address[i * 16 + j];
      
      if ((byte >= 32) && byte <= 126)
        printf("%c", byte);
      else
        printf(".");
    }

    printf("\r\n");
  }
}

int mmc_if_init(void)
{
	XStatus s;
	s = XSpi_Initialize(&Spi_SD, XPAR_XPS_SPI_SD_DEVICE_ID);
	if (s != XST_SUCCESS) {
		xil_printf("sd_spi_init: %d\r\n", s);
		return -1;
	};
	XSpi_SetOptions(&Spi_SD, XSP_MASTER_OPTION | XSP_MANUAL_SSELECT_OPTION |
							XSP_CLK_ACTIVE_LOW_OPTION | XSP_CLK_PHASE_1_OPTION);
	XSpi_Start(&Spi_SD);
	XSpi_mIntrGlobalDisable(&Spi_SD);
	XSpi_SetSlaveSelect (&Spi_SD,0);
	return 0;
}

