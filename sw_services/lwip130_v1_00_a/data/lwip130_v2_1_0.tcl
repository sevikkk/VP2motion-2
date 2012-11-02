# emaclite hw requirements - interrupt needs to be connected
proc lwip_elite_hw_drc {libhandle emac} {
	set emacname [xget_hw_name $emac]
	set intr_port [xget_value $emac "PORT" "IP2INTC_Irpt"]
	if {[string compare -nocase $intr_port ""] == 0} {
		error "xps_ethernetlite core $emacname does not have its interrupt connected to interrupt controller. \
			lwIP operates only in interrupt mode, so please connect the interrupt port to \
			the interrupt controller.\n" "" "MDT_ERROR"
	}
}

# temac support requires the following interrupts to be connected
#	- temac intr
#	- if sdma, sdma tx&rx intr (not verified)
#	- if fifo, fifo intr (not verified)
proc lwip_temac_channel_hw_drc {libhandle emac irpt_name llink_name tx_csum_name rx_csum_name} {
	set emacname [xget_hw_name $emac]
	set mhs_handle [xget_handle $emac "parent"]

	set intr_port [xget_value $emac "PORT" $irpt_name]
	if {[string compare -nocase $intr_port ""] == 0} {
		error "xps_ll_temac core $emacname does not have its interrupt connected to interrupt controller. \
			lwIP requires that this interrupt line be connected to \
			the interrupt controller.\n" "" "MDT_ERROR"
	}

	# find out what is connected to llink0
	set connected_bus [xget_hw_busif_handle $emac $llink_name]
	if {[string compare -nocase $intr_port ""] == 0} {
                error "xps_ll_temac core $emacname does not have its local link port (LLINK0) connected" \
                    "" "MDT_ERROR"
        }

	set connected_bus_name [xget_hw_value $connected_bus]	
	set target_handle [xget_hw_connected_busifs_handle $mhs_handle $connected_bus_name "TARGET"]
	set parent_handle [xget_handle $target_handle "parent"]
	set parent_name [xget_hw_value $parent_handle]

 	set tx_csum [xget_value $libhandle "PARAMETER" "tcp_tx_checksum_offload"]
 	set rx_csum [xget_value $libhandle "PARAMETER" "tcp_rx_checksum_offload"]

	# check checksum parameters 
	if {$tx_csum} {
		if {$parent_name == "xps_ll_fifo"} {
			error "Checksum offload is possible only with a DMA engine" "" "MDT_ERROR"
		}
		set hw_tx_csum [xget_value $emac "PARAMETER" $tx_csum_name]
		if {!$hw_tx_csum} {
			error "lwIP cannot offload TX checksum calculation since hardware \
				does not support TX checksum offload" "" "MDT_ERROR"
		}
	}

	if {$rx_csum} {
		if {$parent_name == "xps_ll_fifo"} {
			error "Checksum offload is possible only with a DMA engine" "" "MDT_ERROR"
		}
		set hw_rx_csum [xget_value $emac "PARAMETER" $rx_csum_name]
		if {!$hw_rx_csum} {
			error "lwIP cannot offload RX checksum calculation since hardware \
				does not support RX checksum offload" "" "MDT_ERROR"
		}
	}
}

proc lwip_temac_hw_drc {libhandle emac} {
	set emac_intr_port [xget_value $emac "PORT" "TemacIntc0_Irpt"]
	set emac0_enabled "0"
	if {$emac_intr_port != ""} { set emac0_enabled "1" }
	if {$emac0_enabled == "1"} {
		lwip_temac_channel_hw_drc $libhandle $emac "TemacIntc0_Irpt" "LLINK0" "C_TEMAC0_TXCSUM" "C_TEMAC0_RXCSUM"
	}

	set emac1_enabled [xget_value $emac "PARAMETER" "C_TEMAC1_ENABLED"]
	if {$emac1_enabled == "1"} {
		lwip_temac_channel_hw_drc $libhandle $emac "TemacIntc1_Irpt" "LLINK1" "C_TEMAC1_TXCSUM" "C_TEMAC1_RXCSUM"
	}
}

