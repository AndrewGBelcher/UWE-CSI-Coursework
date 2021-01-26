module program_counter_mux(
			input [3:0]pc,
			input [3:0]branch_target,
			input [3:0]sel,
			output logic [3:0]state
);

always@(pc,branch_target,sel)
begin
	case(sel)
		4'b0000: state = pc + 1;
		4'b0001: state = pc; 
		4'b0010: state = branch_target; 
		default: state = 0;
	endcase
end
endmodule