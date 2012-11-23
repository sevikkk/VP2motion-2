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
		Bus2IP_WrCE = 32'b00000010000000000000000000000000;
		Bus2IP_Data = 20;
		#10;
		Bus2IP_WrCE = 32'b00000000000000000000000000000000;
		#30;
		Bus2IP_WrCE = 32'b00000001000000000000000000000000;
		Bus2IP_Data = 10;
		#10;
		Bus2IP_WrCE = 32'b00000000000000000000000000000000;
		#30;
		Bus2IP_WrCE = 32'b10000000000000000000000000000000;
		Bus2IP_Data = 1;
		#10;
		Bus2IP_WrCE = 32'b00000000000000000000000000000000;
		#300;
		Bus2IP_WrCE = 32'b10000000000000000000000000000000;
		Bus2IP_Data = 1;
		#10;
		Bus2IP_WrCE = 32'b00000000000000000000000000000000;
		#30;
		Bus2IP_RdCE = 32'b10000000000000000000000000000000;
		#10;
		Bus2IP_RdCE = 32'b00000000000000000000000000000000;
		#30;
		Bus2IP_RdCE = 32'b00000000000000000001000000000000;
		#10;
		Bus2IP_RdCE = 32'b00000000000000000000000000000000;
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

