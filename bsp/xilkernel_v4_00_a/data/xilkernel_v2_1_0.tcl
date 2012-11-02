################################################################
# Copyright (c) 2004 Xilinx, Inc. All Rights Reserved.
# You may copy and modify these files for your own internal use solely with
# Xilinx programmable logic devices and  Xilinx EDK system or create IP
# modules solely for Xilinx programmable logic devices and Xilinx EDK system.
# No rights are granted to distribute any files unless they are distributed in
# Xilinx programmable logic devices. 
###############################################################
#
# This file is part of Xilkernel.
#
# $Id: xilkernel_v2_1_0.tcl,v 1.1.4.4 2007/11/06 20:04:36 vasanth Exp $
###############################################################################

proc kernel_drc {os_handle} {
    set sw_proc_handle [xget_libgen_proc_handle]
    set hw_proc_handle [xget_handle $sw_proc_handle "IPINST"]
    set proctype [xget_value $hw_proc_handle "OPTION" "IPNAME"]
    set compiler [xget_value $sw_proc_handle "PARAMETER" "COMPILER"]

    # System timer frequency must be specified
    set systmr_spec [xget_value $os_handle "PARAMETER" "systmr_spec"]
    if { $systmr_spec == "false" }  {
        error "Xilkernel requires some or all of the parameters in the systmr_spec category to be defined. Please configure this categry as 'true'." "" "mdt_error"
    } 
        
    switch -regexp $proctype {
	"microblaze" { 
            if { [string first "mb-gcc" $compiler] == 0 && [string first "mb-g++" $compiler] == 0} {
                error "Wrong compiler requested. Xilkernel can be compiled only with the GNU compiler for MicroBlaze." "" "mdt_error"
            }

            set systmr_dev [xget_value $os_handle "PARAMETER" "systmr_dev"]
            if { $systmr_dev == "none" } {
                error "Xilkernel for Microblaze requires a system timer device to be specified. Please choose a valid peripheral instance in the systmr_dev parameter." "" "mdt_error"
            }
            set systmr_handle [xget_sw_ipinst_handle_from_processor $sw_proc_handle $systmr_dev]
            set systmr_type [xget_value $systmr_handle "OPTION" "IPNAME"]
            if { $systmr_type != "fit_timer" && $systmr_type != "opb_timer" && $systmr_type != "xps_timer" } {
                error "Xilkernel for Microblaze can work only with xps_timer, opb_timer or fit_timer. Please choose a valid device as the system timer with the parameter systmr_dev." "" "mdt_error"
            }
            
            set procver [xget_value $hw_proc_handle "PARAMETER" "HW_VER"]
	}
	ppc* {
	    if { $compiler != "powerpc-eabi-gcc" && $compiler != "powerpc-eabi-g++" } {
		error "Wrong compiler requested. Xilkernel can be compiled only with the GNU compiler for PPC." "" "mdt_error"
	    }

	}
	"default" {
		error "Unsupported processor type: $proctype. Xilkernel supported only for PPC and Microblaze." "" "mdt_error"
	}
    }

    set systmr_freq [xget_value $os_handle "PARAMETER" "systmr_freq"]
    if { $systmr_freq == -1 } {
        error "System timer frequency not specified." "" "mdt_error"
    }

    set systmr_interval_ms [xget_value $os_handle "PARAMETER" "systmr_interval"]    
    if { $systmr_interval_ms <= 0 } {
        error "Invalid value for parameter systmr_interval specified. Please specify a positive value." "" "mdt_error"
    }

    set config_bufmalloc [xget_value $os_handle "PARAMETER" "config_bufmalloc"]    
    if { $config_bufmalloc == "true" } {
        set memtable_handle [xget_handle $os_handle "ARRAY" "mem_table"]
        set memtable_elements [xget_handle $memtable_handle "ELEMENTS" "*"]
        foreach ele $memtable_elements {
            set bsiz  [xget_value $ele "PARAMETER" "mem_bsize"]
            set nblks [xget_value $ele "PARAMETER" "mem_bsize"]
            if { $bsiz < 4 } {
                error "mem_table mem_bsize specification of $bsiz is incorrect. Block size should be >= 4." "" "mdt_error"
            }
            if { $nblks <= 0 } {
                error "mem_table mem_nblks specification of $nblks is incorrect. Block count should be positive." "" "mdt_error"
            }
        }
    }

    set config_msgq [xget_value $os_handle "PARAMETER" "config_msgq"]
    if { $config_msgq == "true" } {
        set use_malloc [xget_value $os_handle "PARAMETER" "use_malloc"]
        if { $use_malloc != "true" && $config_bufmalloc != "true" } {
            error "Message queues require memory allocation support. Please configure parameters config_bufmalloc or use_malloc to be true." "" "mdt_error"
        }
    }
}

