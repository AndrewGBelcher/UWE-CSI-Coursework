module reg_8 (
		input clk,
		input reset,
		input load,
		input logic [7:0]d,
		output logic [7:0]q
	);
	
	always@(posedge clk or posedge reset)
	begin
	
	if(reset)begin
		q <= 0;
	end
	else if (load)begin
		q <= d;
	end	
	else
		begin	
		q <= q;
	end
	end
	
endmodule