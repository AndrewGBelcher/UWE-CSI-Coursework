module d_mux_4in_2sel_1out( 
	input [7:0]a,	
	input [7:0]b,
	input [7:0]c,
	input [7:0]d,
	input [1:0]sel,
	output logic [7:0]q
);
				 
always@(a,b,c,d,sel)
	begin
		case(sel)
			2'b00 :  
				begin
					q <= a;
			   end
			2'b01 :  
				begin
					q <= b;
			   end
			2'b10 :  
				begin
					q <= c;
			   end
			default :  
				begin
					q <= d;
				end
		endcase				
	end							 
endmodule