proc generate {os_handle} {
    set sw_proc_handle [xget_libgen_proc_handle]
    set hw_proc_handle [xget_handle $sw_proc_handle "IPINST"]
    set proctype [xget_value $hw_proc_handle "OPTION" "IPNAME"]
    set procver [xget_value $hw_proc_handle "PARAMETER" "HW_VER"]
    
    set need_config_file "false"

    # proctype should be "microblaze" or "ppc405_*" or "ppc440_*"
    set mbsrcdir  "../standalone_v2_00_a/src/microblaze"
    set ppcsrcdir "../standalone_v2_00_a/src/ppc405"
    set ppc440srcdir "../standalone_v2_00_a/src/ppc440"
    set datadir   "../standalone_v2_00_a/data"


    # proctype should be "microblaze" or "ppc405_*" or "ppc440_*"
    switch -regexp $proctype {
	"microblaze" { 

	    file copy -force "./src/Makefile_mb.sh" "./src/Makefile" 
            foreach entry [glob -nocomplain [file join $mbsrcdir *]] {
		if { [string first "hw_exception_handler" $entry] == -1 } { ;# Do not copy over the Standalone BSP exception handler
		    file copy -force $entry [file join ".." "standalone_v2_00_a" "src"]
		}
            }
	    set need_config_file "true"
	}
	ppc*  {
	    file copy -force "./src/Makefile_ppc.sh" "./src/Makefile"

            # Write the arch.make file
            set makecpu [open "./src/cpu.make" w]  
            xprint_generated_header_tcl $makecpu "Configuration parameters for PPC Xilkernel Makefile"
            if { [string match -nocase ppc440* $proctype] } {
                puts $makecpu "CPU_TYPE=440"
            } else {
                puts $makecpu "CPU_TYPE=405"                
            }
            close $makecpu

            if { [string match -nocase ppc440* $proctype] } {
                set ppcsrcdir $ppc440srcdir
            }

	    foreach entry [glob -nocomplain [file join $ppcsrcdir *]] {
                if { [string first "xvectors" $entry] == -1 } {      ;# Do not copy xvectors.S. Xilkernel provides its own.
                    file copy -force $entry [file join ".." "standalone_v2_00_a" "src"]                
                }
	    }
	}
	"default" {puts "unknown processor type $proctype\n"}
    }

    # Write the config.make file
    set makeconfig [open "../standalone_v2_00_a/src/config.make" w]  
    xprint_generated_header_tcl $makeconfig "Configuration parameters for Standalone Makefile"

    if { $proctype == "microblaze" } {
	if { [mb_has_exceptions $hw_proc_handle] } {
	    puts $makeconfig "LIBSOURCES = *.s *.c *.S"
	} else {
	    puts $makeconfig "LIBSOURCES = *.s *.c"
	}
    }
    puts $makeconfig "LIBS = standalone_libs"
    close $makeconfig
   
    # Remove microblaze and ppc directories...
    file delete -force $mbsrcdir
    file delete -force $ppcsrcdir
    file delete -force $datadir

    # Handle stdin and stdout
    xhandle_stdin $os_handle
    xhandle_stdout $os_handle

    # Create config file for microblaze interrupt handling
    if {[string compare -nocase $need_config_file "true"] == 0} {
	xhandle_mb_interrupts
    }

    # Create config files for Microblaze exception handling
    if { $proctype == "microblaze" && [mb_has_exceptions $hw_proc_handle] } {
        xcreate_mb_exc_config_file 
    }

    # Create bspconfig file
    set bspcfg_fn [file join ".." "standalone_v2_00_a" "src"  "bspconfig.h"] 
    file delete $bspcfg_fn
    set bspcfg_fh [open $bspcfg_fn w]
    xprint_generated_header $bspcfg_fh "Configurations for Standalone BSP"

    if { $proctype == "microblaze" && [mb_has_pvr $hw_proc_handle] } {
        
        set pvr [xget_value $hw_proc_handle "PARAMETER" "C_PVR"]
        
        switch $pvr {
            "0" {
                puts $bspcfg_fh "#define MICROBLAZE_PVR_NONE"
            }
            "1" {
                puts $bspcfg_fh "#define MICROBLAZE_PVR_BASIC"
            }
            "2" {
                puts $bspcfg_fh "#define MICROBLAZE_PVR_FULL"
            }
            "default" {
                puts $bspcfg_fh "#define MICROBLAZE_PVR_NONE"
            }
        }    
    }

    close $bspcfg_fh

    set config_file [xopen_new_include_file "./src/include/os_config.h" "XilKernel Configuration parameters"]
    set init_file [xopen_new_include_file  "./src/include/config/config_init.h" "XilKernel Configuration parameters"]
    xprint_generated_header $init_file "LibXilKernel Initialization structures"
    puts $init_file "\#include <sys/init.h>"
    puts $init_file "\#include <os_config.h>\n\n"

    switch -regexp $proctype {
	"microblaze" { 
	    xput_define $config_file "MB_XILKERNEL" ""
            if {[mb_has_exceptions $hw_proc_handle] } {
                xput_define $config_file "CONFIG_HARDWARE_EXCEPTIONS" ""
            }
	}
	ppc* {
	    xput_define $config_file "PPC_XILKERNEL" ""
            if { [string match -nocase ppc440* $proctype] } {
                xput_define $config_file "PPC_CPU_440" ""
            }
        }
    }
    
    # Create defines and struct initializations

    set config_debug_support [xget_value $os_handle "PARAMETER" "config_debug_support"]
    if { $config_debug_support == "true" } {
        set config_debug_mode [xget_value $os_handle "PARAMETER" "verbose"]
        if { $config_debug_mode == "true" } {
            xadd_define $config_file $os_handle "verbose"
        }

        ;# set config_debug_mon [xget_value $os_handle "PARAMETER" "debug_mon"]
        ;# if { $config_debug_mon == "true" } {
        ;# xadd_define $config_file $os_handle "config_debugmon"
        ;# }
    }

    set config_enhanced_features [xget_value $os_handle "PARAMETER" "enhanced_features"]
    if { $config_enhanced_features == "true" } {
	set config_kill [xget_value $os_handle "PARAMETER" "config_kill"]
	if { $config_kill == "true" } {
	    xadd_define $config_file $os_handle "config_kill"
	}
	
	set config_yield [xget_value $os_handle "PARAMETER" "config_yield"]
	if { $config_yield == "true" } {
	    xadd_define $config_file $os_handle "config_yield"
	}
    }

    set config_elf_process [xget_value $os_handle "PARAMETER" "config_elf_process"]
    if { $config_elf_process == "true" } {
        xadd_define $config_file $os_handle "config_elf_process"
        xadd_define $config_file $os_handle "max_procs"
        
        # Get the Entry Point address, priority for static ELF processes table
        set static_elf_process_table_handle [xget_handle $os_handle "ARRAY" "static_elf_process_table"]	
        if { $static_elf_process_table_handle != "" } {
            set n_init_process [xget_value $static_elf_process_table_handle "PROPERTY" "size"]
            xput_define $config_file "n_init_process" $n_init_process
            xadd_define $config_file $os_handle "config_static_elf_process_support"
            xadd_struct $init_file $os_handle "_process_init" "se_process_table" "static_elf_process_table" "process_start_addr" "process_prio"
        } 
    } 
    
    set config_pthread [xget_value $os_handle "PARAMETER" "config_pthread_support"]
    if { $config_pthread == "true" } {
	xadd_define $config_file $os_handle "config_pthread_support"
	xadd_define $config_file $os_handle "max_pthreads"
	xadd_define $config_file $os_handle "pthread_stack_size"
	
	set static_pthread_table_handle [xget_handle $os_handle "ARRAY" "static_pthread_table"]	
	if { $static_pthread_table_handle != "" } {
	    set n_init_self_pthreads [xget_value $static_pthread_table_handle "PROPERTY" "size"]
	    xput_define $config_file "config_static_pthread_support" "true"
	    xput_define $config_file "n_init_self_pthreads" $n_init_self_pthreads
	    xadd_extern_fname $init_file $os_handle "static_pthread_table" "pthread_start_func"
	    xadd_struct $init_file $os_handle "_elf_pthread_init" "kb_pthread_table" "static_pthread_table" "pthread_start_func" "pthread_prio"
	}

	set config_pthread_mutex [xget_value $os_handle "PARAMETER" "config_pthread_mutex"]
	if { $config_pthread_mutex == "true" } {
	    xadd_define $config_file $os_handle "config_pthread_mutex"
	    xadd_define $config_file $os_handle "max_pthread_mutex"
	    set max_pthread_mutex_waitq [xget_value $os_handle "PARAMETER" "max_pthread_mutex_waitq"]
	    xput_define $config_file "max_pthread_mutex_waitq" $max_pthread_mutex_waitq
	}
    }

    # System timer configuration (Microblaze only. kernel DRC ensures this)
    set systmr_spec [xget_value $os_handle "PARAMETER" "systmr_spec"]
    if { $proctype == "microblaze" } { 
        set systmr_dev [xget_value $os_handle "PARAMETER" "systmr_dev"]
        set systmr_handle [xget_sw_ipinst_handle_from_processor $sw_proc_handle $systmr_dev]
        set systmr_type [xget_value $systmr_handle "OPTION" "IPNAME"]
    }

    if { $proctype == "microblaze" } {
        if { $systmr_type == "opb_timer" || $systmr_type == "xps_timer" } {
            xput_define $config_file "CONFIG_TIMER_PIT" "true"
            set systmr_baseaddr [xget_value $systmr_handle "PARAMETER" "C_BASEADDR"]
            xput_define $config_file "systmr_baseaddr" $systmr_baseaddr
        }
    }

    set systmr_freq [xget_value $os_handle "PARAMETER" "systmr_freq"]
    if { $proctype == "ppc405" || $proctype == "ppc405_virtex4" || $proctype == "ppc440_virtex5" } {
        set systmr_interval_ms [xget_value $os_handle "PARAMETER" "systmr_interval"]
        set systmr_interval [expr [expr double($systmr_freq) * double($systmr_interval_ms)] / 1000]
        set systmr_interval [expr int($systmr_interval)]
    } elseif {$proctype == "microblaze" && [expr {$systmr_type == "opb_timer" || $systmr_type == "xps_timer"}]} {
        set systmr_interval_ms [xget_value $os_handle "PARAMETER" "systmr_interval"]
        set systmr_interval [expr [expr double($systmr_freq) * double($systmr_interval_ms)] / 1000]
        set systmr_interval [expr int($systmr_interval)]
    } elseif { $proctype == "microblaze" && $systmr_type == "fit_timer" } {
        set systmr_interval [xget_value $systmr_handle "PARAMETER" "C_NO_CLOCKS"]
    }
    xput_define $config_file "SYSTMR_INTERVAL" $systmr_interval
    xput_define $config_file "SYSTMR_CLK_FREQ" $systmr_freq
    xput_define $config_file "SYSTMR_CLK_FREQ_KHZ" [expr $systmr_freq / 1000]
    
    # Interrupt controller configuration
    set sysintc_spec [xget_value $os_handle "PARAMETER" "sysintc_spec"]
    if { $sysintc_spec != "none" } {
	xput_define $config_file "CONFIG_INTC" "true"
	set sysintc_dev_handle [xget_sw_ipinst_handle_from_processor $sw_proc_handle $sysintc_spec]
	set sysintc_baseaddr [xget_name $sysintc_dev_handle "C_BASEADDR"]
	set sysintc_device_id [xget_name $sysintc_dev_handle "DEVICE_ID"]
	xput_define $config_file "sysintc_baseaddr" $sysintc_baseaddr
	xput_define $config_file "sysintc_device_id" $sysintc_device_id
	
	# Additionally for microblaze, figure out which interrupt 
	# input is the system timer interrupt and define its ID
	if { $proctype == "microblaze" } {
	    set systmr_intr [xget_value $systmr_handle "PORT" "Interrupt"]
	    if { [string compare -nocase $systmr_intr ""] == 0 } {
                error "System Timer Interrupt PORT is not specified" "" "mdt_error"
	    }
	    set mhs_handle [xget_handle $systmr_handle "parent"]
	    set intr_ports [xget_connected_ports_handle $mhs_handle $systmr_intr "sink"]
            foreach intr_port $intr_ports {
                set intr_port_type [xget_port_type $intr_port]
                if { [string compare -nocase $intr_port_type "global"] == 0 } {
                    continue
                }

                set intc_handle [xget_handle $intr_port "parent"]
                set intc_name [xget_value $intc_handle "name"]
                set proc_intc_handle [xget_sw_ipinst_handle_from_processor $sw_proc_handle $intc_name]
                if { [string compare -nocase $sysintc_dev_handle $intc_handle] != 0 } {
                    continue
                }

                set num_intr_inputs [xget_value $sysintc_dev_handle "PARAMETER" "C_NUM_INTR_INPUTS"]
                set signals [split [xget_value $intr_port "VALUE"] "&"]
                set i 1
                foreach signal $signals {
                    set signal [string trim $signal]
                    if {[string compare -nocase $signal $systmr_intr] == 0} {
                        puts $config_file "\#define SYSTMR_INTR_ID [expr ($num_intr_inputs - $i)]\n"
                        break
                    }
                    incr i
                }
            }
	}
    }

   
    set config_sched [xget_value $os_handle "PARAMETER" "config_sched"]
    if { $config_sched == "true" } {
	xadd_define $config_file $os_handle "config_sched"
	xadd_define $config_file $os_handle "sched_type"
	xadd_define $config_file $os_handle "n_prio"
        set sched_type [xget_value $os_handle "PARAMETER" "sched_type"]
        if { $sched_type == "SCHED_PRIO" } {
            xput_define $config_file "config_priosched" "true"
        } else {
            xput_define $config_file "config_rrsched" "true"
        }
	xadd_define $config_file $os_handle "max_readyq"
    }
    
    set config_sema [xget_value $os_handle "PARAMETER" "config_sema"]
    if { $config_sema == "true" } {
	xadd_define $config_file $os_handle "config_sema"
	xadd_define $config_file $os_handle "max_sem"
	xadd_define $config_file $os_handle "max_sem_waitq"
        set config_named_sema [xget_value $os_handle "PARAMETER" "config_named_sema"]
        if { $config_named_sema == "true" } {
            xadd_define $config_file $os_handle "config_named_sema"
        }
    }

    set config_msgq [xget_value $os_handle "PARAMETER" "config_msgq"]
    if { $config_msgq == "true" } {
	set num_msgqs [xget_value $os_handle "PARAMETER" "num_msgqs"]
	set msgq_capacity [xget_value $os_handle "PARAMETER" "msgq_capacity"]
        set use_malloc [xget_value $os_handle "PARAMETER" "use_malloc"]
	xadd_define $config_file $os_handle "config_msgq"
	xadd_define $config_file $os_handle "num_msgqs"
	xadd_define $config_file $os_handle "msgq_capacity"
        if { $use_malloc == "true" } {
            xput_define $config_file "config_enhanced_msgq" "true"
            xput_define $config_file "use_malloc"   "true"
        }
    } 
	    
    set config_shm [xget_value $os_handle "PARAMETER" "config_shm"]
    if { $config_shm == "true" } {
	xadd_define $config_file $os_handle "config_shm"
	set shm_handle [xget_handle $os_handle "ARRAY" "shm_table"] 
	if { $shm_handle == "" } {
	    error "SHM configuration needs shm_table specification." "" "mdt_error"
	}
	set n_shm [llength [xget_handle $shm_handle "ELEMENTS" "*"]]
	xput_define $config_file "n_shm" $n_shm
	set shm_msize [get_field_sum $os_handle "shm_table"  "shm_size"]
	xput_define $config_file "shm_msize" $shm_msize
	xadd_struct $init_file $os_handle "_shm_init" "shm_config" "shm_table" "shm_size"
    }

    set config_bufmalloc [xget_value $os_handle "PARAMETER" "config_bufmalloc"]
    if { $config_bufmalloc == "true" } {
	xadd_define $config_file $os_handle "config_bufmalloc" 
        set memtable_handle [xget_handle $os_handle "ARRAY" "mem_table"]
        set memtable_elements [xget_handle $memtable_handle "ELEMENTS" "*"]
	set n_static_bufs [llength $memtable_elements]
        set max_bufs [xget_value $os_handle "PARAMETER" "max_bufs"]


        set bufmalloc_msize [get_field_product_sum $os_handle "mem_table"  "mem_bsize" "mem_nblks"]
        xput_define $config_file "bufmalloc_msize" $bufmalloc_msize
	xput_define $config_file "n_mbufs" [expr $max_bufs + $n_static_bufs]
	xput_define $config_file "n_static_bufs" $n_static_bufs
	xadd_struct $init_file $os_handle "bufmalloc_init_s" "bufmalloc_cfg" "mem_table" "mem_bsize" "mem_nblks"
    } 

    set config_time [xget_value $os_handle "PARAMETER" "config_time"]
    if { $config_time == "true" } {
    	xadd_define $config_file $os_handle "config_time"
	xadd_define $config_file $os_handle "max_tmrs"
    }

    #set config_stats [xget_value $os_handle "PARAMETER" "config_stats"]
    #if { $config_stats == "true" } { 
    #	xadd_define $config_file $os_handle "config_stats"
    #}

    # Handle I/O ranges for MicroBlaze MPU here
    if { $proctype == "microblaze" } {
        set mhs_handle [xget_handle $hw_proc_handle "parent"]
        set mmu [xget_value $hw_proc_handle "PARAMETER" "C_USE_MMU"]
        if { $mmu >= 2 } {

            # Enumerate all the I/O ranges into a structure
            set interconnect [xget_value $hw_proc_handle "PARAMETER" "C_INTERCONNECT"]
            if { $interconnect == "" || $interconnect == 0 } {
                set dbus_if_name "DOPB"
            } else {
                set dbus_if_name "DPLB"
            }
            
            set dbus_name [xget_hw_busif_value $hw_proc_handle $dbus_if_name]
            set dbus_handle [xget_hw_ipinst_handle $mhs_handle $dbus_name]
            set addrlist [xget_hw_bus_slave_addrpairs $dbus_handle]
            
            # Get the list of memory controllers in the mhs. We want to filter
            # "memories" from the above addrlist
            set memcon_handles [xget_hw_memory_controller_handles $mhs_handle]
            set n_ioranges 0
            foreach {base high} $addrlist {               
                set skip 0
                foreach {memcon_handle} $memcon_handles {
                    set memcon_addrlist [xget_addr_values_list_for_ipinst $memcon_handle]
                    foreach {mc_base mc_high} $memcon_addrlist {
                        if {$mc_base == $base && $mc_high == $high} {
                            set skip 1
                        }
                    }
                }
                
                if {$skip == 0} {
                    lappend io_addrlist $base $high
                    incr n_ioranges
                }
            }
            
            puts $config_file "#define XILKERNEL_IO_NRANGES $n_ioranges"
            puts $init_file "xilkernel_io_range_t system_io_range\[XILKERNEL_IO_NRANGES\] = \{"
            
            set count 0
            foreach {base high} $io_addrlist {
                set base [format "0x%X" $base]
                set high [format "0x%X" $high]
                incr count
                puts -nonewline $init_file "\t\{$base, $high, MPU_PROT_READWRITE"
                if {$count < $n_ioranges} {
                    puts $init_file "\},"
                } else {
                    puts $init_file "\}"
                }
            }
            puts $init_file "\}\;"
        }
    }
    
    # complete the header protectors
    puts $config_file "\#endif"
    close $config_file
    puts $init_file "\#endif"
    close $init_file
}