#---
# perform basic sanity checks: 
#	- interrupts are connected
# 	- for csum offload, sdma should be present
#--
proc lwip_hw_drc {libhandle emacs_list} {
	foreach ip $emacs_list {
		set iptype [xget_hw_value $ip]
		if {$iptype == "xps_ethernetlite"} {
			lwip_elite_hw_drc $libhandle $ip
		} elseif {$iptype == "xps_ll_temac"} {
			lwip_temac_hw_drc $libhandle $ip
		}
	}
}

#--------
# Check the following s/w requirements for lwIP:
#	1. in SOCKET API is used, then xilkernel is required
#--------
proc lwip_sw_drc {libhandle emacs_list} {
	set api_mode [xget_value $libhandle "PARAMETER" "api_mode"]
	set api_mode [string toupper $api_mode]
	if { [string compare -nocase "SOCKET_API" $api_mode] == 0} {
    		set sw_proc_handle [xget_libgen_proc_handle]
		set os_handle [xget_handle $sw_proc_handle "OS"]
		set os_name [xget_value $os_handle "NAME"]
		if { [string compare -nocase "xilkernel" $os_name] != 0} {
			error "lwIP with Sockets requires \"xilkernel\" OS" "" "mdt_error"
		}
	}
}

proc get_emac_periphs {processor} {
	set periphs_list [xget_hw_proc_slave_periphs $processor]
	set emac_periphs_list {}

	foreach periph $periphs_list {
		set periphname [xget_hw_value $periph]
		if {$periphname == "xps_ethernetlite" 
			|| $periphname == "opb_ethernetlite"} {
			lappend emac_periphs_list $periph
		} elseif {$periphname == "xps_ll_temac"} {
			set emac_intr_port [xget_value $periph "PORT" "TemacIntc0_Irpt"]
			set emac0_enabled "0"
			if {$emac_intr_port != ""} { set emac0_enabled "1" }
			set emac1_enabled [xget_value $periph "PARAMETER" "C_TEMAC1_ENABLED"]
			if {$emac0_enabled == "1" || $emac1_enabled == "1"} {
				lappend emac_periphs_list $periph
			}
		}
	}

	return $emac_periphs_list
}

#---------------------------------------------
# lwip_drc - check system configuration and make sure 
# all components to run lwIP are available. 
#---------------------------------------------
proc lwip_drc {libhandle} {
	puts "Runnning DRC for lwIP library... \n"

	# find the list of xps_ethernetlite or xps_ll_temac cores
	set sw_processor [xget_libgen_proc_handle]
	set processor [xget_handle $sw_processor "IPINST"]

	set emac_periphs_list [get_emac_periphs $processor]

	if { [llength $emac_periphs_list] == 0 } {
		set cpuname [xget_hw_name $processor]
		error "No Ethernet MAC cores are addressable from processor $cpuname. \
			lwIP requires atleast one EMAC (xps_ethernetlite | xps_ll_temac) core.\n" "" "MDT_ERROR"
		return
	} else {
		set emac_names_list {}

		foreach emac $emac_periphs_list {
			lappend emac_names_list [xget_hw_name $emac]
		}
		puts "lwIP can be used with the following EMAC peripherals found in your system: $emac_names_list"
	}

	#----
	# check each MAC for correctness conditions
	#----

	lwip_hw_drc $libhandle $emac_periphs_list
	lwip_sw_drc $libhandle $emac_periphs_list
}

