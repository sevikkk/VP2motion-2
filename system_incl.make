#################################################################
# Makefile generated by Xilinx Platform Studio 
# Project:/home/seva/src/xc2p_pe1/system.xmp
#
# WARNING : This file will be re-generated every time a command
# to run a make target is invoked. So, any changes made to this  
# file manually, will be lost when make is invoked next. 
#################################################################

XILINX_EDK_DIR = /opt/Xilinx/10.1/EDK
NON_CYG_XILINX_EDK_DIR = /opt/Xilinx/10.1/EDK

SYSTEM = system

MHSFILE = system.mhs

MSSFILE = system.mss

FPGA_ARCH = virtex2p

DEVICE = xc2vp50ff1152-6

LANGUAGE = vhdl

SEARCHPATHOPT = 

SUBMODULE_OPT = 

PLATGEN_OPTIONS = -p $(DEVICE) -lang $(LANGUAGE) $(SEARCHPATHOPT) $(SUBMODULE_OPT)

LIBGEN_OPTIONS = -mhs $(MHSFILE) -p $(DEVICE) $(SEARCHPATHOPT)

VPGEN_OPTIONS = -p $(DEVICE) $(SEARCHPATHOPT)

MANAGE_FASTRT_OPTIONS = -reduce_fanout no

OBSERVE_PAR_OPTIONS = -error yes

TESTAPP_MEMORY_OUTPUT_DIR = TestApp_Memory
TESTAPP_MEMORY_OUTPUT = $(TESTAPP_MEMORY_OUTPUT_DIR)/executable.elf

MEMTEST_OUTPUT_DIR = memtest
MEMTEST_OUTPUT = $(MEMTEST_OUTPUT_DIR)/executable.elf

SDLOADER_OUTPUT_DIR = sdloader
SDLOADER_OUTPUT = $(SDLOADER_OUTPUT_DIR)/executable.elf

MICROBLAZE_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/microblaze/mb_bootloop.elf
PPC405_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/ppc405/ppc_bootloop.elf
PPC440_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/ppc440/ppc440_bootloop.elf
BOOTLOOP_DIR = bootloops

PPC405_0_BOOTLOOP = $(BOOTLOOP_DIR)/ppc405_0.elf

BRAMINIT_ELF_FILES =  $(SDLOADER_OUTPUT) 
BRAMINIT_ELF_FILE_ARGS =   -pe ppc405_0 $(SDLOADER_OUTPUT) 

ALL_USER_ELF_FILES = $(TESTAPP_MEMORY_OUTPUT) $(MEMTEST_OUTPUT) $(SDLOADER_OUTPUT) 

SIM_CMD = vsim

BEHAVIORAL_SIM_SCRIPT = simulation/behavioral/$(SYSTEM)_setup.do

STRUCTURAL_SIM_SCRIPT = simulation/structural/$(SYSTEM)_setup.do

TIMING_SIM_SCRIPT = simulation/timing/$(SYSTEM)_setup.do

DEFAULT_SIM_SCRIPT = $(BEHAVIORAL_SIM_SCRIPT)

MIX_LANG_SIM_OPT = -mixed yes

SIMGEN_OPTIONS = -p $(DEVICE) -lang $(LANGUAGE) $(SEARCHPATHOPT) $(BRAMINIT_ELF_FILE_ARGS) $(MIX_LANG_SIM_OPT)  -s mti


LIBRARIES =  \
       ppc405_0/lib/libxil.a 
VPEXEC = virtualplatform/vpexec

LIBSCLEAN_TARGETS = ppc405_0_libsclean 

PROGRAMCLEAN_TARGETS = TestApp_Memory_programclean memtest_programclean sdloader_programclean 

CORE_STATE_DEVELOPMENT_FILES = 

