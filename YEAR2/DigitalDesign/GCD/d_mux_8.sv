module d_mux_4_8bit(
	input [7:0]a,
	input [1:0]sel,
	output logic [7:0]w,
	output logic [7:0]x,
	output logic [7:0]y,
	output logic [7:0]z
);
							
always@(a,sel)
	begin		
		case(sel)
			2'b00 : w <= a;				
			2'b01 : x <= a;
			2'b10 : y <= a;
			default : z <= a;
		endcase
	end		
endmodule
					
					