proc execs_generate {os_handle} {
    puts "Copying include files and libs"
    # copy the BSP and any previously generated libs and includes to kernel area
    # This allows the kernel make files to limit all include paths to within the kernel dir
    # and consequently, allows the kernel directory to be copied out to any user 
    # specified area where the user can independently edit and build the
    # kernel and related execs using make files
    exec cp -r ../../include .
    set copyoutfiles [xget_value $os_handle "PARAMETER" "copyoutfiles"]
    if {$copyoutfiles == "true"} {
        set copyfromdir "."
	set copytodir [xget_value $os_handle "PARAMETER" "copytodir"]
	puts "copying from $copyfromdir to $copytodir"
	if [file exists $copytodir] {
	    puts "WARNING: removing old version of $copytodir"
	    file delete -force $copytodir
	}
	exec cp -r $copyfromdir $copytodir
    }
}

proc xopen_new_include_file { filename description } {
    set inc_file [open $filename w]
    xprint_generated_header $inc_file $description
    set newfname [string map {. _} [lindex [split $filename {\/}] end]]
    puts $inc_file "\#ifndef _[string toupper $newfname]"
    puts $inc_file "\#define _[string toupper $newfname]\n\n"
    return $inc_file
}

proc xadd_define { config_file os_handle parameter } {
    set param_value [xget_value $os_handle "PARAMETER" $parameter]
    puts $config_file "#define [string toupper $parameter] $param_value\n"

    # puts "creating #define [string toupper $parameter] $param_value\n"
}

