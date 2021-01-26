module z80_db_mux(
	input [3:0]sel,
	input [7:0]rf_out,
	input [7:0]cu_out,
	input [7:0]alu_out,
	output logic [7:0]db_out
);

always@(sel,rf_out,cu_out,alu_out)
begin
	case(sel)
		0: db_out <= rf_out;
		1: db_out <= cu_out;
		2: db_out <= alu_out;
		default: db_out <= 0;
	endcase
end

endmodule