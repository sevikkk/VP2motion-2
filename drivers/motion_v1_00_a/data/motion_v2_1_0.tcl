##############################################################################
## Filename:          /home/seva/src/xc2p_pe1/drivers/motion_v1_00_a/data/motion_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Tue Oct  9 01:16:29 2012 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "motion" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" "C_MEM0_BASEADDR" "C_MEM0_HIGHADDR" 
}