proc xput_define { config_file parameter param_value } {
    puts $config_file "#define [string toupper $parameter] $param_value\n"

    # puts "creating #define [string toupper $parameter] $param_value\n"
}


# args field of the array
proc xadd_extern_fname {initfile oshandle arrayname arg} { 

    set arrhandle [xget_handle $oshandle "ARRAY" $arrayname]
    set elements [xget_handle $arrhandle "ELEMENTS" "*"]
    set count 0
    set max_count [llength $elements]

    foreach ele $elements {
	incr count
	set arg_value [xget_value $ele "PARAMETER" $arg]
	puts $initfile "extern void $arg_value\(\)\;"
    }
    puts $initfile ""
}

# args is variable no - fields of the array
proc xadd_struct {initfile oshandle structtype structname arrayname args} { 

    set arrhandle [xget_handle $oshandle "ARRAY" $arrayname]
    set elements [xget_handle $arrhandle "ELEMENTS" "*"]
    set count 0
    set max_count [llength $elements]
    puts $initfile "struct $structtype $structname\[$max_count\] = \{"

    foreach ele $elements {
	incr count
	puts -nonewline $initfile "\t\{"
	foreach field $args {
	    set field_value [xget_value $ele "PARAMETER" $field]
	    # puts "$arrayname ( $count )->$field is $field_value"
	    puts -nonewline $initfile "$field_value"
	    if { $field != [lindex $args end] } {
		puts -nonewline $initfile ","
	    }
	}
	if {$count < $max_count} {
	    puts $initfile "\},"
	} else {
	    puts $initfile "\}"
	}
    }
    puts $initfile "\}\;"
}


