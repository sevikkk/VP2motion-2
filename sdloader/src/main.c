#include "xparameters.h"

#include "stdio.h"
#include "xenv.h"

#include "xutil.h"
#include "xstatus.h"
#include "xuartlite_l.h"

#include <sys/types.h>
#include <inttypes.h>

#include "mmc.h"
#include "osram.h"

#include "ff.h"
#include "elf.h"
#define printf xil_printf
#include "cmdline.h"

const char osram_Boot[26] = {
	0x03, 0x0f,
	0xc5, 0xe1, 0x96, 0xf2, 0x8c, 0xf0,
	0x45, 0x01, 0x76, 0x8a, 0x8c, 0x70,
	0x85, 0x01, 0x76, 0x8a, 0x8c, 0x70,
	0x05, 0x41, 0xe6, 0x42, 0x44, 0x30
};

char buf[MMC_BLOCK_SIZE];

FATFS Fatfs;			/* File system object */
FIL Fil;			/* File object */

int bootelf(void)
{
	XStatus s;
	FRESULT rc;		/* Result code */
	int i;
	Elf32_Ehdr *ehdr;
	Elf32_Phdr *phdr;
	int br, ph_offset, ph_num, ph_size;
	void (*entry) (void);

	f_mount(0, &Fatfs);	/* Register volume work area (never fails) */

	rc = f_open(&Fil, "boot.elf", FA_READ);
	if (rc) {
		xil_printf("f_open: %d\r\n", rc);
		return -1;
	};
	//xil_printf("first cluster: %x(%d)\r\n", Fil.sclust, Fil.sclust);

	rc = f_read(&Fil, buf, MMC_BLOCK_SIZE, &i);
	if (rc) {
		xil_printf("f_read: %d\r\n", rc);
		return -1;
	};
	//print_block(buf, 512);
	//xil_printf("num: %d\r\n", i);
	ehdr = (Elf32_Ehdr *) buf;
	if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
	    ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
	    ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
	    ehdr->e_ident[EI_MAG3] != ELFMAG3) {
		printf("ERROR: Bad magic\r\n");
		return -1;
	};

	ph_offset = ehdr->e_phoff;
	ph_num = ehdr->e_phnum;
	ph_size = ehdr->e_phentsize;

	printf("class: %d data: %d version: %d\r\n", ehdr->e_ident[EI_CLASS],
	       ehdr->e_ident[EI_DATA], ehdr->e_ident[EI_VERSION]);
	printf("type: %d machine: %d version: %d\r\n", ehdr->e_type,
	       ehdr->e_machine, ehdr->e_version);
	printf("phoff: %d phentsize: %d phnum: %d\r\n", ph_offset, ph_size,
	       ph_num);
	printf("entry: %x\r\n", ehdr->e_entry);

	for (i = 0; i < ph_num; i++) {
		phdr = (Elf32_Phdr *) (buf + ph_offset + ph_size * i);
		printf
		    ("  seg%d: type: %d offset: %d addr: %d fsize: %d msize: %d\r\n",
		     i, phdr->p_type, phdr->p_offset, phdr->p_vaddr,
		     phdr->p_filesz, phdr->p_memsz);
		if (phdr->p_type == 1) {
			if (phdr->p_filesz > 0) {
#define CHUNK_SIZE 32768
				int chunk, to_read;
				for (chunk = 0; chunk < phdr->p_filesz;
				     chunk += CHUNK_SIZE) {
					to_read = phdr->p_filesz - chunk;
					if (to_read > CHUNK_SIZE) {
						to_read = CHUNK_SIZE;
					};
					rc = f_lseek(&Fil,
						     phdr->p_offset + chunk);
					if (rc) {
						xil_printf("f_seek: %d\r\n",
							   rc);
						return -1;
					};
					printf
					    ("    read %d bytes form %d to %d\r\n",
					     to_read, phdr->p_offset + chunk,
					     (phdr->p_vaddr + chunk));
					rc = f_read(&Fil,
						    (char *)(phdr->p_vaddr +
							     chunk), to_read,
						    &br);
					if (rc) {
						xil_printf("f_read: %d\r\n",
							   rc);
						return -1;
					};
				};
			};
			if (phdr->p_memsz > phdr->p_filesz) {
				printf("    zero fill %d bytes at %d\r\n",
				       phdr->p_memsz - phdr->p_filesz,
				       phdr->p_vaddr + phdr->p_filesz);
				memset((char *)(phdr->p_vaddr + phdr->p_filesz),
				       0, phdr->p_memsz - phdr->p_filesz);
			};
		};
	};

	/*
	   for (i=0;i<158000; i+= 16384) {
	   printf("%x:\r\n", i);
	   print_block((char *)i, 1024);
	   };
	 */

	printf("Start program at %d\r\n", ehdr->e_entry);
	entry = (void (*)(void))(ehdr->e_entry);
	entry();

	return 0;
}

void do_help(void)
{
	printf("Commands:\r\n"
	       " help           - this help\r\n"
	       " bootelf        - start boot.elf\r\n" "\r\n");
}

void do_bootelf(void)
{
	bootelf();
}

int main(void)
{
	XStatus s;
	FRESULT rc;		/* Result code */
	int i;

	print("\r\n\r\nStart\r\n");

	s = osram_init();
	if (s != XST_SUCCESS) {
		xil_printf("osram_init: %d\r\n", s);
		return -1;
	};

	for (i = 0; i < 26; i++) {
		osram_send(osram_Boot[i]);
	};

	s = mmc_if_init();
	if (s != XST_SUCCESS) {
		xil_printf("mmc_if_init: %d\r\n", s);
		return -1;
	};

	/* boot_elf(); */
	cmdlineInit();
	cmdlineAddCommand("help", do_help);
	cmdlineAddCommand("?", do_help);
	cmdlineAddCommand("bootelf", do_bootelf);

	cmdlinePrintPrompt();
	while (1) {
		if (!XUartLite_mIsReceiveEmpty(STDIN_BASEADDRESS)) {
			cmdlineInputFunc(XUartLite_mReadReg
					 (STDIN_BASEADDRESS,
					  XUL_RX_FIFO_OFFSET));
		};
		cmdlineMainLoop();
	};

	return 0;
}
