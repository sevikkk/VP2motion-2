//----------------------------------------------------------------------------
// user_logic.vhd - module
//----------------------------------------------------------------------------
//
// ***************************************************************************
// ** Copyright (c) 1995-2008 Xilinx, Inc.  All rights reserved.            **
// **                                                                       **
// ** Xilinx, Inc.                                                          **
// ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
// ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
// ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
// ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
// ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
// ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
// ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
// ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
// ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
// ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
// ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
// ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
// ** FOR A PARTICULAR PURPOSE.                                             **
// **                                                                       **
// ***************************************************************************
//
//----------------------------------------------------------------------------
// Filename:          user_logic.vhd
// Version:           1.00.a
// Description:       User logic module.
// Date:              Tue Oct  9 01:16:28 2012 (by Create and Import Peripheral Wizard)
// Verilog Standard:  Verilog-2001
//----------------------------------------------------------------------------
// Naming Conventions:
//   active low signals:                    "*_n"
//   clock signals:                         "clk", "clk_div#", "clk_#x"
//   reset signals:                         "rst", "rst_n"
//   generics:                              "C_*"
//   user defined types:                    "*_TYPE"
//   state machine next state:              "*_ns"
//   state machine current state:           "*_cs"
//   combinatorial signals:                 "*_com"
//   pipelined or register delay signals:   "*_d#"
//   counter signals:                       "*cnt*"
//   clock enable signals:                  "*_ce"
//   internal version of output port:       "*_i"
//   device pins:                           "*_pin"
//   ports:                                 "- Names begin with Uppercase"
//   processes:                             "*_PROCESS"
//   component instantiations:              "<ENTITY_>I_<#|FUNC>"
//----------------------------------------------------------------------------

module user_logic
(
  // -- ADD USER PORTS BELOW THIS LINE ---------------
  // --USER ports added here 
  S_Dir,
  S_Step,
  S_Enable,
  E_Min,
  E_Max,
  E_Probe,
  A_Step,
  A_Load,
  // -- ADD USER PORTS ABOVE THIS LINE ---------------

  // -- DO NOT EDIT BELOW THIS LINE ------------------
  // -- Bus protocol ports, do not add to or delete 
  Bus2IP_Clk,                     // Bus to IP clock
  Bus2IP_Reset,                   // Bus to IP reset
  Bus2IP_Addr,                    // Bus to IP address bus
  Bus2IP_CS,                      // Bus to IP chip select for user logic memory selection
  Bus2IP_RNW,                     // Bus to IP read/not write
  Bus2IP_Data,                    // Bus to IP data bus
  Bus2IP_BE,                      // Bus to IP byte enables
  Bus2IP_RdCE,                    // Bus to IP read chip enable
  Bus2IP_WrCE,                    // Bus to IP write chip enable
  IP2Bus_Data,                    // IP to Bus data bus
  IP2Bus_RdAck,                   // IP to Bus read transfer acknowledgement
  IP2Bus_WrAck,                   // IP to Bus write transfer acknowledgement
  IP2Bus_Error,                   // IP to Bus error response
  IP2Bus_IntrEvent                // IP to Bus interrupt event
  // -- DO NOT EDIT ABOVE THIS LINE ------------------
); // user_logic

// -- ADD USER PARAMETERS BELOW THIS LINE ------------
// --USER parameters added here 
// -- ADD USER PARAMETERS ABOVE THIS LINE ------------

// -- DO NOT EDIT BELOW THIS LINE --------------------
// -- Bus protocol parameters, do not add to or delete
parameter C_SLV_AWIDTH                   = 32;
parameter C_SLV_DWIDTH                   = 32;
parameter C_NUM_REG                      = 32;
parameter C_NUM_MEM                      = 1;
parameter C_NUM_INTR                     = 4;
// -- DO NOT EDIT ABOVE THIS LINE --------------------

// -- ADD USER PORTS BELOW THIS LINE -----------------
// --USER ports added here 
output S_Dir;
output S_Step;
output S_Enable;
input  E_Min;
input  E_Max;
input  E_Probe;
input  A_Step;
input  A_Load;
// -- ADD USER PORTS ABOVE THIS LINE -----------------

