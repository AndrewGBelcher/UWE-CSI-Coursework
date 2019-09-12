module z80_core
(
	input clk,
	input reset
	//inout logic[7:0]data_bus,
	//output [15:0]addr_bus
);

// main databus/addrbus
wire [7:0]data_bus_int;
logic [15:0]addr_bus_int;

// rf ctl sigs
wire [15:0]WE;
wire [3:0]RE;
wire [7:0]rf_out;
wire rf_read;
// alu ctl sigs
wire [3:0]alu_sel;
wire [7:0]alu_out;

wire [7:0]db_out;
wire [3:0]db_sel;
wire [7:0]cu_out;

// external io from rf/alu
wire [7:0]alu_exio_A;
wire [7:0]alu_exio_F;
wire [7:0]ld_r;
wire [3:0]re_r;
wire [7:0]r_sel;

reg [7:0] program_rom [0:42];

initial 
begin
//$display("Loading rom.");
	$readmemh("F:/DD/rom_image.mem", program_rom);
end

reg [7:0]data_bus;
/*
always@(addr_bus_int)
begin
	data_bus = program_rom[addr_bus_int];
end*/


z80_db_mux db_mux(db_sel, rf_out, cu_out, alu_out, data_bus_int);
z80_cu cu(clk, reset, data_bus, /*addr_bus_int,*/ WE, RE, alu_sel, rf_read, db_sel, cu_out,r_sel); // change cu db input to full databus later
z80_regfile regfile(clk, reset, rf_read, data_bus_int, addr_bus_int, WE, RE, rf_out, ld_r, re_r, alu_exio_A, alu_exio_F);
z80_alu alu(clk,alu_sel,data_bus_int,alu_out,r_sel,ld_r, re_r, alu_exio_A, alu_exio_F);


assign data_bus = program_rom[addr_bus_int];




//assign data_bus_int = db_out;
//assign addr_bus_int = addr_bus;

/*
force clk 0 0, 1 10 -repeat 20
force reset 0
force data_bus 3e
run 20

force data_bus 33
run 20

force data_bus 47
run 20

force data_bus 80
run 20

force data_bus C6
run 20

force data_bus 1
run 20

force data_bus 90
run 20*/

endmodule