proc generate_lwip_opts {libhandle} {
	set lwipopts_file "src/contrib/ports/xilinx/include/lwipopts.h"
    	
	file delete $lwipopts_file
    	set lwipopts_fd [open $lwipopts_file w]

	puts $lwipopts_fd "\#ifndef __LWIPOPTS_H_"
	puts $lwipopts_fd "\#define __LWIPOPTS_H_"
	puts $lwipopts_fd ""

	# always use lightweight prot mechanism for critical regions
	puts $lwipopts_fd "\#define SYS_LIGHTWEIGHT_PROT 1"
	puts $lwipopts_fd ""

    	set api_mode [xget_value $libhandle "PARAMETER" "api_mode"]
	if {$api_mode == "RAW_API"} {
		puts $lwipopts_fd "\#define NO_SYS 1"
		puts $lwipopts_fd "\#define LWIP_SOCKET 0"
		puts $lwipopts_fd "\#define LWIP_COMPAT_SOCKETS 0"
		puts $lwipopts_fd "\#define LWIP_NETCONN 0"
	}
	puts $lwipopts_fd ""

	set thread_prio [xget_value $libhandle "PARAMETER" "socket_mode_thread_prio"]
	if {$api_mode == "SOCKET_API"} {
		puts $lwipopts_fd "\#define TCPIP_THREAD_PRIO $thread_prio"
		puts $lwipopts_fd "\#define DEFAULT_THREAD_PRIO $thread_prio"
		puts $lwipopts_fd ""
	}

	# memory options
    	set mem_size 		[xget_value $libhandle "PARAMETER" "mem_size"]
    	set memp_n_pbuf 	[xget_value $libhandle "PARAMETER" "memp_n_pbuf"]
	set memp_n_udp_pcb 	[xget_value $libhandle "PARAMETER" "memp_n_udp_pcb"]
	set memp_n_tcp_pcb 	[xget_value $libhandle "PARAMETER" "memp_n_tcp_pcb"]
	set memp_n_tcp_pcb_listen 	[xget_value $libhandle "PARAMETER" "memp_n_tcp_pcb_listen"]
	set memp_n_tcp_seg 	[xget_value $libhandle "PARAMETER" "memp_n_tcp_seg"]
	set memp_n_sys_timeout 	[xget_value $libhandle "PARAMETER" "memp_n_sys_timeout"]
	set memp_num_netbuf 	[xget_value $libhandle "PARAMETER" "memp_num_netbuf"]
	set memp_num_netconn 	[xget_value $libhandle "PARAMETER" "memp_num_netconn"]
	set memp_num_api_msg 	[xget_value $libhandle "PARAMETER" "memp_num_api_msg"]
	set memp_num_tcpip_msg 	[xget_value $libhandle "PARAMETER" "memp_num_tcpip_msg"]

	puts $lwipopts_fd "\#define MEM_ALIGNMENT 8"
	puts $lwipopts_fd "\#define MEM_SIZE $mem_size"
	puts $lwipopts_fd "\#define MEMP_NUM_PBUF $memp_n_pbuf"
	puts $lwipopts_fd "\#define MEMP_NUM_UDP_PCB $memp_n_udp_pcb"
	puts $lwipopts_fd "\#define MEMP_NUM_TCP_PCB $memp_n_tcp_pcb"
	puts $lwipopts_fd "\#define MEMP_NUM_TCP_PCB_LISTEN $memp_n_tcp_pcb_listen"
	puts $lwipopts_fd "\#define MEMP_NUM_TCP_SEG $memp_n_tcp_seg"

	# workaround for lwip mem_malloc bug
	# puts $lwipopts_fd "\#define MEM_LIBC_MALLOC 1"
	puts $lwipopts_fd "\#define LWIP_USE_HEAP_FROM_INTERRUPT 1"
	puts $lwipopts_fd ""

	# seq api
	if {$api_mode == "SOCKET_API"} {
		puts $lwipopts_fd "\#define MEMP_NUM_NETBUF     $memp_num_netbuf"
		puts $lwipopts_fd "\#define MEMP_NUM_NETCONN    $memp_num_netconn"
		puts $lwipopts_fd "\#define LWIP_PROVIDE_ERRNO  1"
	}
	puts $lwipopts_fd "\#define MEMP_NUM_SYS_TIMEOUT $memp_n_sys_timeout"

	# pbuf options
	set pbuf_pool_size	[xget_value $libhandle "PARAMETER" "pbuf_pool_size"]
	set pbuf_pool_bufsize	[xget_value $libhandle "PARAMETER" "pbuf_pool_bufsize"]
	set pbuf_link_hlen	[xget_value $libhandle "PARAMETER" "pbuf_link_hlen"]

	puts $lwipopts_fd "\#define PBUF_POOL_SIZE $pbuf_pool_size"
	puts $lwipopts_fd "\#define PBUF_POOL_BUFSIZE $pbuf_pool_bufsize"
	puts $lwipopts_fd "\#define PBUF_LINK_HLEN $pbuf_link_hlen"
	puts $lwipopts_fd ""

	# ARP options
	set arp_table_size	[xget_value $libhandle "PARAMETER" "arp_table_size"]
	set arp_queueing	[xget_value $libhandle "PARAMETER" "arp_queueing"]
	puts $lwipopts_fd "\#define ARP_TABLE_SIZE $arp_table_size"
	puts $lwipopts_fd "\#define ARP_QUEUEING $arp_queueing"
	puts $lwipopts_fd ""

	# ICMP options
	set icmp_ttl 		[xget_value $libhandle "PARAMETER" "icmp_ttl"]
	puts $lwipopts_fd "\#define ICMP_TTL $icmp_ttl"
	puts $lwipopts_fd ""

	# IP options
	set ip_forward          [xget_value $libhandle "PARAMETER" "ip_forward"]
	set ip_options          [xget_value $libhandle "PARAMETER" "ip_options"]
	set ip_reassembly       [xget_value $libhandle "PARAMETER" "ip_reassembly"]
	set ip_frag             [xget_value $libhandle "PARAMETER" "ip_frag"]
	set ip_reass_bufsize    [xget_value $libhandle "PARAMETER" "ip_reass_bufsize"]
	set ip_frag_max_mtu     [xget_value $libhandle "PARAMETER" "ip_frag_max_mtu"]
	set ip_default_ttl      [xget_value $libhandle "PARAMETER" "ip_default_ttl"]

	puts $lwipopts_fd "\#define IP_OPTIONS $ip_options"
	puts $lwipopts_fd "\#define IP_FORWARD $ip_forward"
	puts $lwipopts_fd "\#define IP_REASSEMBLY $ip_reassembly"
	puts $lwipopts_fd "\#define IP_FRAG $ip_frag"
	puts $lwipopts_fd "\#define IP_REASS_BUFSIZE $ip_reass_bufsize"
	puts $lwipopts_fd "\#define IP_FRAG_MAX_MTU $ip_frag_max_mtu"
	puts $lwipopts_fd "\#define IP_DEFAULT_TTL $ip_default_ttl"
	puts $lwipopts_fd "\#define LWIP_CHKSUM_ALGORITHM 3"
	puts $lwipopts_fd ""

	# UDP options
	set lwip_udp 		[expr [xget_value $libhandle "PARAMETER" "lwip_udp"] == true]
	set udp_ttl 		[xget_value $libhandle "PARAMETER" "udp_ttl"]
	puts $lwipopts_fd "\#define LWIP_UDP $lwip_udp"
	puts $lwipopts_fd "\#define UDP_TTL $udp_ttl"
	puts $lwipopts_fd ""

	# TCP options
    	set lwip_tcp 		[expr [xget_value $libhandle "PARAMETER" "lwip_tcp"] == true]
    	set tcp_mss 		[xget_value $libhandle "PARAMETER" "tcp_mss"]
    	set tcp_snd_buf 	[xget_value $libhandle "PARAMETER" "tcp_snd_buf"]
    	set tcp_wnd 		[xget_value $libhandle "PARAMETER" "tcp_wnd"]
	set tcp_ttl		[xget_value $libhandle "PARAMETER" "tcp_ttl"]
	set tcp_maxrtx          [xget_value $libhandle "PARAMETER" "tcp_maxrtx"]
	set tcp_synmaxrtx       [xget_value $libhandle "PARAMETER" "tcp_synmaxrtx"]
	set tcp_queue_ooseq     [xget_value $libhandle "PARAMETER" "tcp_queue_ooseq"]

	puts $lwipopts_fd "\#define LWIP_TCP $lwip_tcp"
	puts $lwipopts_fd "\#define TCP_SND_QUEUELEN   16 * TCP_SND_BUF/TCP_MSS"
	puts $lwipopts_fd "\#define TCP_MSS $tcp_mss"
	puts $lwipopts_fd "\#define TCP_SND_BUF $tcp_snd_buf"
	puts $lwipopts_fd "\#define TCP_WND $tcp_wnd"
	puts $lwipopts_fd "\#define TCP_TTL $tcp_ttl"
	puts $lwipopts_fd "\#define TCP_MAXRTX $tcp_maxrtx"
	puts $lwipopts_fd "\#define TCP_SYNMAXRTX $tcp_synmaxrtx"
	puts $lwipopts_fd "\#define TCP_QUEUE_OOSEQ $tcp_queue_ooseq"

 	set tx_csum [expr ![xget_value $libhandle "PARAMETER" "tcp_tx_checksum_offload"]]
 	puts $lwipopts_fd "\#define CHECKSUM_GEN_TCP $tx_csum"
 	set rx_csum [expr ![xget_value $libhandle "PARAMETER" "tcp_rx_checksum_offload"]]
 	puts $lwipopts_fd "\#define CHECKSUM_CHECK_TCP $rx_csum"
	puts $lwipopts_fd ""

	set jumbo_frames [xget_value $libhandle "PARAMETER" "temac_use_jumbo_frames_experimental"]
	if {$jumbo_frames} {
		puts $lwipopts_fd "\#define XLLTEMACIF_USE_JUMBO_FRAMES_EXPERIMENTAL 1"
		puts $lwipopts_fd ""
	}

	# DHCP options
	set lwip_dhcp 		[expr [xget_value $libhandle "PARAMETER" "lwip_dhcp"] == true]
	set dhcp_does_arp_check [xget_value $libhandle "PARAMETER" "dhcp_does_arp_check"]
	puts $lwipopts_fd "\#define LWIP_DHCP $lwip_dhcp"
	puts $lwipopts_fd "\#define DHCP_DOES_ARP_CHECK $dhcp_does_arp_check"
	puts $lwipopts_fd ""


	# temac adapter options
	set linkspeed [xget_value $libhandle "PARAMETER" "phy_link_speed"]
	puts $lwipopts_fd "\#define $linkspeed 1"
	puts $lwipopts_fd ""

	# lwIP stats
	set lwip_stats		[xget_value $libhandle "PARAMETER" "lwip_stats"]
	if {$lwip_stats} {
		puts $lwipopts_fd "\#define LWIP_STATS 1"
		puts $lwipopts_fd "\#define LWIP_STATS_DISPLAY 1"
		puts $lwipopts_fd ""
	}

	# lwIP debug
	set lwip_debug		[expr [xget_value $libhandle "PARAMETER" "lwip_debug"] == true]
	set ip_debug		[expr [xget_value $libhandle "PARAMETER" "ip_debug"] == true]
	set tcp_debug		[expr [xget_value $libhandle "PARAMETER" "tcp_debug"] == true]
	set netif_debug		[expr [xget_value $libhandle "PARAMETER" "netif_debug"] == true]
	set sys_debug		[expr [xget_value $libhandle "PARAMETER" "sys_debug"] == true]
	set pbuf_debug		[expr [xget_value $libhandle "PARAMETER" "pbuf_debug"] == true]

	if {$lwip_debug == 1} {
		puts $lwipopts_fd "\#define LWIP_DEBUG 1"
		puts $lwipopts_fd "\#define DBG_TYPES_ON DBG_LEVEL_WARNING"
		if {$ip_debug} { puts $lwipopts_fd "\#define IP_DEBUG (LWIP_DBG_LEVEL_SEVERE | LWIP_DBG_ON)" }
		if {$tcp_debug} { 
			puts $lwipopts_fd "\#define TCP_DEBUG (LWIP_DBG_LEVEL_SEVERE | LWIP_DBG_ON)" 
			puts $lwipopts_fd "\#define TCP_DEBUG (LWIP_DBG_LEVEL_SEVERE | LWIP_DBG_ON)" 
			puts $lwipopts_fd "\#define TCP_INPUT_DEBUG (LWIP_DBG_LEVEL_SEVERE | LWIP_DBG_ON)" 
			puts $lwipopts_fd "\#define TCP_OUTPUT_DEBUG (LWIP_DBG_LEVEL_SEVERE | LWIP_DBG_ON)" 
			puts $lwipopts_fd "\#define TCPIP_DEBUG (LWIP_DBG_LEVEL_SEVERE | LWIP_DBG_ON)" 
		}
		if {$netif_debug} { puts $lwipopts_fd "\#define NETIF_DEBUG (LWIP_DBG_LEVEL_SEVERE | LWIP_DBG_ON)" }
		if {$sys_debug} { 
			puts $lwipopts_fd "\#define SYS_DEBUG (LWIP_DBG_LEVEL_SEVERE | LWIP_DBG_ON)"
			puts $lwipopts_fd "\#define API_MSG_DEBUG (LWIP_DBG_LEVEL_SEVERE | LWIP_DBG_ON)" 
		}
		if {$pbuf_debug} {
			puts $lwipopts_fd "\#define PBUF_DEBUG (LWIP_DBG_LEVEL_SEVERE | LWIP_DBG_ON)"
		}
		puts $lwipopts_fd "\#define MEMP_DEBUG (LWIP_DBG_LEVEL_SEVERE | LWIP_DBG_ON)"
		puts $lwipopts_fd ""
	}

	puts $lwipopts_fd "\#endif"
	close $lwipopts_fd
}


