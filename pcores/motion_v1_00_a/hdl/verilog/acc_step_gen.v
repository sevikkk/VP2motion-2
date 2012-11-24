`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:53:53 11/18/2012 
// Design Name: 
// Module Name:    acc_step_gen 
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
module acc_step_gen(
    input clk,
    input reset,
    input [31:0] dt_val,
    input [31:0] steps_val,
    input load,
    output reg [31:0] steps,
    output reg [31:0] dt,
    output reg stopped,
    output reg step_stb, 	// combinatorial!
    output reg done 		// combinatorial!
    );

reg [31:0] dt_limit;
reg [31:0] steps_limit;

reg [31:0] next_dt;
reg [31:0] next_steps;
reg next_stopped;

always @(posedge clk)
	begin
		if (reset)
			begin
				dt_limit <= 0;
				steps_limit <= 0;
			end
		else if (load)
			begin
				dt_limit <= dt_val;
				steps_limit <= steps_val;
			end
	end
	
always @(load, dt_limit, steps_limit, steps, dt, reset, stopped)
	begin
		if (reset)
			begin
				next_steps <= 0;
				next_dt <= 0;
				next_stopped <= 1;
			end
		else if (load)
			begin
				next_steps <= 0;
				next_dt <= 0;
				next_stopped <= 0;
			end
		else if (stopped)
			begin
				next_steps <= 0;
				next_dt <= 0;
				next_stopped <= 1;
			end
		else
			begin
				next_steps <= steps;
				next_dt <= dt + 1;
				next_stopped <= 0;
				if (dt >= dt_limit - 1)
					begin
						next_dt <= 0;
						next_steps <= steps + 1;
						if (steps >= steps_limit - 1)
							begin
								next_stopped <= 1;
							end
					end
			end
	end

always @(dt_limit, steps_limit, steps, dt, reset, stopped)
	begin
		if (reset)
			begin
				done <= #3 0;
				step_stb <= #3 0;
			end
		else if (stopped)
			begin
				done <= #3 0;
				step_stb <= #3 0;
			end
		else
			begin
				done <= #3 0;
				step_stb <= #3 0;
				if (dt >= dt_limit - 1)
					begin
						step_stb <= #3 1;
						if (steps >= steps_limit - 1)
							begin
								done <= #3 1;
							end
					end
			end
	end

always @(posedge clk)
	begin
		dt <= next_dt;
		steps <= next_steps;
		stopped <= next_stopped;
	end

endmodule
