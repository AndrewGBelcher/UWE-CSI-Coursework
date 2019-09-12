module state_controller(
	input start,
	input zero_flag,
	input neg_flag,
	input logic [2:0]branch_instruction,
	output logic [3:0]sel
);

always_comb
begin
	if(~start && branch_instruction == 3'b001)
		sel = 1; // pc = pc
	else if(~zero_flag && branch_instruction == 3'b010)
		sel = 2; // jmp to branch address - bnz branch if not zero
	else if(branch_instruction == 3'b011)
		sel = 3; // reset
	else if(branch_instruction == 3'b100)
		sel = 2; // goto  - branch 
	else if(neg_flag && branch_instruction == 3'b101)
		sel = 2; // branch if negative
	else
		sel = 0; // pc + 1	
end
endmodule