proc update_temac_topology {emac processor topologyvar} {
	upvar $topologyvar topology

        set topology(emac_baseaddr) [xget_hw_parameter_value $emac "C_BASEADDR"]
	set topology(emac_type) "xemac_type_xps_ll_temac"

	# find intc to which the interrupt line is connected
	set emac_intr_port [xget_value $emac "PORT" "TemacIntc0_Irpt"]
	set mhs_handle [xget_handle $emac "parent"]
	set intr_ports [xget_connected_ports_handle $mhs_handle $emac_intr_port "sink"]

	set l [llength $intr_ports]
	puts "$emac_intr_port connected to $l ports. $l != 1"

	if { [llength $intr_ports] != 1 } {
		set emac_name [xget_hw_name $emac]
		error "xps_ll_temac ($emac_name) interrupt port connected to more than IP.\
			lwIP requires that the interrupt line be connected only to the interrupt controller" 
			"" "mdt_error"
	}

	set intr_port [lindex $intr_ports 0]
	set intc_handle [xget_handle $intr_port "parent"]

	# can we address this intc from the processor?
	set proc_connected_periphs [xget_hw_proc_slave_periphs $processor]
	if { [lsearch -exact $proc_connected_periphs $intc_handle] == -1 } {
		set intc_name [xget_hw_name $intc_handle]
		set proc_name [xget_hw_name $processor]	
		error "ERROR: $intc_name to which xps_ll_temac interrupt is connected is not addressable \
			from processor $proc_name" "" "mdt_error"
	}

	set topology(intc_baseaddr) [xget_hw_parameter_value $intc_handle "C_BASEADDR"]
	set topology(intc_baseaddr) [xformat_addr_string $topology(intc_baseaddr) "C_BASEADDR"]
}

