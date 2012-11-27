`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   00:33:54 11/19/2012
// Design Name:   user_logic
// Module Name:   /home/seva/src/xc2p_pe1/pcores/motion_v1_00_a/devl/projnav/test_user_logic.v
// Project Name:  motion
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: user_logic
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module test_user_logic;

	// Inputs
	reg E_Min;
	reg E_Max;
	reg E_Probe;
	reg A_Step;
	reg A_Load;
	reg Bus2IP_Clk;
	reg Bus2IP_Reset;
	reg [0:31] Bus2IP_Addr;
	reg [0:0] Bus2IP_CS;
	reg Bus2IP_RNW;
	reg [0:31] Bus2IP_Data;
	reg [0:3] Bus2IP_BE;
	reg [0:31] Bus2IP_RdCE;
	reg [0:31] Bus2IP_WrCE;

	// Outputs
	wire S_Dir;
	wire S_Step;
	wire S_Enable;
	wire [0:31] IP2Bus_Data;
	wire IP2Bus_RdAck;
	wire IP2Bus_WrAck;
	wire IP2Bus_Error;
	wire [0:3] IP2Bus_IntrEvent;

	// Instantiate the Unit Under Test (UUT)
	user_logic uut (
		.S_Dir(S_Dir), 
		.S_Step(S_Step), 
		.S_Enable(S_Enable), 
		.E_Min(E_Min), 
		.E_Max(E_Max), 
		.E_Probe(E_Probe), 
		.A_Step(A_Step), 
		.A_Load(A_Load), 
		.Bus2IP_Clk(Bus2IP_Clk), 
		.Bus2IP_Reset(Bus2IP_Reset), 
		.Bus2IP_Addr(Bus2IP_Addr), 
		.Bus2IP_CS(Bus2IP_CS), 
		.Bus2IP_RNW(Bus2IP_RNW), 
		.Bus2IP_Data(Bus2IP_Data), 
		.Bus2IP_BE(Bus2IP_BE), 
		.Bus2IP_RdCE(Bus2IP_RdCE), 
		.Bus2IP_WrCE(Bus2IP_WrCE), 
		.IP2Bus_Data(IP2Bus_Data), 
		.IP2Bus_RdAck(IP2Bus_RdAck), 
		.IP2Bus_WrAck(IP2Bus_WrAck), 
		.IP2Bus_Error(IP2Bus_Error), 
		.IP2Bus_IntrEvent(IP2Bus_IntrEvent)
	);

task bus_write;
	input [4:0] addr;
	input [31:0] value;
	begin
		$display($time, "Write", addr, value);
		Bus2IP_WrCE[addr] = 1;
		Bus2IP_Data = value;
		#10;
		Bus2IP_WrCE = 0;
	end
endtask

task bus_read;
	input [4:0] addr;
	begin
		Bus2IP_RdCE[addr] = 1;
		#10;
		$display($time, "Read", addr, IP2Bus_Data);
		Bus2IP_RdCE = 0;
	end
endtask

	initial begin
		// Initialize Inputs
		E_Min = 0;
		E_Max = 0;
		E_Probe = 0;
		A_Step = 0;
		A_Load = 0;
		Bus2IP_Clk = 0;
		Bus2IP_Reset = 1;
		Bus2IP_Addr = 0;
		Bus2IP_CS = 0;
		Bus2IP_RNW = 0;
		Bus2IP_Data = 0;
		Bus2IP_BE = 0;
		Bus2IP_RdCE = 0;
		Bus2IP_WrCE = 0;

		// Wait 100 ns for global reset to finish
		#28;
        
		// Add stimulus here
		Bus2IP_Reset = 0;
		Bus2IP_BE = 15;
		
		#30;
		bus_write(8, 3);       // reg_pre_n
		bus_write(9, 5);       // reg_pulse_n
		bus_write(10, 8);      // reg_post_n
		bus_write(11,
			6+32'h80000000);    // reg_step_bit
		
		bus_write(1, 100);     // reg_next_xl
		bus_write(2, 200);     // reg_next_xh
		bus_write(3, 2);       // reg_next_v
		bus_write(4, 2);       // reg_next_a
		bus_write(5, 1);       // reg_next_j
		bus_write(6, 20);      // reg_next_dt
		bus_write(7, 10);      // reg_next_steps
		bus_write(0, 1+4+8);  // reg_cmd
		
		#1000;
		bus_write(4, -1);       // reg_next_a
		bus_write(7, 30);      // reg_next_steps
		bus_write(0, 1+8);      // reg_cmd

		
	end
      
	initial begin
		#10;
		forever
			begin
				Bus2IP_Clk = 1;
				#5;
				Bus2IP_Clk = 0;
				#5;
			end
	end
endmodule

