module z80_alu(
	input clk,
	input [3:0]sel,
	input [7:0]data_bus,
	output [7:0]alu_out,
	input logic[7:0]r_sel,
	output logic [7:0]rf_ld_r,
	output logic [3:0]rf_re_r,
	inout logic [7:0]rf_A,
	inout logic[7:0]rf_F
	 
);

reg signed[15:0]tmp = 0;
reg signed[7:0]D = 0;
reg [15:0]A = 0;
reg [7:0]F = 0;
reg [7:0]rf_A_reg = 0;

always@(data_bus, sel, posedge clk)
begin
	//rf_ld_r[7:0] = r_sel[7:0];

	case(sel)
	// add r(databus) to A
	0:
	begin
		A = rf_A + data_bus;
	
		//	     7 6 5 4 3 2  1 0
		// set flags S Z X H X PV N C
		// set Carry flag 0
//////////////////////////////		
		if(A[8]==1)
		begin
			F[0] = 1;
		end
		
		else
		begin
			F[0] = 0;
		end

		// set ADD/SUB flag 1
		// reset N flag
//////////////////////////////		
		F[1]= 0;	
		

		// set Overflow/Parity flag 2
		// overflow
//////////////////////////////
		if((A[7]==0) & (rf_A[7]== 1) & (data_bus[7]==1))
		begin
			F[2]=1;
		end

		else
		begin
			F[2]=0;
		end

		// set Half carry flag 4
//////////////////////////////		
		if(A[4]==1)
		begin
			F[4]=1;
		end
		else
		begin
			F[4]=0;
		end

		// set Zero flag 6
//////////////////////////////				
		if(A==0)
		begin
			F[6] = 1;
		end	
		else		
		begin
			F[6] = 0;
		end
			
		// set Signed flag 7
//////////////////////////////				
		if(A[7]==1)
		begin
			F[7] = 1;
		end	
		else
		begin
			F[7] = 0;
		end
		

		rf_ld_r[0] <= 1;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 1;

		rf_re_r <= 0;


	end
	
	
	// subtract A - s(databus)
	1:
	begin
		/*tmp = data_bus;
		D = rf_A;
		A = D - tmp;
		*/

		//D = 0;
		//tmp = 0;


	


		tmp[7:0]= ~data_bus;
		D = rf_A;
		A[7:0] = rf_A + (tmp+1);

		


		//	         7 6 5 4 3 2  1 0
		// set flags S Z X H X PV N C
		// set Carry flag 0
//////////////////////////////		
		if((D[0]==0) & (tmp[0]==1))
		begin
			F[0] = 1;
		end
		
		else
		begin
			F[0] = 0;
		end
		// set ADD/SUB flag 1
		// reset N flag
//////////////////////////////		
		F[1]= 1;	
		

		// set Overflow/Parity flag 2
		// overflow
//////////////////////////////

		//// undeflow
		if((A[7]==1) & (rf_A[7]==0) & (data_bus[7]==0))
		begin
			F[2]=1;
		end

		else
		begin
			F[2]=0;
		end

		// set Half carry flag 4
//////////////////////////////		
		if(A[4]==1)
		begin
			F[4]=1;
		end
		else
		begin
			F[4]=0;
		end

		// set Zero flag 6
//////////////////////////////				
		if(A==0)
		begin
			F[6] = 1;
		end	
		else		
		begin
			F[6] = 0;
		end
			
		// set Signed flag 7
//////////////////////////////				
		if(A[7]==1)
		begin
			F[7] = 1;
		end	
		else
		begin
			F[7] = 0;
		end
		

		rf_ld_r[0] <= 1;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 1;

		rf_re_r <= 0;

		//F = 8'b10101010;
	end
	
	2:
	// subtract A - s(databus) with carry
	begin

		tmp[7:0]= ~data_bus;
		D = rf_A;
		A[7:0] = rf_A + (tmp+1) + F[0];
		

		//	         7 6 5 4 3 2  1 0
		// set flags S Z X H X PV N C
		// set Carry flag 0
//////////////////////////////		
		if((D[0]==0) & (tmp[0]==1))
		begin
			F[0] = 1;
		end
		
		else
		begin
			F[0] = 0;
		end

		// set ADD/SUB flag 1
		// set N flag