# return the sum of all the arg field values in arrayname
proc get_field_sum {oshandle arrayname arg} { 

    set arrhandle [xget_handle $oshandle "ARRAY" $arrayname]
    set elements [xget_handle $arrhandle "ELEMENTS" "*"]
    set count 0
    set max_count [llength $elements]
  
    foreach ele $elements {
	set field_value [xget_value $ele "PARAMETER" $arg]
	set count [expr $field_value+$count]
    }
    return $count
}

# return the sum of the product of field values in arrayname
proc get_field_product_sum {oshandle arrayname field1 field2} { 

    set arrhandle [xget_handle $oshandle "ARRAY" $arrayname]
    set elements [xget_handle $arrhandle "ELEMENTS" "*"]
    set count 0
    set max_count [llength $elements]
  
    foreach ele $elements {
	set field1_value [xget_value $ele "PARAMETER" $field1]
	set field2_value [xget_value $ele "PARAMETER" $field2]
	set incr_value [expr $field1_value*$field2_value]
	set count [expr $count+$incr_value]
    }
    return $count
}

proc xhandle_mb_interrupts {} {

    set default_interrupt_handler "XNullHandler"
    set default_arg "XNULL"

    set source_interrupt_handler $default_interrupt_handler
    set source_handler_arg $default_arg
    
    # Handle the interrupt pin
    set sw_proc_handle [xget_libgen_proc_handle] 
    set periph [xget_handle $sw_proc_handle "IPINST"]
    set source_ports [xget_interrupt_sources $periph]
    if {[llength $source_ports] > 1} {
	error "Too many interrupting ports on the MicroBlaze.  Should only find 1" "" "libgen_error"
	return
    }
    
    if {[llength $source_ports] == 1} {
	set source_port [lindex $source_ports 0]
	if {[llength $source_port] != 0} {
	    set source_port_name [xget_value $source_port "VALUE"]	
	    set source_periph [xget_handle $source_port "PARENT"]
	    set source_name [xget_value $source_periph "NAME"]
  	    set source_driver [xget_sw_driver_handle_for_ipinst $sw_proc_handle $source_name]

	    if {[string compare -nocase $source_driver ""] != 0} {
		set int_array [xget_handle $source_driver "ARRAY" "interrupt_handler"]
		if {[llength $int_array] != 0} {
		    set int_array_elems [xget_handle $int_array "ELEMENTS" "*"]
		    if {[llength $int_array_elems] != 0} {
			foreach int_array_elem $int_array_elems {
			    set int_port [xget_value $int_array_elem "PARAMETER" "int_port"]
			    set mhs_handle [xget_value $int_array_elem "PARAMETER" "mhsinst"]
			    if {[llength $int_port] != 0} {
				if {[string compare -nocase $int_port $source_port_name] == 0 && \
					$mhs_handle == $source_periph } {
				    set source_interrupt_handler [xget_value $int_array_elem "PARAMETER" "int_handler"]
				    set source_handler_arg [xget_value $int_array_elem "PARAMETER" "int_handler_arg"]
				    if {[string compare -nocase $source_handler_arg DEVICE_ID] == 0 } {
					set source_handler_arg [xget_name $source_periph "DEVICE_ID"]
				    } else {
					if {[string compare -nocase "global" [xget_port_type $source_port]] == 0} {
					    set source_handler_arg $default_arg
					} else {
					    set source_handler_arg [xget_name $source_periph "C_BASEADDR"]
					}
				    }
				    break
				}
			    }
			}
		    }
		}
	    }
	}
    }
    
    # Generate microblaze_interrupts_g.c file...
    xcreate_mb_intr_config_file $source_interrupt_handler $source_handler_arg
    
}


