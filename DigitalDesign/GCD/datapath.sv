module datapath(
	input [7:0]din,
	input clk,
	input reset,
	input [15:0]cw,
	output logic [7:0]dout_check,
	output logic [7:0]dout
);
	
logic [7:0]alu_out;
logic [7:0]shift_out;
logic [7:0]rf_out_a;
logic [7:0]rf_out_b;
logic [7:0]in_mux_out;	
logic input_en;
logic [1:0]sh;
logic [2:0]alu_sel;
logic WE;
logic [2:0]WA;
logic RAE;
logic [1:0]RAA;
logic RBE;
logic [1:0]RBA;
logic output_en;
	
mux_8 mux_in(.a(din),.b(shift_out),.sel(input_en),.q(in_mux_out));	
rf register_file(.data_in(in_mux_out),
					.reset(reset), 
					.clk(clk), 
					.WE(WE),
					.WA(WA),
					.RAE(RAE),
					.RAA(RAA),
					.RBE(RBE),
					.RBA(RBA),
					.A(rf_out_a),
					.B(rf_out_b));
					
alu alu_ab(.A(rf_out_a), .B(rf_out_b), .alu_in(alu_sel), .dout(alu_out));
shifter shift(.din(alu_out), .sh_sel(sh), .dout(shift_out));
output_enabler output_enabler(.din(shift_out), .output_en(output_en), .dout(dout));

always@(shift_out)
begin
	dout_check = shift_out;
end

always@(cw)
begin
	input_en   <= cw[15];
	WE   <= cw[14];
	WA   <= cw[13:12];
	RAE  <= cw[11];
	RAA  <= cw[10:9];
	RBE  <= cw[8];
	RBA  <= cw[7:6];
	alu_sel = cw[5:3];
	sh  <= cw[2:1];
	output_en   <= cw[0];
end
endmodule