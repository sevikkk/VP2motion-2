`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    16:25:59 11/23/2012 
// Design Name: 
// Module Name:    motor_step_gen 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module motor_step_gen(
    input clk,
	 input reset,
    input [31:0] pre_n,
    input [31:0] pulse_n,
    input [31:0] post_n,
    input step_stb,
    input step_dir,
    output reg step,
    output reg dir,
    output reg missed
    );
	 
reg [31:0] cnt;
reg [31:0] next_cnt;
reg next_dir;
reg next_step;
reg next_missed;

always @(reset, pre_n, pulse_n, post_n, step_stb, step_dir, cnt, dir)
	begin
		next_cnt <= 0;
		next_dir <= dir;
		next_step <= 0;
		next_missed <= 0;

		if (reset)
			begin
				next_dir <= 0;
			end
		else if (cnt == 0)
			begin
				if (step_stb)
					begin
						next_dir <= step_dir;
						next_cnt <= 1;
					end
			end
		else
			begin
				if (step_stb)
					next_missed <= 1;
				next_cnt <= cnt + 1;
				if (cnt < pre_n)
					next_step <= 0;
				else if (cnt < pulse_n)
					next_step <= 1;
				else if (cnt < post_n)
					next_step <= 0;
				else
					next_cnt <= 0;
			end
	end
	
always @(posedge clk)
	begin
		cnt <= next_cnt;
		dir <= next_dir;
		step <= next_step;
		missed <= next_missed;
	end

endmodule
