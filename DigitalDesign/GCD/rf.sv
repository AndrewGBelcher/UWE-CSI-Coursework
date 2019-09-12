module rf (
	input [7:0]data_in,
	input reset,
	input clk,	
	input WE,
	input [2:0]WA,
	input RAE,
	input [1:0]RAA,
	input RBE,
	input [1:0]RBA,
	output logic [7:0]A,
	output logic [7:0]B
);
				
logic [7:0]reg0_out;
logic [7:0]reg1_out;
logic [7:0]reg2_out;
logic [7:0]reg3_out;

logic reg0_we;
logic reg1_we;
logic reg2_we;
logic reg3_we;

d_mux_4_8bit dmux_data(.a(data_in), .sel(WA), .w(reg_0), .x(reg_1), .y(reg_2), .z(reg_3));
d_mux_4_2bit dmux_sel(.a(WE), .sel(WA), .w(reg0_we), .x(reg1_we), .y(reg2_we), .z(reg3_we));

reg_8 register_0(.clk(clk), .reset(reset), .load(reg0_we), .d(data_in), .q(reg0_out));
reg_8 register_1(.clk(clk), .reset(reset), .load(reg1_we), .d(data_in),.q(reg1_out));
reg_8 register_2(.clk(clk), .reset(reset), .load(reg2_we), .d(data_in),.q(reg2_out));
reg_8 register_3(.clk(clk), .reset(reset), .load(reg3_we), .d(data_in),.q(reg3_out));

d_mux_4in_2sel_1out dmux_a_out(.a(reg0_out),.b(reg1_out),.c(reg2_out),.d(reg3_out),.sel(RAA), .q(A));
d_mux_4in_2sel_1out dmux_b_out(.a(reg0_out),.b(reg1_out),.c(reg2_out),.d(reg3_out),.sel(RBA), .q(B));

endmodule