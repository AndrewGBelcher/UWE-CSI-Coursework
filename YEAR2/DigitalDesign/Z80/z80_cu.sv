module z80_cu(
	input clk,
	input reset,
	input logic [7:0]data_bus,
//	output [15:0]addr_bus,
	output logic [15:0]WE,
	output logic [3:0]RE,
	output logic [3:0]alu_sel,
	output logic rf_read,
	output logic [3:0]db_sel,
	output logic [7:0]cu_out,
	output logic [7:0]r_sel
);



logic [15:0]addr=0;
reg [3:0]reg_sel =0;
reg [3:0]state =0;
reg [7:0]instruction_decoder;
reg [7:0]W;
reg [7:0]Z;
reg halt = 0;

always@(posedge clk,posedge reset)
begin
		
		if(reset==1)
		begin
			halt<=0;
			rf_read <= 0;
			alu_sel <= 15;
			db_sel <= 0;
			RE <= 0;
			WE <= 0;
			r_sel <= 0;
			state = 0;
			W = 0;
		end


		if((state==0) & (reset ==0))
		begin
			W = instruction_decoder;
			
			if(halt == 1)
			begin
				rf_read <= 0;
				alu_sel <= 15;
				db_sel <= 0;
				RE <= 0;
				WE <= 0;
				r_sel <= 0;
				state = 0;


			end
			
			else if(halt == 0)
			begin
				case(data_bus[7:6])
					0:
					begin
						// NOP 0x00
						if(data_bus[7:0] == 0)
						begin
							rf_read <= 0;
							alu_sel <= 15;
							db_sel <= 0;
							RE <= 0;
							WE <= 0;
							r_sel <= 0;
							state = 0;	
						end

						// LD r, n
						if(data_bus[2:0]==3'b110)
						begin
							rf_read <= 0;
							alu_sel <= 15;
							db_sel <= 0;
							RE <= 0;
							WE <= 0;
							r_sel <= 0;
							state = 1;	
						end

						// INC r+1
						if(data_bus[2:0]==3'b100)
						begin
							rf_read <= 1;
							alu_sel <= 8;
							db_sel <= 0;
							WE <= 0;
							state = 0;	
							r_sel <= 0;
								case(data_bus[5:3])
									3'b111:	  
									begin
										RE <= 0;//A
										r_sel[0] <= 1;
									end
									3'b000:	
									begin
										RE <= 2;//B
										r_sel[1] <= 1;
									end
									3'b001:	
									begin
										RE <= 3;//C
										r_sel[2] <= 1;
									
									end
									3'b010:	 
									begin
										RE <= 4;//D
										r_sel[3] <= 1;
		
									end
									3'b011:	 
									begin
										RE <= 5;//E
										r_sel[4] <= 1;
									
									end
									3'b100:	 
									begin
										RE <= 6;//H
										r_sel[5] <= 1;
									
									end
									3'b101:
									begin
										RE <= 7;//L
										r_sel[6] <= 1;
									
									end
								endcase			

						end

						// DEC r-1
						if(data_bus[2:0]==3'b101)
						begin
							rf_read <= 1;
							alu_sel <= 9;
							db_sel <= 0;
							WE <= 0;
							state = 0;	
							r_sel <= 0;
								case(data_bus[5:3])
									3'b111:	  
									begin
										RE <= 0;//A
										r_sel[0] <= 1;
									end
									3'b000:	
									begin
										RE <= 2;//B
										r_sel[1] <= 1;
									end
									3'b001:	
									begin
										RE <= 3;//C
										r_sel[2] <= 1;
									
									end
									3'b010:	 
									begin
										RE <= 4;//D
										r_sel[3] <= 1;
		
									end
									3'b011:	 
									begin
										RE <= 5;//E
										r_sel[4] <= 1;
									
									end
									3'b100:	 
									begin
										RE <= 6;//H
										r_sel[5] <= 1;
									
									end
									3'b101:
									begin
										RE <= 7;//L
										r_sel[6] <= 1;
									
									end
								endcase			

						end

						// CPL A = !A
						if(data_bus[2:0]==3'b111)
						begin
							if(data_bus[5:3]==3'b101)
							begin
								rf_read <= 0;
								alu_sel <= 10;
								db_sel <= 0;
								RE <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 0;	
							end
						end


						// CCF carry flag inverted
						if(data_bus[2:0]==3'b111)
						begin
							if(data_bus[5:3]==3'b111)
							begin
								rf_read <= 0;
								alu_sel <= 12;
								db_sel <= 0;
								RE <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 0;	
							end
						end

						// CCF carry flag inverted
						if(data_bus[2:0]==3'b111)
						begin
							if(data_bus[5:3]==3'b110)
							begin
								rf_read <= 0;
								alu_sel <= 13;
								db_sel <= 0;
								RE <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 0;	
							end
						end
							
							//load = 0;
							//addr = 1;
					end

					1:
					begin
							//load = 1;

						if(data_bus[5:3]==3'b110)
						begin
							if(data_bus[2:0]==3'b110)
							begin
								rf_read <= 0;
								alu_sel <= 15;
								db_sel <= 0;
								RE <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 0;
								halt <= 1;	
							end
						end

						if(data_bus[2:0]!=3'b110)
						begin
							if(data_bus[5:3]!=3'b110)
							begin
								//setup for register/register ld
								rf_read <= 1;
								alu_sel <= 15;
								db_sel <= 0;
								r_sel <= 0;
								//RE <= 0;
								case(data_bus[5:3])
									3'b111:	  
									begin
										WE[2] <= 1;//A
									end
									3'b000:	
									begin
										WE[4] <= 1;//B
									end
									3'b001:	
									begin
										WE[5] <= 1;//C
									end
									3'b010:	 
									begin
										WE[6] <= 1;//D
									end
									3'b011:	 
									begin
										WE[7] <= 1;//E
									end
									3'b100:	 
									begin
										WE[8] <= 1;//H
									end
									3'b101:
									begin
										WE[9] <= 1;//L
									end
								endcase	
								
								case(data_bus[2:0])
									3'b111:	  
									begin
										RE <= 0;//A
									end
									3'b000:	
									begin
										RE <= 2;//B
									end
									3'b001:	
									begin
										RE <= 3;//C
									end
									3'b010:	 
									begin
										RE <= 4;//D
									end
									3'b011:	 
									begin
										RE <= 5;//E
									end
									3'b100:	 
									begin
										RE <= 6;//H
									end
									3'b101:
									begin
										RE <= 7;//L
									end
								endcase			
							end // end of 2nd if databus 2:0
							state = 0;
						end // end of first if databus 5:3
					end // end of 1case for top 2 bit case

					2:
					begin
						case(data_bus[5:3])
							// ADD A,r
							0:
							begin
								rf_read <= 1;
								alu_sel <= 0;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 0;
								case(data_bus[2:0])
										3'b111:	  
										begin
											RE <= 0;//A
										end
										3'b000:	
										begin
											RE <= 2;//B
										end
										3'b001:	
										begin
											RE <= 3;//C
										end
										3'b010:	 
										begin
											RE <= 4;//D
										end
										3'b011:	 
										begin
											RE <= 5;//E
										end
										3'b100:	 
										begin
											RE <= 6;//H
										end
										3'b101:
										begin
											RE <= 7;//L
										end
										
			
								endcase	
							end // end of add a,r 

							// ADC A,r
							1:
							begin
								rf_read <= 1;
								alu_sel <= 3;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 0;
								case(data_bus[2:0])
										3'b111:	  
										begin
											RE <= 0;//A
										end
										3'b000:	
										begin
											RE <= 2;//B
										end
										3'b001:	
										begin
											RE <= 3;//C
										end
										3'b010:	 
										begin
											RE <= 4;//D
										end
										3'b011:	 
										begin
											RE <= 5;//E
										end
										3'b100:	 
										begin
											RE <= 6;//H
										end
										3'b101:
										begin
											RE <= 7;//L
										end
										
			
								endcase	
							end // end of adc A,r						


							// SUB A,r
							2:
							begin
								rf_read <= 1;
								alu_sel <= 1;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 0;
								case(data_bus[2:0])
										3'b111:	  
										begin
											RE <= 0;//A
										end
										3'b000:	
										begin
											RE <= 2;//B
										end
										3'b001:	
										begin
											RE <= 3;//C
										end
										3'b010:	 
										begin
											RE <= 4;//D
										end
										3'b011:	 
										begin
											RE <= 5;//E
										end
										3'b100:	 
										begin
											RE <= 6;//H
										end
										3'b101:
										begin
											RE <= 7;//L
										end
								endcase	
							end// end of sub A,r	

							// SBC A,r
							3:
							begin
								rf_read <= 1;
								alu_sel <= 2;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 0;
								case(data_bus[2:0])
										3'b111:	  
										begin
											RE <= 0;//A
										end
										3'b000:	
										begin
											RE <= 2;//B
										end
										3'b001:	
										begin
											RE <= 3;//C
										end
										3'b010:	 
										begin
											RE <= 4;//D
										end
										3'b011:	 
										begin
											RE <= 5;//E
										end
										3'b100:	 
										begin
											RE <= 6;//H
										end
										3'b101:
										begin
											RE <= 7;//L
										end
								endcase	
							end// end of SUB A,r	


							// AND A,r
							4:
							begin
								rf_read <= 1;
								alu_sel <= 4;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 0;
								case(data_bus[2:0])
										3'b111:	  
										begin
											RE <= 0;//A
										end
										3'b000:	
										begin
											RE <= 2;//B
										end
										3'b001:	
										begin
											RE <= 3;//C
										end
										3'b010:	 
										begin
											RE <= 4;//D
										end
										3'b011:	 
										begin
											RE <= 5;//E
										end
										3'b100:	 
										begin
											RE <= 6;//H
										end
										3'b101:
										begin
											RE <= 7;//L
										end
								endcase	
							end// end of AND A,r


							// XOR A,r
							5:
							begin
								rf_read <= 1;
								alu_sel <= 6;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 0;
								case(data_bus[2:0])
										3'b111:	  
										begin
											RE <= 0;//A
										end
										3'b000:	
										begin
											RE <= 2;//B
										end
										3'b001:	
										begin
											RE <= 3;//C
										end
										3'b010:	 
										begin
											RE <= 4;//D
										end
										3'b011:	 
										begin
											RE <= 5;//E
										end
										3'b100:	 
										begin
											RE <= 6;//H
										end
										3'b101:
										begin
											RE <= 7;//L
										end
								endcase	
							end// end of XOR A,r


							// OR A,r
							6:
							begin
								rf_read <= 1;
								alu_sel <= 5;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 0;
								case(data_bus[2:0])
										3'b111:	  
										begin
											RE <= 0;//A
										end
										3'b000:	
										begin
											RE <= 2;//B
										end
										3'b001:	
										begin
											RE <= 3;//C
										end
										3'b010:	 
										begin
											RE <= 4;//D
										end
										3'b011:	 
										begin
											RE <= 5;//E
										end
										3'b100:	 
										begin
											RE <= 6;//H
										end
										3'b101:
										begin
											RE <= 7;//L
										end
								endcase	
							end// end of OR A,r

							// CP A,r
							7:
							begin
								rf_read <= 1;
								alu_sel <= 7;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 0;
								case(data_bus[2:0])
										3'b111:	  
										begin
											RE <= 0;//A
										end
										3'b000:	
										begin
											RE <= 2;//B
										end
										3'b001:	
										begin
											RE <= 3;//C
										end
										3'b010:	 
										begin
											RE <= 4;//D
										end
										3'b011:	 
										begin
											RE <= 5;//E
										end
										3'b100:	 
										begin
											RE <= 6;//H
										end
										3'b101:
										begin
											RE <= 7;//L
										end
								endcase	
							end// end of CMP A,r



						endcase // end of 5:3 for  2 of 7:6
					end	// end of case 2 for 7:6

					3:
					begin
						// ADD A, n
						if(data_bus[5:3] == 0)
						begin
							if(data_bus[2:0] == 3'b110)
							begin
								rf_read <= 0;
								alu_sel <= 15;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 1;
								
							end
						end
						// ADC A, n
						if(data_bus[5:3] == 3'b001)
						begin
							if(data_bus[2:0] == 3'b110)
							begin
								rf_read <= 0;
								alu_sel <= 15;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 1;
								
							end
						end
						// SUB A, n
						if(data_bus[5:3] == 3'b010)
						begin
							if(data_bus[2:0] == 3'b110)
							begin
								rf_read <= 0;
								alu_sel <= 15;
								db_sel <= 0;
								r_sel <= 0;
								WE <= 0;
								state = 1;
								
							end
						end		
						// SBC A, r
						if(data_bus[5:3] == 3'b011)
						begin
							if(data_bus[2:0] == 3'b110)
							begin
								rf_read <= 0;
								alu_sel <= 15;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 1;
								
							end
						end	

						// AND A, r
						if(data_bus[5:3] == 3'b100)
						begin
							if(data_bus[2:0] == 3'b110)
							begin
								rf_read <= 0;
								alu_sel <= 15;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 1;
								
							end
						end	

						
						if(data_bus[5:3] == 3'b101)
						begin
							// XOR A, r
							if(data_bus[2:0] == 3'b110)
							begin
								rf_read <= 0;
								alu_sel <= 15;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 1;
								
							end
							// NEG A
							if(data_bus[2:0] == 3'b101)
							begin
								rf_read <= 0;
								alu_sel <= 15;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 1;
								
							end
						end	

						// OR A, r
						if(data_bus[5:3] == 3'b110)
						begin
							if(data_bus[2:0] == 3'b110)
							begin
								rf_read <= 0;
								alu_sel <= 15;
								db_sel <= 0;
								r_sel <= 0;
								WE <= 0;
								state = 1;
								
							end
						end	

						// CP A, r
						if(data_bus[5:3] == 3'b111)
						begin
							if(data_bus[2:0] == 3'b110)
							begin
								rf_read <= 0;
								alu_sel <= 15;
								db_sel <= 0;
								WE <= 0;
								r_sel <= 0;
								state = 1;
								
							end
						end	

					end //end of 3 of top 2 bit case
				endcase // end of top 2 bit case
			end // end of halt == 0	
		end // end of if state == 0

		else if((state==1) & (reset ==0))
		begin

			// ADD A, n
			if(W[7:6]== 3)
			begin
				if(W[5:3] == 0)
				begin
					if(W[2:0] == 3'b110)
					begin
						rf_read <= 0;
						alu_sel <= 0;
						db_sel <= 1; // CU OUT
						WE <= 0;
						r_sel <= 0;
						RE <= 0;
						cu_out <= data_bus;
						state = 0;
					end
				end
			end

			// ADC A, n
			if(W[7:6]== 3)
			begin
				if(W[5:3] == 3'b001)
				begin
					if(W[2:0] == 3'b110)
					begin
						rf_read <= 0;
						alu_sel <= 3;
						db_sel <= 1; // CU OUT
						WE <= 0;
						RE <= 0;
						r_sel <= 0;
						cu_out <= data_bus;
						state = 0;
					end
				end
			end

			// SUB A, n
			if(W[7:6]== 3)
			begin
				if(W[5:3] == 3'b010)
				begin
					if(W[2:0] == 3'b110)
					begin
						rf_read <= 0;
						alu_sel <= 1;
						db_sel <= 1; // CU OUT
						WE <= 0;
						RE <= 0;
						r_sel <= 0;
						cu_out <= data_bus;
						state = 0;
					end
				end
			end

			// SBC A, n
			if(W[7:6]== 3)
			begin
				if(W[5:3] == 3'b011)
				begin
					if(W[2:0] == 3'b110)
					begin
						rf_read <= 0;
						alu_sel <= 2;
						db_sel <= 1; // CU OUT
						WE <= 0;
						RE <= 0;
						r_sel <= 0;
						cu_out <= data_bus;
						state = 0;
					end
				end
			end



			// AND A, n
			if(W[7:6]== 3)
			begin
				if(W[5:3] == 3'b100)
				begin
					if(W[2:0] == 3'b110)
					begin
						rf_read <= 0;
						alu_sel <= 4;
						db_sel <= 1; // CU OUT
						WE <= 0;
						RE <= 0;
						r_sel <= 0;
						cu_out <= data_bus;
						state = 0;
					end
				end
			end


			if(W[7:6]== 3)
			begin
				// XOR A, n
				if(W[5:3] == 3'b101)
				begin
					if(W[2:0] == 3'b110)
					begin
						rf_read <= 0;
						alu_sel <= 6;
						db_sel <= 1; // CU OUT
						WE <= 0;
						RE <= 0;
						r_sel <= 0;
						cu_out <= data_bus;
						state = 0;
					end
				end
				// NEG A
				if(W[5:3]== 3'b101)
				begin
					if(W[2:0]== 3'b101)
					begin
						if(data_bus[7:0] == 8'b01000100)
						begin
							rf_read <= 1;
							alu_sel <= 11;
							db_sel <= 0; // rf out
							WE <= 0;
							RE <= 0;
							r_sel <= 0;
							//cu_out <= data_bus;
							state = 0;
						end
					end
				end
			end

			// OR A, n
			if(W[7:6]== 3)
			begin
				if(W[5:3] == 3'b110)
				begin
					if(W[2:0] == 3'b110)
					begin
						rf_read <= 0;
						alu_sel <= 5;
						db_sel <= 1; // CU OUT
						WE <= 0;
						RE <= 0;
						r_sel <= 0;
						cu_out <= data_bus;
						state = 0;
					end
				end
			end


			// CMP A, n
			if(W[7:6]== 3)
			begin
				if(W[5:3] == 3'b111)
				begin
					if(W[2:0] == 3'b110)
					begin
						rf_read <= 0;
						alu_sel <= 7;
						db_sel <= 1; // CU OUT
						WE <= 0;
						RE <= 0;
						r_sel <= 0;
						cu_out <= data_bus;
						state = 0;
					end
				end
			end

			
			if(W[7:6]== 0)
			begin
				// LD r, n
				if(W[2:0]==3'b110)
				begin
					rf_read <= 0;
					alu_sel <= 15;
					db_sel <= 1; // CU OUT
					RE <= 0;
					r_sel <= 0;
					cu_out <= data_bus;

					case(W[5:3])
						3'b111:	  
						begin
							WE[2] <= 1;//A
						end
						3'b000:	
						begin
							WE[4] <= 1;//B
						end
						3'b001:	
						begin
							WE[5] <= 1;//C
						end
						3'b010:	 
						begin
							WE[6] <= 1;//D
						end
						3'b011:	 
						begin
							WE[7] <= 1;//E
						end
						3'b100:	 
						begin
							WE[8] <= 1;//H
						end
						3'b101:
						begin
							WE[9] <= 1;//L
						end
					endcase		
				end // end of if W l r, n
				state = 0;
			end // end of if W[7:6]==0
		end // end if state==1
		
end // end always

assign instruction_decoder = data_bus;
//assign addr_bus = addr;
endmodule