WRAPPER_NGC_FILES = implementation/ppc405_0_wrapper.ngc \
implementation/jtagppc_cntlr_0_wrapper.ngc \
implementation/plb0_wrapper.ngc \
implementation/xps_bram_if_cntlr_1_wrapper.ngc \
implementation/plb_bram_if_cntlr_1_bram_wrapper.ngc \
implementation/leds_wrapper.ngc \
implementation/sdr_sdram_custom_wrapper.ngc \
implementation/ppc405_0_iplb1_wrapper.ngc \
implementation/ppc405_0_dplb1_wrapper.ngc \
implementation/clock_generator_0_wrapper.ngc \
implementation/proc_sys_reset_0_wrapper.ngc \
implementation/xps_intc_0_wrapper.ngc \
implementation/xps_timebase_wdt_0_wrapper.ngc \
implementation/xps_ethernetlite_0_wrapper.ngc \
implementation/xps_timer_0_wrapper.ngc \
implementation/xps_spi_sd_wrapper.ngc \
implementation/xps_spi_max_wrapper.ngc \
implementation/xps_spi_osram_wrapper.ngc \
implementation/fit_timer_0_wrapper.ngc \
implementation/osram_data_inv_wrapper.ngc \
implementation/osram_clk_inv_wrapper.ngc \
implementation/osram_load_inv_wrapper.ngc \
implementation/rs232_wrapper.ngc

POSTSYN_NETLIST = implementation/$(SYSTEM).ngc

SYSTEM_BIT = implementation/$(SYSTEM).bit

DOWNLOAD_BIT = implementation/download.bit

SYSTEM_ACE = implementation/$(SYSTEM).ace

UCF_FILE = data/system.ucf

BMM_FILE = implementation/$(SYSTEM).bmm

BITGEN_UT_FILE = etc/bitgen.ut

XFLOW_OPT_FILE = etc/fast_runtime.opt
XFLOW_DEPENDENCY = __xps/xpsxflow.opt $(XFLOW_OPT_FILE)

XPLORER_DEPENDENCY = __xps/xplorer.opt
XPLORER_OPTIONS = -p $(DEVICE) -uc $(SYSTEM).ucf -bm $(SYSTEM).bmm -max_runs 7

FPGA_IMP_DEPENDENCY = $(BMM_FILE) $(POSTSYN_NETLIST) $(UCF_FILE) $(XFLOW_DEPENDENCY)

#################################################################
# SOFTWARE APPLICATION TESTAPP_MEMORY
#################################################################

TESTAPP_MEMORY_SOURCES = TestApp_Memory/src/TestApp_Memory.c 

TESTAPP_MEMORY_HEADERS = 

TESTAPP_MEMORY_CC = powerpc-eabi-gcc
TESTAPP_MEMORY_CC_SIZE = powerpc-eabi-size
TESTAPP_MEMORY_CC_OPT = -O2
TESTAPP_MEMORY_CFLAGS = 
TESTAPP_MEMORY_CC_SEARCH = # -B
TESTAPP_MEMORY_LIBPATH = -L./ppc405_0/lib/ # -L
TESTAPP_MEMORY_INCLUDES = -I./ppc405_0/include/ # -I
TESTAPP_MEMORY_LFLAGS =   -llwip4 
TESTAPP_MEMORY_LINKER_SCRIPT = TestApp_Memory/src/TestApp_Memory_LinkScr.ld
TESTAPP_MEMORY_LINKER_SCRIPT_FLAG = -Wl,-T -Wl,$(TESTAPP_MEMORY_LINKER_SCRIPT) 
TESTAPP_MEMORY_CC_DEBUG_FLAG =  -g 
TESTAPP_MEMORY_CC_PROFILE_FLAG = # -pg
TESTAPP_MEMORY_CC_GLOBPTR_FLAG= # -msdata=eabi
TESTAPP_MEMORY_CC_INFERRED_FLAGS= 
TESTAPP_MEMORY_CC_START_ADDR_FLAG=  #  # -Wl,-defsym -Wl,_START_ADDR=
TESTAPP_MEMORY_CC_STACK_SIZE_FLAG=  #  # -Wl,-defsym -Wl,_STACK_SIZE=
TESTAPP_MEMORY_CC_HEAP_SIZE_FLAG=  #  # -Wl,-defsym -Wl,_HEAP_SIZE=
TESTAPP_MEMORY_OTHER_CC_FLAGS= $(TESTAPP_MEMORY_CC_GLOBPTR_FLAG)  \
                  $(TESTAPP_MEMORY_CC_START_ADDR_FLAG) $(TESTAPP_MEMORY_CC_STACK_SIZE_FLAG) $(TESTAPP_MEMORY_CC_HEAP_SIZE_FLAG)  \
                  $(TESTAPP_MEMORY_CC_INFERRED_FLAGS)  \
                  $(TESTAPP_MEMORY_LINKER_SCRIPT_FLAG) $(TESTAPP_MEMORY_CC_DEBUG_FLAG) $(TESTAPP_MEMORY_CC_PROFILE_FLAG) 