// -- DO NOT EDIT BELOW THIS LINE --------------------
// -- Bus protocol ports, do not add to or delete
input                                     Bus2IP_Clk;
input                                     Bus2IP_Reset;
input      [0 : C_SLV_AWIDTH-1]           Bus2IP_Addr;
input      [0 : C_NUM_MEM-1]              Bus2IP_CS;
input                                     Bus2IP_RNW;
input      [C_SLV_DWIDTH-1:0]           Bus2IP_Data;
//input      [0 : C_SLV_DWIDTH-1]           Bus2IP_Data;
input      [0 : C_SLV_DWIDTH/8-1]         Bus2IP_BE;
input      [0 : C_NUM_REG-1]              Bus2IP_RdCE;
input      [0 : C_NUM_REG-1]              Bus2IP_WrCE;
output  reg   [C_SLV_DWIDTH-1:0]           IP2Bus_Data;
//output  reg   [0 : C_SLV_DWIDTH-1]           IP2Bus_Data;
output                                    IP2Bus_RdAck;
output                                    IP2Bus_WrAck;
output                                    IP2Bus_Error;
output     [0 : C_NUM_INTR-1]             IP2Bus_IntrEvent;
// -- DO NOT EDIT ABOVE THIS LINE --------------------

//----------------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------------

  // --USER nets declarations added here, as needed for user logic

  // Nets for user logic slave model s/w accessible register example
  reg        [C_SLV_DWIDTH-1:0]           reg_cmd;
  reg        [C_SLV_DWIDTH-1:0]           reg_next_xl;
  reg        [C_SLV_DWIDTH-1:0]           reg_next_xh;
  reg        [C_SLV_DWIDTH-1:0]           reg_next_v;
  reg        [C_SLV_DWIDTH-1:0]           reg_next_a;
  reg        [C_SLV_DWIDTH-1:0]           reg_next_j;
  reg        [C_SLV_DWIDTH-1:0]           reg_next_dt;
  reg        [C_SLV_DWIDTH-1:0]           reg_next_steps;
  reg        [C_SLV_DWIDTH-1:0]           reg_pre_n;
  reg        [C_SLV_DWIDTH-1:0]           reg_pulse_n;
  reg        [C_SLV_DWIDTH-1:0]           reg_post_n;
  reg        [C_SLV_DWIDTH-1:0]           reg_step_bit;
  reg        [C_SLV_DWIDTH-1:0]           stat_xl;
  reg        [C_SLV_DWIDTH-1:0]           stat_xh;
  reg        [C_SLV_DWIDTH-1:0]           stat_v;
  reg        [C_SLV_DWIDTH-1:0]           stat_a;
  reg        [C_SLV_DWIDTH-1:0]           stat_j;
  wire       [C_SLV_DWIDTH-1:0]           stat_dt;
  wire       [C_SLV_DWIDTH-1:0]           stat_steps;
  wire       [C_SLV_DWIDTH-1:0]           stat_status;
  reg        [C_SLV_DWIDTH-1:0]           stat_emin_xl;
  reg        [C_SLV_DWIDTH-1:0]           stat_emin_xh;
  reg        [C_SLV_DWIDTH-1:0]           stat_emax_xl;
  reg        [C_SLV_DWIDTH-1:0]           stat_emax_xh;
  reg        [C_SLV_DWIDTH-1:0]           stat_eprobe_xl;
  reg        [C_SLV_DWIDTH-1:0]           stat_eprobe_xh;
  reg        [C_SLV_DWIDTH-1:0]           stat_missed_steps;
  reg        [C_SLV_DWIDTH-1:0]           reg_stop_delay;
  reg        [C_SLV_DWIDTH-1:0]           reg_probe_delay;
  reg        [C_SLV_DWIDTH-1:0]           stat_min_bounce;
  reg        [C_SLV_DWIDTH-1:0]           stat_max_bounce;
  reg        [C_SLV_DWIDTH-1:0]           stat_probe_bounce;
  wire       [0 : 31]                       slv_reg_write_sel;
  wire       [0 : 31]                       slv_reg_read_sel;
  reg        [C_SLV_DWIDTH-1:0]           slv_ip2bus_data;
  wire                                      slv_read_ack;
  wire                                      slv_write_ack;
  integer                                   byte_index, bit_index;
  wire                                      done;
  wire                                      stopped;
  wire													load;

  // --USER logic implementation added here

  // ------------------------------------------------------
  // Example code to read/write user logic slave model s/w accessible registers
  // 
  // Note:
  // The example code presented here is to show you one way of reading/writing
  // software accessible registers implemented in the user logic slave model.
  // Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
  // to one software accessible register by the top level template. For example,
  // if you have four 32 bit software accessible registers in the user logic,
  // you are basically operating on the following memory mapped registers:
  // 
  //    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
  //                     "1000"   C_BASEADDR + 0x0
  //                     "0100"   C_BASEADDR + 0x4
  //                     "0010"   C_BASEADDR + 0x8
  //                     "0001"   C_BASEADDR + 0xC
  // 
  // ------------------------------------------------------

  assign
    slv_reg_write_sel = Bus2IP_WrCE[0:31],
    slv_reg_read_sel  = Bus2IP_RdCE[0:31],
    slv_write_ack     = Bus2IP_WrCE[0] || Bus2IP_WrCE[1] || Bus2IP_WrCE[2] || Bus2IP_WrCE[3] || Bus2IP_WrCE[4] || Bus2IP_WrCE[5] || Bus2IP_WrCE[6] || Bus2IP_WrCE[7] || Bus2IP_WrCE[8] || Bus2IP_WrCE[9] || Bus2IP_WrCE[10] || Bus2IP_WrCE[11] || Bus2IP_WrCE[12] || Bus2IP_WrCE[13] || Bus2IP_WrCE[14] || Bus2IP_WrCE[15] || Bus2IP_WrCE[16] || Bus2IP_WrCE[17] || Bus2IP_WrCE[18] || Bus2IP_WrCE[19] || Bus2IP_WrCE[20] || Bus2IP_WrCE[21] || Bus2IP_WrCE[22] || Bus2IP_WrCE[23] || Bus2IP_WrCE[24] || Bus2IP_WrCE[25] || Bus2IP_WrCE[26] || Bus2IP_WrCE[27] || Bus2IP_WrCE[28] || Bus2IP_WrCE[29] || Bus2IP_WrCE[30] || Bus2IP_WrCE[31],
    slv_read_ack      = Bus2IP_RdCE[0] || Bus2IP_RdCE[1] || Bus2IP_RdCE[2] || Bus2IP_RdCE[3] || Bus2IP_RdCE[4] || Bus2IP_RdCE[5] || Bus2IP_RdCE[6] || Bus2IP_RdCE[7] || Bus2IP_RdCE[8] || Bus2IP_RdCE[9] || Bus2IP_RdCE[10] || Bus2IP_RdCE[11] || Bus2IP_RdCE[12] || Bus2IP_RdCE[13] || Bus2IP_RdCE[14] || Bus2IP_RdCE[15] || Bus2IP_RdCE[16] || Bus2IP_RdCE[17] || Bus2IP_RdCE[18] || Bus2IP_RdCE[19] || Bus2IP_RdCE[20] || Bus2IP_RdCE[21] || Bus2IP_RdCE[22] || Bus2IP_RdCE[23] || Bus2IP_RdCE[24] || Bus2IP_RdCE[25] || Bus2IP_RdCE[26] || Bus2IP_RdCE[27] || Bus2IP_RdCE[28] || Bus2IP_RdCE[29] || Bus2IP_RdCE[30] || Bus2IP_RdCE[31];

  // implement slave model register(s)
  always @( posedge Bus2IP_Clk )
    begin: SLAVE_REG_WRITE_PROC

      if ( Bus2IP_Reset == 1 )
        begin
          reg_cmd <= 0;
          reg_next_xl <= 0;
          reg_next_xh <= 0;
          reg_next_v <= 0;
          reg_next_a <= 0;
          reg_next_j <= 0;
          reg_next_dt <= 0;
          reg_next_steps <= 0;
          reg_pre_n <= 0;
          reg_pulse_n <= 0;
          reg_post_n <= 0;
          reg_step_bit <= 0;
          // stat_xl <= 0;
          // stat_xh <= 0;
          // stat_v <= 0;
          // stat_a <= 0;
          // stat_j <= 0;
          // stat_dt <= 0;
          // stat_steps <= 0;
          // stat_status <= 0;
          // stat_emin_xl <= 0;
          // stat_emin_xh <= 0;
          // stat_emax_xl <= 0;
          // stat_emax_xh <= 0;
          // stat_eprobe_xl <= 0;
          // stat_eprobe_xh <= 0;
          // stat_missed_steps <= 0;
          reg_stop_delay <= 0;
          reg_probe_delay <= 0;
          // stat_min_bounce <= 0;
          // stat_max_bounce <= 0;
          // stat_probe_bounce <= 0;
        end
      else
       begin
        case ( slv_reg_write_sel )
          32'b10000000000000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_cmd[bit_index] <= Bus2IP_Data[bit_index];
          32'b01000000000000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_next_xl[bit_index] <= Bus2IP_Data[bit_index];
          32'b00100000000000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_next_xh[bit_index] <= Bus2IP_Data[bit_index];
          32'b00010000000000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_next_v[bit_index] <= Bus2IP_Data[bit_index];
          32'b00001000000000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_next_a[bit_index] <= Bus2IP_Data[bit_index];
          32'b00000100000000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_next_j[bit_index] <= Bus2IP_Data[bit_index];
          32'b00000010000000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_next_dt[bit_index] <= Bus2IP_Data[bit_index];
          32'b00000001000000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_next_steps[bit_index] <= Bus2IP_Data[bit_index];
          32'b00000000100000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_pre_n[bit_index] <= Bus2IP_Data[bit_index];
          32'b00000000010000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_pulse_n[bit_index] <= Bus2IP_Data[bit_index];
          32'b00000000001000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_post_n[bit_index] <= Bus2IP_Data[bit_index];
          32'b00000000000100000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_step_bit[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000010000000000000000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_xl[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000001000000000000000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_xh[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000100000000000000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_v[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000010000000000000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_a[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000001000000000000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_j[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000100000000000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_dt[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000010000000000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_steps[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000001000000000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_status[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000000100000000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_emin_xl[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000000010000000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_emin_xh[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000000001000000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_emax_xl[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000000000100000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_emax_xh[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000000000010000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_eprobe_xl[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000000000001000000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_eprobe_xh[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000000000000100000 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_missed_steps[bit_index] <= Bus2IP_Data[bit_index];
          32'b00000000000000000000000000010000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_stop_delay[bit_index] <= Bus2IP_Data[bit_index];
          32'b00000000000000000000000000001000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  reg_probe_delay[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000000000000000100 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_min_bounce[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000000000000000010 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_max_bounce[bit_index] <= Bus2IP_Data[bit_index];
          // 32'b00000000000000000000000000000001 :
          //   for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
          //     if ( Bus2IP_BE[byte_index] == 1 )
          //       for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
          //         stat_probe_bounce[bit_index] <= Bus2IP_Data[bit_index];
          default : ;
        endcase
        if ((slv_reg_write_sel != 32'b10000000000000000000000000000000) && (done || stopped))
          reg_cmd <= 0;
       end

    end // SLAVE_REG_WRITE_PROC

  // implement slave model register read mux
  always @( slv_reg_read_sel or reg_cmd or reg_next_xl or reg_next_xh or reg_next_v or reg_next_a or reg_next_j or reg_next_dt or reg_next_steps or reg_pre_n or reg_pulse_n or reg_post_n or reg_step_bit or stat_xl or stat_xh or stat_v or stat_a or stat_j or stat_dt or stat_steps or stat_status or stat_emin_xl or stat_emin_xh or stat_emax_xl or stat_emax_xh or stat_eprobe_xl or stat_eprobe_xh or stat_missed_steps or reg_stop_delay or reg_probe_delay or stat_min_bounce or stat_max_bounce or stat_probe_bounce )
    begin: SLAVE_REG_READ_PROC

      case ( slv_reg_read_sel )
        32'b10000000000000000000000000000000 : slv_ip2bus_data <= reg_cmd;
        32'b01000000000000000000000000000000 : slv_ip2bus_data <= reg_next_xl;
        32'b00100000000000000000000000000000 : slv_ip2bus_data <= reg_next_xh;
        32'b00010000000000000000000000000000 : slv_ip2bus_data <= reg_next_v;
        32'b00001000000000000000000000000000 : slv_ip2bus_data <= reg_next_a;
        32'b00000100000000000000000000000000 : slv_ip2bus_data <= reg_next_j;
        32'b00000010000000000000000000000000 : slv_ip2bus_data <= reg_next_dt;
        32'b00000001000000000000000000000000 : slv_ip2bus_data <= reg_next_steps;
        32'b00000000100000000000000000000000 : slv_ip2bus_data <= reg_pre_n;
        32'b00000000010000000000000000000000 : slv_ip2bus_data <= reg_pulse_n;
        32'b00000000001000000000000000000000 : slv_ip2bus_data <= reg_post_n;
        32'b00000000000100000000000000000000 : slv_ip2bus_data <= reg_step_bit;
        32'b00000000000010000000000000000000 : slv_ip2bus_data <= stat_xl;
        32'b00000000000001000000000000000000 : slv_ip2bus_data <= stat_xh;
        32'b00000000000000100000000000000000 : slv_ip2bus_data <= stat_v;
        32'b00000000000000010000000000000000 : slv_ip2bus_data <= stat_a;
        32'b00000000000000001000000000000000 : slv_ip2bus_data <= stat_j;
        32'b00000000000000000100000000000000 : slv_ip2bus_data <= stat_dt;
        32'b00000000000000000010000000000000 : slv_ip2bus_data <= stat_steps;
        32'b00000000000000000001000000000000 : slv_ip2bus_data <= stat_status;
        32'b00000000000000000000100000000000 : slv_ip2bus_data <= stat_emin_xl;
        32'b00000000000000000000010000000000 : slv_ip2bus_data <= stat_emin_xh;
        32'b00000000000000000000001000000000 : slv_ip2bus_data <= stat_emax_xl;
        32'b00000000000000000000000100000000 : slv_ip2bus_data <= stat_emax_xh;
        32'b00000000000000000000000010000000 : slv_ip2bus_data <= stat_eprobe_xl;
        32'b00000000000000000000000001000000 : slv_ip2bus_data <= stat_eprobe_xh;
        32'b00000000000000000000000000100000 : slv_ip2bus_data <= stat_missed_steps;
        32'b00000000000000000000000000010000 : slv_ip2bus_data <= reg_stop_delay;
        32'b00000000000000000000000000001000 : slv_ip2bus_data <= reg_probe_delay;
        32'b00000000000000000000000000000100 : slv_ip2bus_data <= stat_min_bounce;
        32'b00000000000000000000000000000010 : slv_ip2bus_data <= stat_max_bounce;
        32'b00000000000000000000000000000001 : slv_ip2bus_data <= stat_probe_bounce;
        default : slv_ip2bus_data <= 0;
      endcase

    end // SLAVE_REG_READ_PROC

  // ------------------------------------------------------------
  // Example code to drive IP to Bus signals
  // ------------------------------------------------------------

  always @(slv_ip2bus_data)
    for ( bit_index = 0; bit_index <= C_SLV_DWIDTH; bit_index = bit_index+1 )
	    IP2Bus_Data[bit_index] <= slv_ip2bus_data[bit_index];
		 
  assign IP2Bus_WrAck   = slv_write_ack;
  assign IP2Bus_RdAck   = slv_read_ack;
  assign IP2Bus_Error   = 0;

	acc_step_gen acc_step_gen (
    .clk(Bus2IP_Clk), 
    .reset(Bus2IP_Reset), 
    .dt_val(reg_next_dt), 
    .steps_val(reg_next_steps), 
    .load(load), 
    .steps(stat_steps), 
    .dt(stat_dt), 
    .stopped(stopped), 
    .step_stb(step_stb), 
    .done(done)
    );

  assign stat_status[0] = stopped;
  assign stat_status[31:1] = 0;
  assign load = reg_cmd[0] && (done || stopped);

endmodule
