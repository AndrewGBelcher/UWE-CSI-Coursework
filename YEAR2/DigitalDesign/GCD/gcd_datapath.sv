module gcd_datapath(input [7:0]data_in,
						  input clk,
						  input reset,
						  input [15:0]CW,
						  output zero_flag,
						  output [7:0]data_out
						  );
						  
						  
logic [7:0]dout_eval;
						  
datapath core_datapath(.din(data_in), .clk(clk), .reset(reset), .cw(CW), .dout_check(dout_eval), .dout(data_out));
status_register status_reg(.din(dout_eval), .zero_flag(zero_flag));

						  
endmodule