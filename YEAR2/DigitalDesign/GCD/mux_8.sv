module mux_8(
	input [7:0]a,
	input [7:0]b, 
	input sel, 
	output [7:0]q
);

assign q[0] = (a[0] & (sel)) | (b[0] & ~sel);
assign q[1] = (a[1] & (sel)) | (b[1] & ~sel);
assign q[2] = (a[2] & (sel)) | (b[2] & ~sel);
assign q[3] = (a[3] & (sel)) | (b[3] & ~sel);
assign q[4] = (a[4] & (sel)) | (b[4] & ~sel);
assign q[5] = (a[5] & (sel)) | (b[5] & ~sel);
assign q[6] = (a[6] & (sel)) | (b[6] & ~sel);
assign q[7] = (a[7] & (sel)) | (b[7] & ~sel);


endmodule
