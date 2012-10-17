#################################################################
# Makefile generated by Xilinx Platform Studio 
# Project:/home/seva/src/xc2p_pe1/system.xmp
#
# WARNING : This file will be re-generated every time a command
# to run a make target is invoked. So, any changes made to this  
# file manually, will be lost when make is invoked next. 
#################################################################

# Name of the Microprocessor system
# The hardware specification of the system is in file :
# /home/seva/src/xc2p_pe1/system.mhs
# The software specification of the system is in file :
# /home/seva/src/xc2p_pe1/system.mss

include system_incl.make

#################################################################
# PHONY TARGETS
#################################################################
.PHONY: dummy
.PHONY: netlistclean
.PHONY: bitsclean
.PHONY: simclean
.PHONY: vpclean

#################################################################
# EXTERNAL TARGETS
#################################################################
all:
	@echo "Makefile to build a Microprocessor system :"
	@echo "Run make with any of the following targets"
	@echo " "
	@echo "  netlist  : Generates the netlist for the given MHS "
	@echo "  bits     : Runs Implementation tools to generate the bitstream"
	@echo " "
	@echo "  libs     : Configures the sw libraries for this system"
	@echo "  program  : Compiles the program sources for all the processor instances"
	@echo " "
	@echo "  init_bram: Initializes bitstream with BRAM data"
	@echo "  ace      : Generate ace file from bitstream and elf"
	@echo "  download : Downloads the bitstream onto the board"
	@echo " "
	@echo "  sim      : Generates HDL simulation models and runs simulator for chosen simulation mode"
	@echo "  simmodel : Generates HDL simulation models for chosen simulation mode"
	@echo "  behavioral_model : Generates behavioral HDL models with BRAM initialization"
	@echo "  structural_model : Generates structural simulation HDL models with BRAM initialization"
	@echo "  timing   : Generates timing simulation HDL models with BRAM initialization"
	@echo "  vp       : Generates virtual platform model"
	@echo " "
	@echo "  netlistclean: Deletes netlist"
	@echo "  bitsclean: Deletes bit, ncd, bmm files"
	@echo "  hwclean  : Deletes implementation dir"
	@echo "  libsclean: Deletes sw libraries"
	@echo "  programclean: Deletes compiled ELF files"
	@echo "  swclean  : Deletes sw libraries and ELF files"
	@echo "  simclean : Deletes simulation dir"
	@echo "  vpclean  : Deletes virtualplatform dir"
	@echo "  clean    : Deletes all generated files/directories"
	@echo " "
	@echo "  make <target> : (Default)"
	@echo "      Creates a Microprocessor system using default initializations"
	@echo "      specified for each processor in MSS file"


bits: $(SYSTEM_BIT)

ace: $(SYSTEM_ACE)

netlist: $(POSTSYN_NETLIST)

libs: $(LIBRARIES)

program: $(ALL_USER_ELF_FILES)

download: $(DOWNLOAD_BIT) dummy
	@echo "*********************************************"
	@echo "Downloading Bitstream onto the target board"
	@echo "*********************************************"
	impact -batch etc/download.cmd

init_bram: $(DOWNLOAD_BIT)

sim: $(DEFAULT_SIM_SCRIPT)
	cd simulation/behavioral; \
	$(SIM_CMD)  &

simmodel: $(DEFAULT_SIM_SCRIPT)

behavioral_model: $(BEHAVIORAL_SIM_SCRIPT)

structural_model: $(STRUCTURAL_SIM_SCRIPT)

vp: $(VPEXEC)

clean: hwclean libsclean programclean simclean vpclean
	rm -f _impact.cmd
	rm -f *.log

hwclean: netlistclean bitsclean
	rm -rf implementation synthesis xst hdl
	rm -rf xst.srp $(SYSTEM).srp

netlistclean:
	rm -f $(POSTSYN_NETLIST)
	rm -f platgen.log
	rm -f $(BMM_FILE)

bitsclean:
	rm -f $(SYSTEM_BIT)
	rm -f implementation/$(SYSTEM).ncd
	rm -f implementation/$(SYSTEM)_bd.bmm 
	rm -f implementation/$(SYSTEM)_map.ncd 
	rm -f __xps/$(SYSTEM)_routed

simclean: 
	rm -rf simulation/behavioral
	rm -f simgen.log

swclean: libsclean programclean
	@echo ""

libsclean: $(LIBSCLEAN_TARGETS)
	rm -f libgen.log

programclean: $(PROGRAMCLEAN_TARGETS)

