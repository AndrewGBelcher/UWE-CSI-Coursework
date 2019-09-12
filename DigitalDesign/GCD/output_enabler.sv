module output_enabler(
	input [7:0]din,
	input output_en,
	output logic [7:0]dout
);
			
always@(output_en,din)
	begin
		if(output_en)
			begin
				dout <= din;
			end
		else
			begin
				dout <= 0;
			end
	end	
endmodule
					