`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   16:49:35 11/23/2012
// Design Name:   motor_step_gen
// Module Name:   /homes/robin/seva/src/xc2p_pe1/pcores/motion_v1_00_a/devl/projnav//test_motor_step_gen.v
// Project Name:  motion
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: motor_step_gen
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module test_motor_step_gen;

	// Inputs
	reg clk;
	reg reset;
	reg [31:0] pre_n;
	reg [31:0] pulse_n;
	reg [31:0] post_n;
	reg step_stb;
	reg step_dir;

	// Outputs
	wire step;
	wire dir;
	wire missed;

	// Instantiate the Unit Under Test (UUT)
	motor_step_gen uut (
		.clk(clk), 
		.reset(reset), 
		.pre_n(pre_n), 
		.pulse_n(pulse_n), 
		.post_n(post_n), 
		.step_stb(step_stb), 
		.step_dir(step_dir), 
		.step(step), 
		.dir(dir), 
		.missed(missed)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		reset = 1;
		pre_n = 0;
		pulse_n = 0;
		post_n = 0;
		step_stb = 0;
		step_dir = 0;

		// Wait 100 ns for global reset to finish
		#99;
        
		// Add stimulus here
		reset = 0;
		pre_n = 5;
		pulse_n = 15;
		post_n = 20;

		#100;
		step_stb = 1;
		#10;
		step_stb = 0;

		#300;
		step_dir = 1;
		step_stb = 1;
		#10;
		step_stb = 0;
		
		#100;
		step_stb = 1;
		#10;
		step_stb = 0;

		#300;
		step_stb = 1;
		#10;
		step_stb = 0;

		#300;
		step_stb = 1;
		#10;
		step_stb = 0;

		#300;
		step_stb = 1;
		#10;
		step_stb = 0;

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