vpclean:
	rm -rf virtualplatform
	rm -f vpgen.log

#################################################################
# SOFTWARE PLATFORM FLOW
#################################################################


$(LIBRARIES): $(MHSFILE) $(MSSFILE) __xps/libgen.opt
	@echo "*********************************************"
	@echo "Creating software libraries..."
	@echo "*********************************************"
	libgen $(LIBGEN_OPTIONS) $(MSSFILE)


ppc405_0_libsclean:
	rm -rf ppc405_0/

#################################################################
# SOFTWARE APPLICATION TESTAPP_MEMORY
#################################################################

TestApp_Memory_program: $(TESTAPP_MEMORY_OUTPUT) 

$(TESTAPP_MEMORY_OUTPUT) : $(TESTAPP_MEMORY_SOURCES) $(TESTAPP_MEMORY_HEADERS) $(TESTAPP_MEMORY_LINKER_SCRIPT) \
                    $(LIBRARIES) __xps/testapp_memory_compiler.opt
	@mkdir -p $(TESTAPP_MEMORY_OUTPUT_DIR) 
	$(TESTAPP_MEMORY_CC) $(TESTAPP_MEMORY_CC_OPT) $(TESTAPP_MEMORY_SOURCES) -o $(TESTAPP_MEMORY_OUTPUT) \
	$(TESTAPP_MEMORY_OTHER_CC_FLAGS) $(TESTAPP_MEMORY_INCLUDES) $(TESTAPP_MEMORY_LIBPATH) \
	$(TESTAPP_MEMORY_CFLAGS) $(TESTAPP_MEMORY_LFLAGS) 
	$(TESTAPP_MEMORY_CC_SIZE) $(TESTAPP_MEMORY_OUTPUT) 
	@echo ""

TestApp_Memory_programclean:
	rm -f $(TESTAPP_MEMORY_OUTPUT) 

#################################################################
# SOFTWARE APPLICATION MEMTEST
#################################################################

memtest_program: $(MEMTEST_OUTPUT) 

$(MEMTEST_OUTPUT) : $(MEMTEST_SOURCES) $(MEMTEST_HEADERS) $(MEMTEST_LINKER_SCRIPT) \
                    $(LIBRARIES) __xps/memtest_compiler.opt
	@mkdir -p $(MEMTEST_OUTPUT_DIR) 
	$(MEMTEST_CC) $(MEMTEST_CC_OPT) $(MEMTEST_SOURCES) -o $(MEMTEST_OUTPUT) \
	$(MEMTEST_OTHER_CC_FLAGS) $(MEMTEST_INCLUDES) $(MEMTEST_LIBPATH) \
	$(MEMTEST_CFLAGS) $(MEMTEST_LFLAGS) 
	$(MEMTEST_CC_SIZE) $(MEMTEST_OUTPUT) 
	@echo ""

memtest_programclean:
	rm -f $(MEMTEST_OUTPUT) 

#################################################################
# SOFTWARE APPLICATION SDLOADER
#################################################################

sdloader_program: $(SDLOADER_OUTPUT) 

$(SDLOADER_OUTPUT) : $(SDLOADER_SOURCES) $(SDLOADER_HEADERS) $(SDLOADER_LINKER_SCRIPT) \
                    $(LIBRARIES) __xps/sdloader_compiler.opt
	@mkdir -p $(SDLOADER_OUTPUT_DIR) 
	$(SDLOADER_CC) $(SDLOADER_CC_OPT) $(SDLOADER_SOURCES) -o $(SDLOADER_OUTPUT) \
	$(SDLOADER_OTHER_CC_FLAGS) $(SDLOADER_INCLUDES) $(SDLOADER_LIBPATH) \
	$(SDLOADER_CFLAGS) $(SDLOADER_LFLAGS) 
	$(SDLOADER_CC_SIZE) $(SDLOADER_OUTPUT) 
	@echo ""

sdloader_programclean:
	rm -f $(SDLOADER_OUTPUT) 

#################################################################
# BOOTLOOP ELF FILES
#################################################################



$(PPC405_0_BOOTLOOP): $(PPC405_BOOTLOOP)
	@mkdir -p $(BOOTLOOP_DIR)
	cp -f $(PPC405_BOOTLOOP) $(PPC405_0_BOOTLOOP)

#################################################################
# HARDWARE IMPLEMENTATION FLOW
#################################################################


