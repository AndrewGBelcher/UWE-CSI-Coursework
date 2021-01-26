module z80_regfile(
	input clk,
	input reset,
	input read,
	input [7:0]data_bus,
	output logic[15:0]addr_bus,
	input [15:0]WE,
	input [3:0]RE,
	output [7:0]regfile_data_out,
	input [7:0]alu_ld_r,
	input [3:0]alu_re_r,
	inout logic[7:0]alu_R,
	inout logic [7:0]alu_F
	);


reg [7:0]regfile_out = 0;
reg [7:0]alu_RE_reg = 0;

wire[7:0]reg_W_out;
wire[7:0]reg_Z_out;
wire[7:0]reg_A_out;
wire[7:0]reg_F_out;
wire[7:0]reg_B_out;
wire[7:0]reg_C_out;
wire[7:0]reg_D_out;
wire[7:0]reg_E_out;
wire[7:0]reg_L_out;

wire[15:0]reg_IX_out;
wire[15:0]reg_IY_out;
wire[15:0]reg_SP_out;
logic[15:0]reg_PC_out;

logic [7:0]A_in;
logic [7:0]F_in;
logic [7:0]B_in;
logic [7:0]C_in;
logic [7:0]D_in;
logic [7:0]E_in;
logic [7:0]H_in;
logic [7:0]L_in;
logic [15:0]PC_in=0;



wire A_WE;
wire F_WE;
wire B_WE;
wire C_WE;
wire D_WE;
wire E_WE;
wire H_WE;
wire L_WE;

// 8 bit data registers
reg8 reg_A(.clk(clk), .reset(reset), .WE(A_WE), .data_in(A_in), .data_out(reg_A_out));
reg8 reg_F(.clk(clk), .reset(reset), .WE(F_WE), .data_in(F_in), .data_out(reg_F_out));
reg8 reg_B(.clk(clk), .reset(reset), .WE(B_WE), .data_in(B_in), .data_out(reg_B_out));
reg8 reg_C(.clk(clk), .reset(reset), .WE(C_WE), .data_in(C_in), .data_out(reg_C_out));
reg8 reg_D(.clk(clk), .reset(reset), .WE(D_WE), .data_in(D_in), .data_out(reg_D_out));
reg8 reg_E(.clk(clk), .reset(reset), .WE(E_WE), .data_in(E_in), .data_out(reg_E_out));
reg8 reg_H(.clk(clk), .reset(reset), .WE(H_WE), .data_in(H_in), .data_out(reg_H_out));
reg8 reg_L(.clk(clk), .reset(reset), .WE(L_WE), .data_in(L_in), .data_out(reg_L_out));

// 16 bit addressing registers
reg16 reg_IX(.clk(clk), .reset(reset), .WE(WE[10]), .data_in(data_bus), .data_out(reg_IX_out));
reg16 reg_IY(.clk(clk), .reset(reset), .WE(WE[11]), .data_in(data_bus), .data_out(reg_IY_out));
reg16 reg_SP(.clk(clk), .reset(reset), .WE(WE[12]), .data_in(data_bus), .data_out(reg_SP_out));
reg16 reg_PC(.clk(clk), .reset(reset), .WE(1), .data_in(PC_in), .data_out(reg_PC_out));

always@(read,RE, clk)
begin
	if(read==1)
	begin
		case(RE)
			0: regfile_out <= reg_A_out;
			1: regfile_out <= reg_F_out;
			2: regfile_out <= reg_B_out;
			3: regfile_out <= reg_C_out;
			4: regfile_out <= reg_D_out;
			5: regfile_out <= reg_E_out;
			6: regfile_out <= reg_H_out;
			7: regfile_out <= reg_L_out;
			default: regfile_out <= 0;
		endcase
	end

	else
	begin
		regfile_out <= 0;
	end
end

always@(alu_re_r, clk)
begin

		case(alu_re_r)
			1: alu_RE_reg <= reg_F_out;
			2: alu_RE_reg <= reg_B_out;
			3: alu_RE_reg <= reg_C_out;
			4: alu_RE_reg <= reg_D_out;
			5: alu_RE_reg <= reg_E_out;
			6: alu_RE_reg <= reg_H_out;
			7: alu_RE_reg <= reg_L_out;
			default: alu_RE_reg <= reg_A_out;
		endcase
end


always@(posedge clk)
begin

	PC_in <= reg_PC_out + 1;
end

assign addr_bus = reg_PC_out;

assign A_in = alu_ld_r[0] ? alu_R : data_bus;
assign F_in = alu_ld_r[7] ? alu_F : data_bus;
assign B_in = alu_ld_r[1] ? alu_R : data_bus;
assign C_in = alu_ld_r[2] ? alu_R : data_bus;
assign D_in = alu_ld_r[3] ? alu_R : data_bus;
assign E_in = alu_ld_r[4] ? alu_R : data_bus;
assign H_in = alu_ld_r[5] ? alu_R : data_bus;
assign L_in = alu_ld_r[6] ? alu_R : data_bus;

assign alu_R = alu_ld_r ? 'z : alu_RE_reg;
assign alu_F = alu_ld_r ? 'z : reg_F_out; 

assign regfile_data_out = regfile_out;
assign A_WE = alu_ld_r[0] ? 1 : WE[2];
assign F_WE = alu_ld_r[7] ? 1 : WE[3];
assign B_WE = alu_ld_r[1] ? 1 : WE[4];
assign C_WE = alu_ld_r[2] ? 1 : WE[5];
assign D_WE = alu_ld_r[3] ? 1 : WE[6];
assign E_WE = alu_ld_r[4] ? 1 : WE[7];
assign H_WE = alu_ld_r[5] ? 1 : WE[8];
assign L_WE = alu_ld_r[6] ? 1 : WE[9];

endmodule