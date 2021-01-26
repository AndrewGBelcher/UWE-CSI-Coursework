module status_register(
	input [7:0]din,
	output logic zero_flag,
	output logic neg_flag
);

always@(din)
begin
	if(din == 8'b00000000)
	begin
		zero_flag = 1;
		neg_flag = 0;
	end
	else if(din[7] == 1)
	begin
		neg_flag = 1;
		zero_flag = 0;
	end
	else
	begin
		neg_flag = 0;
		zero_flag = 0;
	end
end
endmodule
