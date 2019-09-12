module cw1(
	input clk,
	input logic reset,
	input logic start,
	input [7:0]data_in,
	output [7:0]data_out,
	output done
);

logic [15:0]cw;
logic zero_flag;
logic neg_flag;

controller control(.clk(clk), .reset(reset), .start(start), .zero_flag(zero_flag), .neg_flag(neg_flag), .cw(cw), .done(done)); 
full_datapath datapath(.data_in(data_in), .clk(clk), .reset(reset), .CW(cw), .zero_flag(zero_flag), .neg_flag(neg_flag), .data_out(data_out));

endmodule