#################################################################
# SOFTWARE APPLICATION MEMTEST
#################################################################

MEMTEST_SOURCES = memtest/src/TestApp_Memory.c 

MEMTEST_HEADERS = 

MEMTEST_CC = powerpc-eabi-gcc
MEMTEST_CC_SIZE = powerpc-eabi-size
MEMTEST_CC_OPT = -O2
MEMTEST_CFLAGS = 
MEMTEST_CC_SEARCH = # -B
MEMTEST_LIBPATH = -L./ppc405_0/lib/ # -L
MEMTEST_INCLUDES = -I./ppc405_0/include/ # -I
MEMTEST_LFLAGS = # -l
MEMTEST_LINKER_SCRIPT = memtest/memtest_linker_script.ld
MEMTEST_LINKER_SCRIPT_FLAG = -Wl,-T -Wl,$(MEMTEST_LINKER_SCRIPT) 
MEMTEST_CC_DEBUG_FLAG =  -g 
MEMTEST_CC_PROFILE_FLAG = # -pg
MEMTEST_CC_GLOBPTR_FLAG= # -msdata=eabi
MEMTEST_CC_INFERRED_FLAGS= 
MEMTEST_CC_START_ADDR_FLAG=  #  # -Wl,-defsym -Wl,_START_ADDR=
MEMTEST_CC_STACK_SIZE_FLAG=  #  # -Wl,-defsym -Wl,_STACK_SIZE=
MEMTEST_CC_HEAP_SIZE_FLAG=  #  # -Wl,-defsym -Wl,_HEAP_SIZE=
MEMTEST_OTHER_CC_FLAGS= $(MEMTEST_CC_GLOBPTR_FLAG)  \
                  $(MEMTEST_CC_START_ADDR_FLAG) $(MEMTEST_CC_STACK_SIZE_FLAG) $(MEMTEST_CC_HEAP_SIZE_FLAG)  \
                  $(MEMTEST_CC_INFERRED_FLAGS)  \
                  $(MEMTEST_LINKER_SCRIPT_FLAG) $(MEMTEST_CC_DEBUG_FLAG) $(MEMTEST_CC_PROFILE_FLAG) 

#################################################################
# SOFTWARE APPLICATION SDLOADER
#################################################################

SDLOADER_SOURCES = sdloader/src/main.c sdloader/src/mmc.c sdloader/src/osram.c sdloader/src/diskio.c sdloader/src/ff.c sdloader/src/unicode.c 

SDLOADER_HEADERS = 

SDLOADER_CC = powerpc-eabi-gcc
SDLOADER_CC_SIZE = powerpc-eabi-size
SDLOADER_CC_OPT = -O2
SDLOADER_CFLAGS = 
SDLOADER_CC_SEARCH = # -B
SDLOADER_LIBPATH = -L./ppc405_0/lib/ # -L
SDLOADER_INCLUDES = -I./ppc405_0/include/ # -I
SDLOADER_LFLAGS = # -l
SDLOADER_LINKER_SCRIPT = sdloader/sdloader_linker_script.ld
SDLOADER_LINKER_SCRIPT_FLAG = -Wl,-T -Wl,$(SDLOADER_LINKER_SCRIPT) 
SDLOADER_CC_DEBUG_FLAG =  -g 
SDLOADER_CC_PROFILE_FLAG = # -pg
SDLOADER_CC_GLOBPTR_FLAG= # -msdata=eabi
SDLOADER_CC_INFERRED_FLAGS= 
SDLOADER_CC_START_ADDR_FLAG=  #  # -Wl,-defsym -Wl,_START_ADDR=
SDLOADER_CC_STACK_SIZE_FLAG=  #  # -Wl,-defsym -Wl,_STACK_SIZE=
SDLOADER_CC_HEAP_SIZE_FLAG=  #  # -Wl,-defsym -Wl,_HEAP_SIZE=
SDLOADER_OTHER_CC_FLAGS= $(SDLOADER_CC_GLOBPTR_FLAG)  \
                  $(SDLOADER_CC_START_ADDR_FLAG) $(SDLOADER_CC_STACK_SIZE_FLAG) $(SDLOADER_CC_HEAP_SIZE_FLAG)  \
                  $(SDLOADER_CC_INFERRED_FLAGS)  \
                  $(SDLOADER_LINKER_SCRIPT_FLAG) $(SDLOADER_CC_DEBUG_FLAG) $(SDLOADER_CC_PROFILE_FLAG) 
