`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   17:52:32 11/26/2012
// Design Name:   acc_profile_gen
// Module Name:   /homes/robin/seva/src/xc2p_pe1/pcores/motion_v1_00_a/devl/projnav//test_acc_profile_gen.v
// Project Name:  motion
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: acc_profile_gen
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module test_acc_profile_gen;

	// Inputs
	reg clk;
	reg reset;
	reg acc_step;
	reg load;
	reg set_x;
	reg set_v;
	reg set_a;
	reg set_j;
	reg [63:0] x_val;
	reg [31:0] v_val;
	reg [31:0] a_val;
	reg [31:0] j_val;
	reg [5:0] step_bit;

	// Outputs
	wire [63:0] x;
	wire [31:0] v;
	wire [31:0] a;
	wire [31:0] j;
	wire step;
	wire dir;

	// Instantiate the Unit Under Test (UUT)
	acc_profile_gen uut (
		.clk(clk), 
		.reset(reset), 
		.acc_step(acc_step), 
		.load(load), 
		.set_x(set_x), 
		.set_v(set_v), 
		.set_a(set_a), 
		.set_j(set_j), 
		.x_val(x_val), 
		.v_val(v_val), 
		.a_val(a_val), 
		.j_val(j_val), 
		.step_bit(step_bit), 
		.x(x), 
		.v(v), 
		.a(a), 
		.j(j), 
		.step(step), 
		.dir(dir)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		reset = 1;
		acc_step = 0;
		load = 0;
		set_x = 0;
		set_v = 0;
		set_a = 0;
		set_j = 0;
		x_val = 0;
		v_val = 0;
		a_val = 0;
		j_val = 0;
		step_bit = 0;

		// Wait 100 ns for global reset to finish
		#99;
        
		// Add stimulus here
		reset = 0;
		#20;
		
		set_v = 1;
		v_val = 5;
		set_a = 1;
		a_val = 1;
		step_bit = 6;
		load = 1;
		
		#10;
		load = 0;
		
		#100;
		acc_step = 1;
		#10;
		acc_step = 0;
		
		#100;
		acc_step = 1;
		#10;
		acc_step = 0;
		
		#100;
		acc_step = 1;
		#10;
		acc_step = 0;
		
		#100;
		acc_step = 1;
		#10;
		acc_step = 0;
		

	end
	
	initial begin
		#10;
		forever
			begin
				clk = 1;
				#5;
				clk = 0;
				#5;
			end
	end      
endmodule