//////////////////////////////		
		F[1]= 1;	
		

		// set Overflow/Parity flag 2
		// overflow
//////////////////////////////

		//// undeflow
		if((A[7]==1) & (rf_A[7]==0) & (data_bus[7]==0))
		begin
			F[2]=1;
		end

		else
		begin
			F[2]=0;
		end

		// set Half carry flag 4
//////////////////////////////		
		if(A[4]==1)
		begin
			F[4]=1;
		end
		else
		begin
			F[4]=0;
		end

		// set Zero flag 6
//////////////////////////////				
		if(A==0)
		begin
			F[6] = 1;
		end	
		else		
		begin
			F[6] = 0;
		end
			
		// set Signed flag 7
//////////////////////////////				
		if(A[7]==1)
		begin
			F[7] = 1;
		end	
		else
		begin
			F[7] = 0;
		end
		

		rf_ld_r[0] <= 1;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 1;

		rf_re_r <= 0;
		
	end
		
	3:
	// ADC add with carry
	begin
		A = rf_A + data_bus+F[0];

		//	     7 6 5 4 3 2  1 0
		// set flags S Z X H X PV N C
		// set Carry flag 0
//////////////////////////////		
		if(A[8]==1)
		begin
			F[0] = 1;
		end
		
		else
		begin
			F[0] = 0;
		end

		// set ADD/SUB flag 1
		// set N flag
//////////////////////////////		
		F[1]= 1;	
		

		// set Overflow/Parity flag 2
		// overflow
//////////////////////////////
		if((A[7]==0) & (rf_A[7]== 1) & (data_bus[7]==1))
		begin
			F[2]=1;
		end

		else
		begin
			F[2]=0;
		end

		// set Half carry flag 4
//////////////////////////////		
		if(A[4]==1)
		begin
			F[4]=1;
		end
		else
		begin
			F[4]=0;
		end

		// set Zero flag 6
//////////////////////////////				
		if(A==0)
		begin
			F[6] = 1;
		end	
		else		
		begin
			F[6] = 0;
		end
			
		// set Signed flag 7
//////////////////////////////				
		if(A[7]==1)
		begin
			F[7] = 1;
		end	
		else
		begin
			F[7] = 0;
		end
		

		rf_ld_r[0] <= 1;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 1;
		rf_re_r <= 0;
	end

	4:
	// AND A, databus
	begin

		
	
		D = data_bus;
		tmp[7:0] = rf_A;
	
		if(D[0] && tmp[0])
		begin
			A[0] = 1;
		end
		else
		begin
			A[0] = 0;
		end
		if(D[1] && tmp[1])
		begin
			A[1] = 1;
		end
		else
		begin
			A[1] = 0;
		end
		if(D[2] && tmp[2])
		begin
			A[2] = 1;
		end
		else
		begin
			A[2] = 0;
		end
		if(D[3] && tmp[3])
		begin
			A[3] = 1;
		end
		else
		begin
			A[3] = 0;
		end

		if(D[4] && tmp[4])
		begin
			A[4] = 1;
		end
		else
		begin
			A[4] = 0;
		end
		if(D[5] && tmp[5])
		begin
			A[5] = 1;
		end
		else
		begin
			A[5] = 0;
		end

		if(D[6] && tmp[6])
		begin
			A[6] = 1;
		end
		else
		begin
			A[6] = 0;
		end

		if(D[7] && tmp[7])
		begin
			A[7] = 1;
		end
		else
		begin
			A[7] = 0;
		end

		/*//	     7 6 5 4 3 2  1 0
		// set flags S Z X H X PV N C
		// set Carry flag 0
//////////////////////////////		
		F[0] = 0;

		// set ADD/SUB flag 1
		// reset N flag
//////////////////////////////		

		F[1] = 0; 		

		// set Overflow/Parity flag 2
		// overflow
//////////////////////////////
		if((A[7]==0) & (rf_A[7]== 1) & (data_bus[7]==1))
		begin
			F[2]=1;
		end

		else
		begin
			F[2]=0;
		end

		// set Half carry flag 4
//////////////////////////////		

		F[4]=1;


		// set Zero flag 6
//////////////////////////////				
		if(A==0)
		begin
			F[6] = 1;
		end	
		else		
		begin
			F[6] = 0;
		end
			
		// set Signed flag 7
//////////////////////////////				
		if(A[7]==1)
		begin
			F[7] = 1;
		end	
		else
		begin
			F[7] = 0;
		end
		


		rf_ld_r[0] <= 1;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 1;
		rf_re_r <= 0;*/


	end

	5:
	// OR A, databus
	begin
		A = rf_A | data_bus;


		//	     7 6 5 4 3 2  1 0
		// set flags S Z X H X PV N C
		// set Carry flag 0