# redundant code
# repeat the same steps as update_temac_topology, but for temac1 
proc update_temac1_topology {emac processor topologyvar} {
	upvar $topologyvar topology

        set topology(emac_baseaddr) [format 0x%x [expr [xget_hw_parameter_value $emac "C_BASEADDR"] + 0x40]]
	set topology(emac_type) "xemac_type_xps_ll_temac"

	# find intc to which the interrupt line is connected
	set emac_intr_port [xget_value $emac "PORT" "TemacIntc1_Irpt"]
	set mhs_handle [xget_handle $emac "parent"]
	set intr_ports [xget_connected_ports_handle $mhs_handle $emac_intr_port "sink"]

	if { [llength $intr_ports] != 1 } {
		set emac_name [xget_hw_name $emac]
		error "xps_ll_temac ($emac_name) interrupt port connected to more than IP.\
			lwIP requires that the interrupt line be connected only to the interrupt controller" 
			"" "mdt_error"
	}

	set intr_port [lindex $intr_ports 0]
	set intc_handle [xget_handle $intr_port "parent"]

	# can we address this intc from the processor?
	set proc_connected_periphs [xget_hw_proc_slave_periphs $processor]
	if { [lsearch -exact $proc_connected_periphs $intc_handle] == -1 } {
		set intc_name [xget_hw_name $intc_handle]
		set proc_name [xget_hw_name $processor]	
		error "ERROR: $intc_name to which xps_ll_temac interrupt is connected is not addressable \
			from processor $proc_name" "" "mdt_error"
	}

	set topology(intc_baseaddr) [xget_hw_parameter_value $intc_handle "C_BASEADDR"]
	set topology(intc_baseaddr) [xformat_addr_string $topology(intc_baseaddr) "C_BASEADDR"]
}

