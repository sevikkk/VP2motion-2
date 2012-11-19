`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   21:45:50 11/18/2012
// Design Name:   acc_step_gen
// Module Name:   /home/seva/src/xc2p_pe1/pcores/motion_v1_00_a/devl/projnav/test_ac_step_gen.v
// Project Name:  motion
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: acc_step_gen
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module test_ac_step_gen;

	// Inputs
	reg clk;
	reg reset;
	reg [31:0] dt_val;
	reg [31:0] steps_val;
	reg load;

	// Outputs
	wire [31:0] steps;
	wire [31:0] dt;
	wire stopped;
	wire step_stb;
	wire done;
	wire load1;

	assign load1 = done || load;

	// Instantiate the Unit Under Test (UUT)
	acc_step_gen uut (
		.clk(clk), 
		.reset(reset), 
		.dt_val(dt_val), 
		.steps_val(steps_val), 
		.load(load1), 
		.steps(steps), 
		.dt(dt), 
		.stopped(stopped), 
		.step_stb(step_stb), 
		.done(done)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		reset = 1;
		dt_val = 0;
		steps_val = 0;
		load = 0;

		// Wait 100 ns for global reset to finish
		#20;
        
		// Add stimulus here
		reset = 0;
		
		#54;
		dt_val = 20;
		steps_val = 20;
		load = 1;
		#9;
		load = 0;
		
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