//////////////////////////////		
		F[0] = 0;

		// set ADD/SUB flag 1
		// reset N flag
//////////////////////////////		

		F[1] = 0; 		

		// set Overflow/Parity flag 2
		// overflow
//////////////////////////////
		if((A[7]==0) & (rf_A[7]== 1) & (data_bus[7]==1))
		begin
			F[2]=1;
		end

		else
		begin
			F[2]=0;
		end

		// set Half carry flag 4
//////////////////////////////		

		F[4]=1;


		// set Zero flag 6
//////////////////////////////				
		if(A==0)
		begin
			F[6] = 1;
		end	
		else		
		begin
			F[6] = 0;
		end
			
		// set Signed flag 7
//////////////////////////////				
		if(A[7]==1)
		begin
			F[7] = 1;
		end	
		else
		begin
			F[7] = 0;
		end

		rf_ld_r[0] <= 1;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 1;
		rf_re_r <= 0;


	end

	6:
	// XOR A, databus
	begin


		D = data_bus;
		tmp[7:0] = rf_A;
		// Being very explicit since it wasnt working with x ^ x
		if(((D[0]==1)&(tmp[0]==1)) | ((D[0]==0)&(tmp[0]==0))  )
		begin
			A[0] = 0;
		end
		else
		begin
			A[0] = 1;
		end

		if(((D[1]==1)&(tmp[1]==1)) | ((D[1]==0)&(tmp[1]==0))  )
		begin
			A[1] = 0;
		end
		else
		begin
			A[1] = 1;
		end
		if(((D[2]==1)&(tmp[2]==1)) | ((D[2]==0)&(tmp[2]==0))  )
		begin
			A[2] = 0;
		end
		else
		begin
			A[2] = 1;
		end
		if(((D[3]==1)&(tmp[3]==1)) | ((D[3]==0)&(tmp[3]==0))  )
		begin
			A[3] = 0;
		end
		else
		begin
			A[3] = 1;
		end
		if(((D[4]==1)&(tmp[4]==1)) | ((D[4]==0)&(tmp[4]==0))  )
		begin
			A[4] = 0;
		end
		else
		begin
			A[4] = 1;
		end
		if(((D[5]==1)&(tmp[5]==1)) | ((D[5]==0)&(tmp[5]==0))  )
		begin
			A[5] = 0;
		end
		else
		begin
			A[5] = 1;
		end
		if(((D[6]==1)&(tmp[6]==1)) | ((D[6]==0)&(tmp[6]==0))  )
		begin
			A[6] = 0;
		end
		else
		begin
			A[6] = 1;
		end

		if(((D[7]==1)&(tmp[7]==1)) | ((D[7]==0)&(tmp[7]==0))  )
		begin
			A[7] = 0;
		end
		else
		begin
			A[7] = 1;
		end

		//	     7 6 5 4 3 2  1 0
		// set flags S Z X H X PV N C
		// set Carry flag 0
//////////////////////////////		
		F[0] = 0;

		// set ADD/SUB flag 1
		// reset N flag
//////////////////////////////		

		F[1] = 0; 		

		// set Overflow/Parity flag 2
		// overflow
//////////////////////////////

		// set Half carry flag 4
//////////////////////////////		

		F[4]=1;


		// set Zero flag 6
//////////////////////////////				
		if(A==0)
		begin
			F[6] = 1;
		end	
		else		
		begin
			F[6] = 0;
		end
			
		// set Signed flag 7
//////////////////////////////				
		if(A[7]==1)
		begin
			F[7] = 1;
		end	
		else
		begin
			F[7] = 0;
		end
		


		rf_ld_r[0] <= 1;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 1;
		rf_re_r <= 0;


	end

	7:
	// CP A, databus
	begin

		tmp = data_bus;
		D = rf_A;	
		A = D - tmp;

		//	     7 6 5 4 3 2  1 0
		// set flags S Z X H X PV N C
		// set Carry flag 0