proc update_emaclite_topology {emac processor topologyvar} {
	upvar $topologyvar topology

	# get emac baseaddr
        set topology(emac_baseaddr) [xget_hw_parameter_value $emac "C_BASEADDR"]
	set topology(emac_type) "xemac_type_xps_emaclite"

	# find intc to which the interrupt line is connected
	set emac_intr_port [xget_value $emac "PORT" "IP2INTC_Irpt"]
	set mhs_handle [xget_handle $emac "parent"]
	set intr_ports [xget_connected_ports_handle $mhs_handle $emac_intr_port "sink"]

	if { [llength $intr_ports] != 1 } {
		set emac_name [xget_hw_name $emac]
		error "emaclite ($emac_name) interrupt port connected to more than IP.\
			lwIP requires that the interrupt line be connected only to the interrupt controller" 
			"" "mdt_error"
	}

	set intr_port [lindex $intr_ports 0]
	set intc_handle [xget_handle $intr_port "parent"]

	# can we address this intc from the processor?
	set proc_connected_periphs [xget_hw_proc_slave_periphs $processor]
	if { [lsearch -exact $proc_connected_periphs $intc_handle] == -1 } {
		set intc_name [xget_hw_name $intc_handle]
		set proc_name [xget_hw_name $processor]	
		error "$intc_name to which ethernetlite interrupt is connected is not addressable \
			from processor $proc_name" "" "mdt_error"
	}

	set topology(intc_baseaddr) [xget_hw_parameter_value $intc_handle "C_BASEADDR"]

	# find interrupt pin number
	set num_intr_inputs [xget_value $intc_handle "PARAMETER" "C_NUM_INTR_INPUTS"]
	set signals [split [xget_value $intr_port "VALUE"] "&"]
	set i 1
	foreach signal $signals {
		set signal [string trim $signal]
		if {[string compare -nocase $signal $emac_intr_port] == 0} {
			set topology(emac_intr_id) [expr ($num_intr_inputs - $i)]
			break
		}
		incr i
	}
}