proc xcreate_mb_intr_config_file {handler arg} {
    
    set mb_table "MB_InterruptVectorTable"

    set filename [file join "../standalone_v2_00_a/src" "microblaze_interrupts_g.c"] 
    file delete $filename
    set config_file [open $filename w]

    xprint_generated_header $config_file "Interrupt Handler Table for MicroBlaze Processor"
    
    puts $config_file "#include \"microblaze_interrupts_i.h\""
    puts $config_file "#include \"xparameters.h\""
    puts $config_file "\n"
    puts $config_file [format "extern void %s (void *);" $handler]
    puts $config_file "\n/*"
    puts $config_file "* The interrupt handler table for microblaze processor"
    puts $config_file "*/\n"
    puts $config_file [format "%sEntry %s\[\] =" $mb_table $mb_table]
    puts $config_file "\{"
    puts -nonewline $config_file [format "\t%s" $handler]
    puts -nonewline $config_file [format ",\n\t(void*) %s" $arg]
    puts -nonewline $config_file "\n\};"
    puts $config_file "\n"
    close $config_file
}


# -------------------------------------------
# Tcl procedure xcreate_mb_exc_config file
# -------------------------------------------
proc xcreate_mb_exc_config_file { } {
    
    set hfilename [file join "src" "include" "microblaze_exceptions_g.h"] 
    file delete $hfilename
    set hconfig_file [open $hfilename w]

    xprint_generated_header $hconfig_file "Exception Handling Header for MicroBlaze Processor"
    
    puts $hconfig_file "\n"

    set sw_proc_handle [xget_libgen_proc_handle]
    set hw_proc_handle [xget_handle $sw_proc_handle "IPINST"]
    set procver [xget_value $hw_proc_handle "PARAMETER" "HW_VER"]

    if { ![mb_has_exceptions $hw_proc_handle]} { ;# NO exceptions are enabled
        close $hconfig_file              ;# Do not generate any info in either the header or the C file
        return
    }
    
    puts $hconfig_file "\#define MICROBLAZE_EXCEPTIONS_ENABLED 1"
    if { [mb_can_handle_exceptions_in_delay_slots $procver] } {
        puts $hconfig_file "#define MICROBLAZE_CAN_HANDLE_EXCEPTIONS_IN_DELAY_SLOTS"
    }

    close $hconfig_file
}