//////////////////////////////		
		//F[0] = 0;
		if((rf_A[0]==0) & (tmp[0]==1))
		begin
			F[0] = 1;
		end
		
		else
		begin
			F[0] = 0;
		end

		// set ADD/SUB flag 1
		// reset N flag
//////////////////////////////		

		F[1] = 1; 		

		// set Overflow/Parity flag 2
		// overflow
//////////////////////////////
		if((A[7]==0) & (rf_A[7]== 1) & (data_bus[7]==1))
		begin
			F[2]=1;
		end

		else
		begin
			F[2]=0;
		end

		// set Half carry flag 4
//////////////////////////////		

		//F[4]=1;


		// set Zero flag 6
//////////////////////////////				
		if(A==0)
		begin
			F[6] = 1;
		end	
		else		
		begin
			F[6] = 0;
		end
			
		// set Signed flag 7
//////////////////////////////				
		if(A[7]==1)
		begin
			F[7] = 1;
		end	
		else
		begin
			F[7] = 0;
		end
		


		rf_ld_r[0] <= 0;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 1;

		rf_re_r <= 0;

	end

	8:
	// INC r(r_sel), databus
	begin

		tmp = data_bus;
		A <= data_bus + 1;

		//	     7 6 5 4 3 2  1 0
		// set flags S Z X H X PV N C


		// set ADD/SUB flag 1
		// reset N flag
//////////////////////////////		

		F[1] = 1; 		

		// set Overflow/Parity flag 2
		// overflow