proc generate_topology_per_emac {fd topologyvar} {
	upvar $topologyvar topology

	puts $fd "	{"
	puts $fd "		$topology(emac_baseaddr),"
	puts $fd "		$topology(emac_type),"
	puts $fd "		$topology(intc_baseaddr),"
	puts $fd "		$topology(emac_intr_id),"
	puts $fd "	},"
}

proc generate_topology {libhandle} {
	set sw_processor [xget_libgen_proc_handle]
	set processor [xget_handle $sw_processor "IPINST"]
	set emac_periphs_list [get_emac_periphs $processor]

	set topology_file "src/contrib/ports/xilinx/netif/xtopology_g.c"
	set topology_size 0
    	
	file delete $topology_file
    	set tfd [open $topology_file w]

	puts $tfd "#include \"netif/xtopology.h\""
	puts $tfd ""

	puts $tfd "struct xtopology_t xtopology\[\] = {"

	foreach emac $emac_periphs_list {
		# initialize topology variables
		set topology(emac_baseaddr) -1
		set topology(emac_type) "xemac_type_unknown"
		set topology(intc_baseaddr) -1	
		set topology(emac_intr_id)  -1

		# get topology for the emac
		set iptype [xget_hw_value $emac]
		if {$iptype == "xps_ethernetlite" || $iptype == "opb_ethernetlite"} {
			update_emaclite_topology $emac $processor topology
			generate_topology_per_emac $tfd topology
			incr topology_size 1
		} elseif {$iptype == "xps_ll_temac"} {
			set emac_intr_port [xget_value $emac "PORT" "TemacIntc0_Irpt"]
			set temac0 "0"
			if {$emac_intr_port != ""} { set temac0 "1" }
			if {$temac0 == "1"} {
				update_temac_topology $emac $processor topology
				generate_topology_per_emac $tfd topology
				incr topology_size 1
			}

        		set temac1 [xget_param_value $emac "C_TEMAC1_ENABLED"]
			if {$temac1 == "1"} {
				update_temac1_topology $emac $processor topology
				generate_topology_per_emac $tfd topology
				incr topology_size 1
			}
		}
	}

	puts $tfd "};"
	puts $tfd ""

	puts $tfd "int xtopology_n_emacs = $topology_size;"

	close $tfd
}

