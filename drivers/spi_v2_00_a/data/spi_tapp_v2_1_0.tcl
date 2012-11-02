###############################################################
# Copyright (c) 2005, 2006 Xilinx, Inc. All Rights Reserved.
# You may copy and modify these files for your own internal use solely with 
# Xilinx programmable logic devices and  Xilinx EDK system or create IP 
# modules solely for Xilinx programmable logic devices and Xilinx EDK system. 
# No rights are granted to distribute any files unless they are distributed in 
# Xilinx programmable logic devices.
###############################################################

## @BEGIN_CHANGELOG EDK_Im_SP2
##
##  - Added Interrupt support 
##
## @END_CHANGELOG

## @BEGIN_CHANGELOG EDK_I
##
##  - include header files
##
## @END_CHANGELOG

## @BEGIN_CHANGELOG EDK_H
##
##  - Added support for generation of multiple applications.
##    All TCL procedures are now required to have a software
##    project type as its first argument
##    
## @END_CHANGELOG

# Uses $XILINX_EDK/bin/lib/xillib_sw.tcl

# -----------------------------------------------------------------
# Software Project Types (swproj):
#   0 : MemoryTest - Calls basic  memorytest routines from common driver dir
#   1 : PeripheralTest - Calls any existing polled_example and/or selftest
# -----------------------------------------------------------------

# -----------------------------------------------------------------
# TCL Procedures:
# -----------------------------------------------------------------

proc gen_include_files {swproj mhsinst} {
  if {$swproj == 0} {
    return ""
  }
  if {$swproj == 1} {
    set spi_intr [get_intr $mhsinst]
      if { ${spi_intr} == 1} {
          set inc_file_lines {xspi.h spi_header.h spi_intr_header.h}    
      } else {
          set inc_file_lines {xspi.h spi_header.h}    
      }

    return $inc_file_lines
  }
}

proc gen_src_files {swproj mhsinst} {
  if {$swproj == 0} {
    return ""
  }
  if {$swproj == 1} {
      set spi_intr [get_intr $mhsinst]
      if { ${spi_intr} == 1} {
          set inc_file_lines {examples/xspi_selftest_example.c examples/xspi_intr_example.c data/spi_header.h data/spi_intr_header.h}
      } else {
          set inc_file_lines {examples/xspi_selftest_example.c data/spi_header.h}
      }

    return $inc_file_lines
  }
}

proc gen_testfunc_def {swproj mhsinst} {
  return ""
}

proc gen_init_code {swproj mhsinst} {
    if {$swproj == 0} {
        return ""
    }
    if {$swproj == 1} {
        set spi_intr [get_intr $mhsinst]
        set ipname [xget_value $mhsinst "NAME"]
        
        if { ${spi_intr} == 1} {
            set decl "   static XSpi ${ipname}_Spi;"
            set inc_file_lines $decl
            return $inc_file_lines
        } else {
            return ""
        }
    }
}

proc gen_testfunc_call {swproj mhsinst} {

  if {$swproj == 0} {
    return ""
  }
 
  set ipname [xget_value $mhsinst "NAME"] 
  set deviceid [xget_name $mhsinst "DEVICE_ID"]
  set hasStdout [xhas_stdout $mhsinst]
  set spi_intr [get_intr $mhsinst]
  
  if { ${spi_intr} == 1} {
        set mhsHandle [xget_hw_parent_handle $mhsinst]
        set retMhsInst [xget_intc $mhsHandle] 
        set intcname [xget_value $retMhsInst "NAME"]
        set intcvar [xget_intc_drv_var]
    }

  set testfunc_call ""

  if {${hasStdout} == 0} {

      append testfunc_call "

   {
      XStatus status;
                  
      status = SpiSelfTestExample(${deviceid});

   }"
      if { ${spi_intr} == 1} {
          set intr_id "XPAR_${intcname}_${ipname}_IP2INTC_IRPT_INTR"
          set intr_id [string toupper $intr_id]
          append testfunc_call "
    {
       XStatus Status;
	
       Status = SpiIntrExample(&${intcvar}, &${ipname}_Spi, \\
                                ${deviceid}, \\
                                ${intr_id});
    }"  
        }
      
      
  } else {

      append testfunc_call "

   {
      XStatus status;
      
      print(\"\\r\\n Runnning SpiSelfTestExample() for ${ipname}...\\r\\n\");
      
      status = SpiSelfTestExample(${deviceid});
      
      if (status == 0) {
         print(\"SpiSelfTestExample PASSED\\r\\n\");
      }
      else {
         print(\"SpiSelfTestExample FAILED\\r\\n\");
      }
   }"
      if { ${spi_intr} == 1} {
          set intr_id "XPAR_${intcname}_${ipname}_IP2INTC_IRPT_INTR"
          set intr_id [string toupper $intr_id]
          append testfunc_call "
    {
       XStatus Status;
	
       print(\"\\r\\n Running Interrupt Test for ${ipname}...\\r\\n\");

       Status = SpiIntrExample(&${intcvar}, &${ipname}_Spi, \\
                                ${deviceid}, \\
                                ${intr_id});
      if (Status == 0) {
         print(\"Spi Interrupt Test PASSED\\r\\n\");
      } 
      else {
         print(\"Spi Interrupt Test FAILED\\r\\n\");
      }

    }"  
      }
  }
  
  return $testfunc_call
}


proc get_intr {mhsinst} {
    
    set param_intr [xget_value $mhsinst "PARAMETER" "C_INTERRUPT_PRESENT"]
    set ipname [xget_value $mhsinst "NAME"]
    
    if {$param_intr == 1} {
        set mhsHandle [xget_hw_parent_handle $mhsinst]
        set sinkHandle [xget_hw_connected_ports_handle $mhsHandle "${ipname}_IP2INTC_Irpt" "sink"]
        if {$sinkHandle != ""} {
            set intcHandle [xget_hw_parent_handle $sinkHandle]
            set irqValue [xget_hw_port_value $intcHandle "Irq"] 
            if {$irqValue != ""} {
                set procSinkHandle [xget_hw_connected_ports_handle $mhsHandle $irqValue "sink"]
                if {$procSinkHandle != ""} {
                    set procSinkName [xget_hw_name $procSinkHandle]
                    set procVisiblePPC440 [string match $procSinkName "EICC440EXTIRQ"]
                    set procVisiblePPC [string match $procSinkName "EICC405EXTINPUTIRQ"]
                    set procVisibleMB [string match $procSinkName "INTERRUPT"]                        
                    set procVisible [expr $procVisiblePPC || $procVisibleMB || $procVisiblePPC440]
                    if {$procVisible == 1} {
                        set param_intr 1
                        return $param_intr
                    }
                }
            }
        }
    }
    set param_intr 0
    return $param_intr
}