//////////////////////////////
		if(tmp == 8'b10000000)
		begin
			F[2]=1;
		end

		else
		begin
			F[2]=0;
		end

		// set Half carry flag 4
//////////////////////////////		

		//F[4]=1;


		// set Zero flag 6
//////////////////////////////				
		if(A==0)
		begin
			F[6] = 1;
		end	
		else		
		begin
			F[6] = 0;
		end
			
		// set Signed flag 7
//////////////////////////////				
		if(A[7]==1)
		begin
			F[7] = 1;
		end	
		else
		begin
			F[7] = 0;
		end


		rf_ld_r[6:0] <= r_sel;
		//rf_re_r[6:0] <= r_sel;
		
		if(r_sel[0]==1)
		begin
			rf_re_r <= 0;
		end
		else if(r_sel[1]==1)
		begin
			rf_re_r <= 1;
		end
		else if(r_sel[2]==1)
		begin
			rf_re_r <= 2;
		end

		else if(r_sel[3]==1)
		begin
			rf_re_r <= 3;
		end
		else if(r_sel[4]==1)
		begin
			rf_re_r <= 4;
		end
		else if(r_sel[5]==1)
		begin
			rf_re_r <= 5;
		end
		else if(r_sel[6]==1)
		begin
			rf_re_r <= 6;
		end
		else if(r_sel[7]==1)
		begin
			rf_re_r <= 7;
		end	

	end


	9:
	// DEC r(r_sel), databus
	begin

		tmp = data_bus;
		A <= data_bus - 1;

		//	     7 6 5 4 3 2  1 0
		// set flags S Z X H X PV N C


		// set ADD/SUB flag 1
		// reset N flag
//////////////////////////////		

		F[1] = 1; 		

		// set Overflow/Parity flag 2
		// overflow
//////////////////////////////
		if(tmp == 8'b10000000)
		begin
			F[2]=1;
		end

		else
		begin
			F[2]=0;
		end

		// set Half carry flag 4
//////////////////////////////		

		//F[4]=1;


		// set Zero flag 6
//////////////////////////////				
		if(A==0)
		begin
			F[6] = 1;
		end	
		else		
		begin
			F[6] = 0;
		end
			
		// set Signed flag 7
//////////////////////////////				
		if(A[7]==1)
		begin
			F[7] = 1;
		end	
		else
		begin
			F[7] = 0;
		end
		



		rf_ld_r[6:0] <= r_sel;
		//rf_re_r[6:0] <= r_sel;
		
		if(r_sel[0]==1)
		begin
			rf_re_r <= 0;
		end
		else if(r_sel[1]==1)
		begin
			rf_re_r <= 1;
		end
		else if(r_sel[2]==1)
		begin
			rf_re_r <= 2;
		end

		else if(r_sel[3]==1)
		begin
			rf_re_r <= 3;
		end
		else if(r_sel[4]==1)
		begin
			rf_re_r <= 4;
		end
		else if(r_sel[5]==1)
		begin
			rf_re_r <= 5;
		end
		else if(r_sel[6]==1)
		begin
			rf_re_r <= 6;
		end
		else if(r_sel[7]==1)
		begin
			rf_re_r <= 7;
		end	

	end

	10:
	// CPL A, !A
	begin
		A = ~data_bus;


		// set ADD/SUB flag 1
		// reset N flag
//////////////////////////////		

		F[1] = 1; 		


		// set Half carry flag 4
//////////////////////////////		

		F[4]=1;



		rf_ld_r[0] <= 1;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 1;
		rf_re_r <= 0;


	end


	11:
	// NEG 0-A
	begin

		tmp = data_bus;
		A = ~data_bus+1;


		//	     7 6 5 4 3 2  1 0
		// set flags S Z X H X PV N C
		// set Carry flag 0
//////////////////////////////		
		if(tmp != 0)
		begin
			F[0] = 1;
		end
		if(tmp == 0)
		begin
			F[0] = 0;
		end


		// set ADD/SUB flag 1
		// reset N flag
//////////////////////////////		

		F[1] = 1; 		

		// set Overflow/Parity flag 2
		// overflow
//////////////////////////////
		if(tmp == 8'b10000000)
		begin
			F[2]=1;
		end

		else
		begin
			F[2]=0;
		end

		// set Half carry flag 4
//////////////////////////////		

		//F[4]=1;


		// set Zero flag 6
//////////////////////////////				
		if(A==0)
		begin
			F[6] = 1;
		end	
		else		
		begin
			F[6] = 0;
		end
			
		// set Signed flag 7
//////////////////////////////				
		if(A[7]==1)
		begin
			F[7] = 1;
		end	
		else
		begin
			F[7] = 0;
		end

		rf_ld_r[0] <= 1;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 1;
		rf_re_r <= 0;


	end


	12:
	// CCF invert carry
	begin

		F[0] = !F[0];
		F[1] = 0;

		rf_ld_r[0] <= 0;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 1;
		rf_re_r <= 0;


	end

	13:
	// SCF set carry
	begin

		F[0] = 1;
		F[1] = 0;
		F[4] = 0;

		rf_ld_r[0] <= 0;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 1;
		rf_re_r <= 0;


	end


	default:
	begin

		rf_ld_r[0] <= 0;
		rf_ld_r[1] <= 0;
		rf_ld_r[2] <= 0;
		rf_ld_r[3] <= 0;
		rf_ld_r[4] <= 0;
		rf_ld_r[5] <= 0;
		rf_ld_r[6] <= 0;
		rf_ld_r[7] <= 0;
	end
	endcase

end

/*
always@(rf_ld_r, A)
begin
	if(rf_ld_r[0]==1)
	begin
		rf_A_reg = A[7:0]; 
	end
	else if(rf_ld_r[1]==1)
	begin
		rf_A_reg = A[7:0]; 
	end
	else if(rf_ld_r[2]==1)
	begin
		rf_A_reg = A[7:0]; 
	end
	else if(rf_ld_r[3]==1)
	begin
		rf_A_reg = A[7:0]; 
	end
	else if(rf_ld_r[4]==1)
	begin
		rf_A_reg = A[7:0]; 
	end

	else if(rf_ld_r[5]==1)
	begin
		rf_A_reg = A[7:0]; 
	end
	else if(rf_ld_r[6]==1)
	begin
		rf_A_reg = A[7:0]; 
	end
	else if(rf_ld_r[7]==1)
	begin
		rf_A_reg = A[7:0]; 
	end
	
	else	
	begin
		rf_A_reg = 'z;
	end

	
end*/

assign rf_A = rf_ld_r[6:0] ? A : 'z;
//assign rf_A = rf_A_reg;
assign rf_F = rf_ld_r[7] ? F : 'z; 

endmodule