proc generate_adapterconfig_makefile {libhandle} {
	set sw_processor [xget_libgen_proc_handle]
	set processor [xget_handle $sw_processor "IPINST"]
	set emac_periphs_list [get_emac_periphs $processor]

	set have_emaclite 0
	set have_temac 0

	foreach emac $emac_periphs_list {
		set iptype [xget_hw_value $emac]
		if {$iptype == "xps_ethernetlite" || $iptype == "opb_ethernetlite"} {
			set have_emaclite 1
		} elseif {$iptype == "xps_ll_temac"} {
			set have_temac 1
		}
	}

	set makeconfig "src/Makefile.config"	
	file delete $makeconfig
	set fd [open $makeconfig w]

        # determine the processor type so that we know the compiler to use
        set proctype [xget_value $processor "OPTION" "IPNAME"]
        switch -regexp $proctype {
            "microblaze" {
                puts $fd "GCC_COMPILER=mb-gcc"
            }
            ppc* {
                puts $fd "GCC_COMPILER=powerpc-eabi-gcc"
            }
            default {
                puts "unknown processor type $proctype\n"
            }
        }

	if {$have_emaclite == 1} {
		puts $fd "CONFIG_XEMACLITE=y"
	}

	if {$have_temac == 1} {
		puts $fd "CONFIG_XLLTEMAC=y"
	}

    	set api_mode [xget_value $libhandle "PARAMETER" "api_mode"]
	if {$api_mode == "SOCKET_API"} {
		puts $fd "CONFIG_SOCKETS=y"
	}

	close $fd
}

proc generate_adapterconfig_include {libhandle} {
	set sw_processor [xget_libgen_proc_handle]
	set processor [xget_handle $sw_processor "IPINST"]
	set emac_periphs_list [get_emac_periphs $processor]

	set have_emaclite 0
	set have_temac 0

	foreach emac $emac_periphs_list {
		set iptype [xget_hw_value $emac]
		if {$iptype == "xps_ethernetlite" || $iptype == "opb_ethernetlite"} {
			set have_emaclite 1
		} elseif {$iptype == "xps_ll_temac"} {
			set have_temac 1
		}
	}

	set adapterconfig "src/contrib/ports/xilinx/include/xlwipconfig.h"	
	file delete $adapterconfig
	set fd [open $adapterconfig w]

	puts $fd "\#ifndef __XLWIPCONFIG_H_"
	puts $fd "\#define __XLWIPCONFIG_H_\n\n"
	puts $fd "/* This is a generated file - do not edit */"
	puts $fd ""

	if {$have_emaclite == 1} {
		puts $fd "\#define XLWIP_CONFIG_INCLUDE_EMACLITE 1"
	}

	if {$have_temac == 1} {
		puts $fd "\#define XLWIP_CONFIG_INCLUDE_TEMAC 1"

	    	set ndesc [xget_value $libhandle "PARAMETER" "n_tx_descriptors"]
		puts $fd "\#define XLWIP_CONFIG_N_TX_DESC $ndesc"	
	    	set ndesc [xget_value $libhandle "PARAMETER" "n_rx_descriptors"]
		puts $fd "\#define XLWIP_CONFIG_N_RX_DESC $ndesc"	
		puts $fd ""
	
	    	set ncoalesce [xget_value $libhandle "PARAMETER" "n_tx_coalesce"]
		puts $fd "\#define XLWIP_CONFIG_N_TX_COALESCE $ncoalesce"	
	    	set ncoalesce [xget_value $libhandle "PARAMETER" "n_rx_coalesce"]
		puts $fd "\#define XLWIP_CONFIG_N_RX_COALESCE $ncoalesce"	
		puts $fd ""
	}

	puts $fd "\#endif"

	close $fd
}

#-------
# generate: called after OS and library files are copied into project dir
# 	we need to generate the following:
#		1. Makefile options
#		2. System Arch settings for lwIP to use
#-------
proc generate {libhandle} {
	generate_lwip_opts $libhandle
	generate_topology  $libhandle	
	generate_adapterconfig_makefile $libhandle
	generate_adapterconfig_include $libhandle
}

#-------
# post_generate: called after generate called on all libraries
#-------
proc post_generate {libhandle} {
}

#-------
# execs_generate: called after BSP's, libraries and drivers have been compiled
#	This procedure builds the liblwip4.a library
#-------
proc execs_generate {libhandle} {
}