$(BMM_FILE) \
$(WRAPPER_NGC_FILES): $(MHSFILE) __xps/platgen.opt \
                      $(CORE_STATE_DEVELOPMENT_FILES)
	@echo "****************************************************"
	@echo "Creating system netlist for hardware specification.."
	@echo "****************************************************"
	platgen $(PLATGEN_OPTIONS) $(MHSFILE)

$(POSTSYN_NETLIST): $(WRAPPER_NGC_FILES)
	@echo "Running synthesis..."
	bash -c "cd synthesis; ./synthesis.sh"

__xps/$(SYSTEM)_routed: $(FPGA_IMP_DEPENDENCY)
	@echo "*********************************************"
	@echo "Running Xilinx Implementation tools.."
	@echo "*********************************************"
	@cp -f $(UCF_FILE) implementation/$(SYSTEM).ucf
	xilperl $(NON_CYG_XILINX_EDK_DIR)/data/fpga_impl/manage_fastruntime_opt.pl $(MANAGE_FASTRT_OPTIONS)
	xflow -wd implementation -p $(DEVICE) -implement xflow.opt $(SYSTEM).ngc
	touch __xps/$(SYSTEM)_routed

$(SYSTEM_BIT): __xps/$(SYSTEM)_routed $(BITGEN_UT_FILE)
	xilperl $(NON_CYG_XILINX_EDK_DIR)/data/fpga_impl/observe_par.pl $(OBSERVE_PAR_OPTIONS) implementation/$(SYSTEM).par
	@echo "*********************************************"
	@echo "Running Bitgen.."
	@echo "*********************************************"
	@cp -f $(BITGEN_UT_FILE) implementation/bitgen.ut
	cd implementation; bitgen -w -f bitgen.ut $(SYSTEM)

$(DOWNLOAD_BIT): $(SYSTEM_BIT) $(BRAMINIT_ELF_FILES) __xps/bitinit.opt
	@cp -f implementation/$(SYSTEM)_bd.bmm .
	@echo "*********************************************"
	@echo "Initializing BRAM contents of the bitstream"
	@echo "*********************************************"
	bitinit $(MHSFILE) $(SEARCHPATHOPT) $(BRAMINIT_ELF_FILE_ARGS) \
	-bt $(SYSTEM_BIT) -o $(DOWNLOAD_BIT)
	@rm -f $(SYSTEM)_bd.bmm

$(SYSTEM_ACE): $(DOWNLOAD_BIT) $(TESTAPP_MEMORY_OUTPUT) $(MEMTEST_OUTPUT) $(SDLOADER_OUTPUT) 
	@echo "*********************************************"
	@echo "Creating system ace file"
	@echo "*********************************************"
	xmd -tcl genace.tcl -jprog -hw $(DOWNLOAD_BIT) -elf $(TESTAPP_MEMORY_OUTPUT) $(MEMTEST_OUTPUT) $(SDLOADER_OUTPUT)  -target ppc_hw  -ace $(SYSTEM_ACE)

#################################################################
# SIMULATION FLOW
#################################################################


################## BEHAVIORAL SIMULATION ##################

$(BEHAVIORAL_SIM_SCRIPT): $(MHSFILE) __xps/simgen.opt \
                          $(BRAMINIT_ELF_FILES)
	@echo "*********************************************"
	@echo "Creating behavioral simulation models..."
	@echo "*********************************************"
	simgen $(SIMGEN_OPTIONS) -m behavioral $(MHSFILE)

################## STRUCTURAL SIMULATION ##################

$(STRUCTURAL_SIM_SCRIPT): $(WRAPPER_NGC_FILES) __xps/simgen.opt \
                          $(BRAMINIT_ELF_FILES)
	@echo "*********************************************"
	@echo "Creating structural simulation models..."
	@echo "*********************************************"
	simgen $(SIMGEN_OPTIONS) -sd implementation -m structural $(MHSFILE)


################## TIMING SIMULATION ##################

$(TIMING_SIM_SCRIPT): $(SYSTEM_BIT) __xps/simgen.opt \
                      $(BRAMINIT_ELF_FILES)
	@echo "*********************************************"
	@echo "Creating timing simulation models..."
	@echo "*********************************************"
	simgen $(SIMGEN_OPTIONS) -sd implementation -m timing $(MHSFILE)

#################################################################
# VIRTUAL PLATFORM FLOW
#################################################################


$(VPEXEC): $(MHSFILE) __xps/vpgen.opt
	@echo "****************************************************"
	@echo "Creating virtual platform for hardware specification.."
	@echo "****************************************************"
	vpgen $(VPGEN_OPTIONS) $(MHSFILE)

dummy:
	@echo ""