# --------------------------------------
# Tcl procedure post_generate
# This proc removes from libxil.a the basic 
# and standalone BSP versions of 
# _interrupt_handler and _hw_exception_handler
# routines
# --------------------------------------
proc post_generate {os_handle} {
    set sw_proc_handle [xget_libgen_proc_handle]
    set hw_proc_handle [xget_handle $sw_proc_handle "IPINST"]
    set proctype [xget_value $hw_proc_handle "OPTION" "IPNAME"]
    set procname [xget_value $hw_proc_handle "NAME"]

    set procdrv [xget_sw_driver_handle_for_ipinst $sw_proc_handle $procname]
    set archiver [xget_value $procdrv "PARAMETER" "archiver"]

    if {[string compare -nocase $proctype "microblaze"] == 0 } {
        # Remove _interrupt_handler.o from libxil.a for Xilkernel
        exec bash -c "$archiver -d ../../lib/libxil.a _interrupt_handler.o"
        # We have linkage problems due to how these platforms are defined. Can't do this right now.  
        # # Remove _exception_handler.o from libxil.a for Xilkernel
        # exec bash -c "$archiver -d ../../lib/libxil.a _exception_handler.o"
        
        # Remove _hw_exception_handler.o from libxil.a for microblaze cores with exception support
        # if {[mb_has_exceptions $hw_proc_handle]} {
        #     exec bash -c "$archiver -d ../../lib/libxil.a _hw_exception_handler.o"
        # }
    }
}

