module z80_regbank(
	input clk,
	input reset,
	inout [7:0]data_bus,
	input [15:0]addr_bus,
	input [15:0]WE,
	input [15:0]RE
	);




// 8 bit data registers
reg8 reg_W(.clk(clk), .reset(reset), .WE(WE[0]), .RE(RE[0]), .data_in(data_bus), .data_out(data_bus));
reg8 reg_Z(.clk(clk), .reset(reset), .WE(WE[1]), .RE(RE[1]), .data_in(data_bus), .data_out(data_bus));
reg8 reg_A(.clk(clk), .reset(reset), .WE(WE[2]), .RE(RE[2]), .data_in(data_bus), .data_out(data_bus));
reg8 reg_F(.clk(clk), .reset(reset), .WE(WE[3]), .RE(RE[3]), .data_in(data_bus), .data_out(data_bus));
reg8 reg_B(.clk(clk), .reset(reset), .WE(WE[4]), .RE(RE[4]), .data_in(data_bus), .data_out(data_bus));
reg8 reg_C(.clk(clk), .reset(reset), .WE(WE[5]), .RE(RE[5]), .data_in(data_bus), .data_out(data_bus));
reg8 reg_D(.clk(clk), .reset(reset), .WE(WE[6]), .RE(RE[6]), .data_in(data_bus), .data_out(data_bus));
reg8 reg_E(.clk(clk), .reset(reset), .WE(WE[7]), .RE(RE[7]), .data_in(data_bus), .data_out(data_bus));
reg8 reg_H(.clk(clk), .reset(reset), .WE(WE[8]), .RE(RE[8]), .data_in(data_bus), .data_out(data_bus));
reg8 reg_L(.clk(clk), .reset(reset), .WE(WE[9]), .RE(RE[9]), .data_in(data_bus), .data_out(data_bus));

// 16 bit addressing registers
reg16 reg_IX(.clk(clk), .reset(reset), .WE(WE[10]),.RE(RE[10]),  .data_in(data_bus), .data_out(addr_bus));
reg16 reg_IY(.clk(clk), .reset(reset), .WE(WE[11]),.RE(RE[11]),  .data_in(data_bus), .data_out(addr_bus));
reg16 reg_SP(.clk(clk), .reset(reset), .WE(WE[12]),.RE(RE[12]),  .data_in(data_bus), .data_out(addr_bus));
reg16 reg_PC(.clk(clk), .reset(reset), .WE(WE[13]),.RE(RE[13]),  .data_in(data_bus), .data_out(addr_bus));


/*
reg [7:0]W;
reg [7:0]Z;
reg [7:0]A;
reg [7:0]B;
reg [7:0]C;
reg [7:0]D;
reg [7:0]E;
reg [7:0]H;
reg [7:0]L;

reg [7:0]w;
reg [7:0]z;
reg [7:0]b;
reg [7:0]c;
reg [7:0]d;
reg [7:0]e;
reg [7:0]h;
reg [7:0]l;

reg [7:0]IX;
reg [7:0]IY;
reg [7:0]SP;
reg [7:0]PC;
reg memRead;
reg memWrite;
reg [2:0]regSelect;
reg runOpcode;

always@(posedge clk)
begin
//always@(data_bus,load)
//begin

//
//if(memRead)
//begin


//end

//if(load==1 and memRead==0)
	if(data_bus[7:6]==1)
	begin
		if(runOpcode==0)
		begin
			D=33;
			case(data_bus[2:0])
				3'b111:	  W <= A;
				3'b000:	  W <= B;
				3'b001:	  W <= C;
				3'b010:	  W <= D;
				3'b011:	  W <= E;
				3'b100:	  W <= H;
				3'b101:	  W <= L;
					
				//3'b110:
		//			begin
		//					addr_bus[15:8] = H;
		//					addr_bus[7:0] = L;
		//					W = data_bus;
		//					memRead = 1;
		//			end
			endcase

			case(data_bus[5:3])
				3'b111:	  A <= W;
				3'b000:	  B <= W;
				3'b001:	  C <= W;
				3'b010:	  D <= W;
				3'b011:	  E <= W;
				3'b100:	  H <= W;
				3'b101:	  L <= W;
				
				//3'b110:	;

			endcase
		end
	end
	
	if(data_bus[7:6]==0)
	begin
		if(data_bus[2:0]==3'b110)
		begin
			case(data_bus[5:3])
				3'b111:	  regSelect = 3'b111;
				3'b000:	  regSelect = 3'b000;
				3'b001:	  regSelect = 3'b001;
				3'b010:	  regSelect = 3'b010;
				3'b011:	  regSelect = 3'b011;
				3'b100:	  regSelect = 3'b100;
				3'b101:	  regSelect = 3'b101;
			endcase
			runOpcode=1;
		end
		
		
	end		


	if(data_bus[7:6]==3)
	begin
		if(runOpcode==0)
		begin
			if(data_bus[5:3]==3'b111)
			begin
				if(data_bus[2:0]==3'b001)
				begin
					//LD SP, HL
					SP[15:8] = H;
					SP[7:0] = L;
				end
			end
			
			if(data_bus[5:3]==3'b011)
			begin
				if(data_bus[2:0]==3'b101)
				begin
					// LD SP, IX
				end
			end
		end
	end
	

	if(regSelect!=3'b110)
	begin
		if(runOpcode==1)
		begin
			case(regSelect)
				3'b111:	  A <= data_bus;
				3'b000:	  B <= data_bus;
				3'b001:	  C <= data_bus;
				3'b010:	  D <= data_bus;
				3'b011:	  E <= data_bus;
				3'b100:	  H <= data_bus;
				3'b101:	  L <= data_bus;
			endcase
			regSelect=3'b110;
			runOpcode=0;
		end
	end
end

assign W_sig = W;
assign Z_sig = Z;
assign A_sig = A;
assign B_sig = B;
assign C_sig = C;
assign D_sig = D;
assign E_sig = E;
assign H_sig = H;
assign L_sig = L;

*/

endmodule