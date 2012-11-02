###############################################################
# Copyright (c) 2004 - 2008 Xilinx, Inc. All Rights Reserved.
# You may copy and modify these files for your own internal use solely with
# Xilinx programmable logic devices and  Xilinx EDK system or create IP
# modules solely for Xilinx programmable logic devices and Xilinx EDK system.
# No rights are granted to distribute any files unless they are distributed in
# Xilinx programmable logic devices.
###############################################################
#uses "xillib.tcl"

proc generate {drv_handle} {
    xdefine_include_file $drv_handle "xparameters.h" "XSpi" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" "C_FIFO_EXIST" "C_SPI_SLAVE_ONLY" "C_NUM_SS_BITS" "C_NUM_TRANSFER_BITS"

    xdefine_config_file $drv_handle "xspi_g.c" "XSpi"  "DEVICE_ID" "C_BASEADDR" "C_FIFO_EXIST" "C_SPI_SLAVE_ONLY" "C_NUM_SS_BITS" "C_NUM_TRANSFER_BITS"

    xdefine_canonical_xpars $drv_handle "xparameters.h" "Spi" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" "C_FIFO_EXIST" "C_SPI_SLAVE_ONLY" "C_NUM_SS_BITS" "C_NUM_TRANSFER_BITS"

}


#
# Given a list of arguments, define each as a canonical constant name, using
# the driver name, in an include file.
#
proc xdefine_canonical_xpars {drv_handle file_name drv_string args} {
    # Open include file
    set file_handle [xopen_include_file $file_name]

    # Get all peripherals connected to this driver
    set periphs [xget_periphs $drv_handle]

    # Print canonical parameters for each peripheral
    set device_id 0
    foreach periph $periphs {
        puts $file_handle ""
        set periph_name [string toupper [xget_hw_name $periph]]
        set canonical_name [format "%s_%s" $drv_string $device_id]

        # Make sure canonical name is not the same as hardware instance
        if { [string compare -nocase $canonical_name $periph_name] == 0 } {
            # Abort canonical names
            close $file_handle
            return
        }

        puts $file_handle "/* Canonical definitions for peripheral $periph_name */"

        foreach arg $args {

            set lvalue [xget_dname $canonical_name $arg]

# The commented out rvalue is the name of the instance-specific constant
#           set rvalue [xget_name $periph $arg]

            # The rvalue set below is the actual value of the parameter
            set rvalue [xget_param_value $periph $arg]
            if {[llength $rvalue] == 0} {
                set rvalue 0
            }
            set rvalue [xformat_addr_string $rvalue $arg]

            puts $file_handle "#define $lvalue $rvalue"

        }
        incr device_id
        puts $file_handle ""
    }
    puts $file_handle "\n/******************************************************************/\n"
    close $file_handle
}

