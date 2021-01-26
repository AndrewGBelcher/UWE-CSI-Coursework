module controller(
	input clk,
	input logic reset,
	input logic start,
	input logic zero_flag,
	input logic neg_flag,
	output [15:0]cw,
	output done
);

logic [2:0]bi;
logic [3:0]state_sel;
logic [3:0]pc;
logic [3:0]bt;
logic [3:0]prog_state;
logic [23:0]rom_word;
		
state_controller state_ctr(.start(start), .zero_flag(zero_flag), .neg_flag(neg_flag), .branch_instruction(bi), .sel(state_sel));
program_counter_mux pc_mux(.pc(pc), .branch_target(bt), .sel(state_sel), .state(prog_state));
Rom rom(.address(pc),.q(rom_word));

assign cw = rom_word[16:1];
assign done = rom_word[0];
assign bi = rom_word[23:21];
assign bt = rom_word[20:17];

always@(posedge clk or posedge reset)
begin

	if(reset)
	begin
		pc <= 0;
	end

	else
	begin
		pc <= prog_state;
	end
end
endmodule