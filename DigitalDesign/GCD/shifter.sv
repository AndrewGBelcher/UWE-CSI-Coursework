module shifter(
	input [7:0]din,
	input [1:0]sh_sel,
	output logic [7:0]dout
);

always@(sh_sel,din)
	begin
		case(sh_sel)
			2'b00 : dout <= din;
			2'b01 : dout <= din << 1;
			2'b10 : dout <= din >> 1;
			2'b11 :	dout <= {din[0], din[7:1]};
		endcase
	end	
endmodule