# --------------------------------------
# Tcl procedure execs_generate
# This proc removes from libxil.a all 
# the stuff that we are overriding
# with xilkernel
# We currently override,
#  MicroBlaze
#   - Dummy _interrupt_hander and _hw_exception_handler 
#     (in post_generate)
#  PPC
#   - xvectors.o; sleep.o (IF config_time is true)
#  Common to all processors
#    - errno.o
# --------------------------------------
proc execs_generate {os_handle} {
    set sw_proc_handle [xget_libgen_proc_handle]
    set hw_proc_handle [xget_handle $sw_proc_handle "IPINST"]
    set proctype [xget_value $hw_proc_handle "OPTION" "IPNAME"]
    set procname [xget_value $hw_proc_handle "NAME"]

    set procdrv [xget_sw_driver_handle_for_ipinst $sw_proc_handle $procname]
    # Remove _interrupt_handler.o from libxil.a for mb-gcc
    set archiver [xget_value $procdrv "PARAMETER" "archiver"]

    exec bash -c "$archiver -d ../../lib/libxil.a errno.o"

    if {[string compare -nocase $proctype "microblaze"] != 0 } { 
        # We have linkage problems due to how these platforms are defined. Can't do this right now.
        # exec bash -c "$archiver -d ../../lib/libxil.a xvectors.o"
        exec bash -c "$archiver -d ../../lib/libxil.a kill.o"
        set config_time [xget_value $os_handle "PARAMETER" "config_time"]
        if { $config_time == "true" } {
            exec bash -c "$archiver -d ../../lib/libxil.a sleep.o"            
        }
    } else {
        # We have linkage problems due to how these platforms are defined. Can't do this right now.  
        # exec bash -c "$archiver -d ../../lib/libxil.a microblaze_interrupt_handler.o"

        set config_enhanced_features [xget_value $os_handle "PARAMETER" "enhanced_features"]
        if { $config_enhanced_features == "true" } {
            set config_kill [xget_value $os_handle "PARAMETER" "config_kill"]
            if { $config_kill == "true" } {
                exec bash -c "$archiver -d ../../lib/libxil.a kill.o"
            } 
        }
    }
}


# --------------------------------------
# Return true if this MB has 
# exception handling support
# --------------------------------------
proc mb_has_exceptions { hw_proc_handle } {
   
    # Check if the following parameters exist on this MicroBlaze's MPD
    set ee [xget_value $hw_proc_handle "PARAMETER" "C_UNALIGNED_EXCEPTIONS"]
    if { $ee != "" } {
        return true
    }

    set ee [xget_value $hw_proc_handle "PARAMETER" "C_ILL_OPCODE_EXCEPTION"]
    if { $ee != "" } {
        return true
    }

    set ee [xget_value $hw_proc_handle "PARAMETER" "C_IOPB_BUS_EXCEPTION"]
    if { $ee != "" } {
        return true
    }

    set ee [xget_value $hw_proc_handle "PARAMETER" "C_DOPB_BUS_EXCEPTION"]
    if { $ee != "" } {
        return true
    }

    set ee [xget_value $hw_proc_handle "PARAMETER" "C_DIV_BY_ZERO_EXCEPTION"]
    if { $ee != "" } {
        return true
    } 

    set ee [xget_value $hw_proc_handle "PARAMETER" "C_DIV_ZERO_EXCEPTION"]
    if { $ee != "" } {
        return true
    } 

    set ee [xget_value $hw_proc_handle "PARAMETER" "C_FPU_EXCEPTION"]
    if { $ee != "" } {
        return true
    } 

    set ee [xget_value $hw_proc_handle "PARAMETER" "C_USE_MMU"]
    if { $ee != "" && $ee != 0 } {
        return true
    } 

    return false
}

# --------------------------------------
# Return true if this MB has 
# FPU exception handling support
# --------------------------------------
proc mb_has_fpu_exceptions { hw_proc_handle } {
    
    # Check if the following parameters exist on this MicroBlaze's MPD
    set ee [xget_value $hw_proc_handle "PARAMETER" "C_FPU_EXCEPTION"]
    if { $ee != "" } {
        return true
    } 

    return false
}

# --------------------------------------
# Return true if this MB has PVR support
# --------------------------------------
proc mb_has_pvr { hw_proc_handle } {
    
    # Check if the following parameters exist on this MicroBlaze's MPD
    set pvr [xget_value $hw_proc_handle "PARAMETER" "C_PVR"]
    if { $pvr != "" } {
        return true
    } 

    return false
}

# --------------------------------------
# Return true if MB ver 'procver' has 
# support for handling exceptions in 
# delay slots
# --------------------------------------
proc mb_can_handle_exceptions_in_delay_slots { procver } {
    
    if { [string compare -nocase $procver "5.00.a"] >= 0 } {
        return true
    } else {
        return false
    }
}


