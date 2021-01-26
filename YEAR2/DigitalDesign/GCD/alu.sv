module alu(
	input [7:0]A,
	input [7:0]B,
	input [2:0]alu_in,
	output logic [7:0]dout
);
		   
always@(alu_in, A,B)		   
	begin
		case(alu_in)
		   3'b000 : 
			begin
			dout = A;
			end
		   3'b001 : 
			begin 
			dout = A & B; 
			end
		   3'b010 : 
			begin 
			dout = A | B; 
			end
		   3'b011 : 
			begin 
			dout = ~A;
			end
		   3'b100 : 
			begin 
			dout = A + B;
			end
		   3'b101 : 
			begin 
			dout = A - B;
			end
		   3'b110 : 
			begin 
			dout = A + 1;
			end
		   3'b111 : 
			begin 
			dout = A - 1;
			end
		endcase
	end	   
		   
endmodule

			