module d_mux_4_2bit(
	input a,
	input [1:0]sel,
	output logic w,
	output logic x,
	output logic y,
	output logic z
);
											
always@(a,sel)
	begin
		case(sel)
			2'b00 : begin
						w <= a;
						x <= 0;
						y <= 0;
						z <= 0;
					 end
			2'b01 : begin
						w <= 0;
						x <= a;
						y <= 0;
						z <= 0;
						end
			2'b10 : begin
						w <= 0;
						x <= 0;
						y <= a;
						z <= 0;
						end
					
			default : begin
						w <= 0;
						x <= 0;
						y <= 0;
						z <= a;
						end
		endcase
	end	
endmodule
					
					