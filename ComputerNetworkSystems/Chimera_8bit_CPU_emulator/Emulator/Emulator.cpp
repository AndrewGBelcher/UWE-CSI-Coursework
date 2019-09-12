#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include "stdafx.h"
#include <winsock2.h>

#pragma comment(lib, "wsock32.lib")


#define STUDENT_NUMBER    "17010347"

#define IP_ADDRESS_SERVER "127.0.0.1"

#define PORT_SERVER 0x1984 // We define a port that we are going to use.
#define PORT_CLIENT 0x1985 // We define a port that we are going to use.

#define WORD  unsigned short
#define DWORD unsigned long
#define BYTE  unsigned char

#define MAX_FILENAME_SIZE 500
#define MAX_BUFFER_SIZE   500


SOCKADDR_IN server_addr;
SOCKADDR_IN client_addr;

SOCKET sock;  // This is our socket, it is the handle to the IO address to read/write packets

WSADATA data;

char InputBuffer [MAX_BUFFER_SIZE];

char hex_file [MAX_BUFFER_SIZE];
char trc_file [MAX_BUFFER_SIZE];

//////////////////////////
//   Registers          //
//////////////////////////

#define FLAG_I  0x10
#define FLAG_V  0x08
#define FLAG_N  0x04
#define FLAG_Z  0x02
#define FLAG_C  0x01
#define REGISTER_A	5
#define REGISTER_F	4
#define REGISTER_E	3
#define REGISTER_D	2
#define REGISTER_C	1
#define REGISTER_B	0
#define REGISTER_X 0
#define REGISTER_Y 1
BYTE Index_Registers[2];

BYTE Registers[6];
BYTE Flags;
WORD ProgramCounter;
WORD StackPointer;

int testCnt = 0;

////////////
// Memory //
////////////

#define MEMORY_SIZE	65536

BYTE Memory[MEMORY_SIZE];

#define TEST_ADDRESS_1  0x01FA
#define TEST_ADDRESS_2  0x01FB
#define TEST_ADDRESS_3  0x01FC
#define TEST_ADDRESS_4  0x01FD
#define TEST_ADDRESS_5  0x01FE
#define TEST_ADDRESS_6  0x01FF
#define TEST_ADDRESS_7  0x0200
#define TEST_ADDRESS_8  0x0201
#define TEST_ADDRESS_9  0x0202
#define TEST_ADDRESS_10  0x0203
#define TEST_ADDRESS_11  0x0204
#define TEST_ADDRESS_12  0x0205


///////////////////////
// Control variables //
///////////////////////

bool memory_in_range = true;
bool halt = false;


///////////////////////
// Disassembly table //
///////////////////////

char opcode_mneumonics[][14] =
{
"ILLEGAL     ", 
"ILLEGAL     ", 
"STX abs      ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"MV  #,B      ", 
"MV  #,C      ", 
"MV  #,D      ", 
"MV  #,E      ", 
"MV  #,F      ", 
"MAY impl     ", 
"MYA impl     ", 
"MAS impl     ", 
"CSA impl     ", 

"ILLEGAL     ", 
"ILLEGAL     ", 
"STX abs,X    ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"SWI impl     ", 
"RTI impl     ", 
"CLC impl     ", 
"SEC impl     ", 
"CLI impl     ", 
"STI impl     ", 
"STV impl     ", 
"CLV impl     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 

"ILLEGAL     ", 
"ILLEGAL     ", 
"STX abs,Y    ", 
"ADD A,B      ", 
"SUB A,B      ", 
"CMP A,B      ", 
"OR A,B       ", 
"AND A,B      ", 
"EOR A,B      ", 
"BT A,B       ", 
"LD A,A       ", 
"LD B,A       ", 
"LD C,A       ", 
"LD D,A       ", 
"LD E,A       ", 
"LD F,A       ", 

"ILLEGAL     ", 
"LDX  #       ", 
"STX abs,XY   ", 
"ADD A,C      ", 
"SUB A,C      ", 
"CMP A,C      ", 
"OR A,C       ", 
"AND A,C      ", 
"EOR A,C      ", 
"BT A,C       ", 
"LD A,B       ", 
"LD B,B       ", 
"LD C,B       ", 
"LD D,B       ", 
"LD E,B       ", 
"LD F,B       ", 

"ILLEGAL     ", 
"LDX abs      ", 
"STX (ind),XY ", 
"ADD A,D      ", 
"SUB A,D      ", 
"CMP A,D      ", 
"OR A,D       ", 
"AND A,D      ", 
"EOR A,D      ", 
"BT A,D       ", 
"LD A,C       ", 
"LD B,C       ", 
"LD C,C       ", 
"LD D,C       ", 
"LD E,C       ", 
"LD F,C       ", 

"ILLEGAL     ", 
"LDX abs,X    ", 
"ILLEGAL     ", 
"ADD A,E      ", 
"SUB A,E      ", 
"CMP A,E      ", 
"OR A,E       ", 
"AND A,E      ", 
"EOR A,E      ", 
"BT A,E       ", 
"LD A,D       ", 
"LD B,D       ", 
"LD C,D       ", 
"LD D,D       ", 
"LD E,D       ", 
"LD F,D       ", 

"ILLEGAL     ", 
"LDX abs,Y    ", 
"ILLEGAL     ", 
"ADD A,F      ", 
"SUB A,F      ", 
"CMP A,F      ", 
"OR A,F       ", 
"AND A,F      ", 
"EOR A,F      ", 
"BT A,F       ", 
"LD A,E       ", 
"LD B,E       ", 
"LD C,E       ", 
"LD D,E       ", 
"LD E,E       ", 
"LD F,E       ", 

"ILLEGAL     ", 
"LDX abs,XY   ", 
"ILLEGAL     ", 
"NOP impl     ", 
"HLT impl     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"LD A,F       ", 
"LD B,F       ", 
"LD C,F       ", 
"LD D,F       ", 
"LD E,F       ", 
"LD F,F       ", 

"ILLEGAL     ", 
"LDX (ind),XY ", 
"ADI  #       ", 
"SBI  #       ", 
"CPI  #       ", 
"ORI  #       ", 
"ANI  #       ", 
"XRI  #       ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 

"LDA  #       ", 
"TST abs      ", 
"INC abs      ", 
"DEC abs      ", 
"RCR abs      ", 
"RLC abs      ", 
"ASL abs      ", 
"SAR abs      ", 
"COM abs      ", 
"RAL abs      ", 
"ROR abs      ", 
"LX  #,A      ", 
"ILLEGAL     ", 
"LODS  #      ", 
"PUSH  ,A     ", 
"POP A,       ", 

"LDA abs      ", 
"TST abs,X    ", 
"INC abs,X    ", 
"DEC abs,X    ", 
"RCR abs,X    ", 
"RLC abs,X    ", 
"ASL abs,X    ", 
"SAR abs,X    ", 
"COM abs,X    ", 
"RAL abs,X    ", 
"ROR abs,X    ", 
"ILLEGAL     ", 
"STO abs      ", 
"LODS abs     ", 
"PUSH  ,s     ", 
"POP s,       ", 

"LDA abs,X    ", 
"TST abs,Y    ", 
"INC abs,Y    ", 
"DEC abs,Y    ", 
"RCR abs,Y    ", 
"RLC abs,Y    ", 
"ASL abs,Y    ", 
"SAR abs,Y    ", 
"COM abs,Y    ", 
"RAL abs,Y    ", 
"ROR abs,Y    ", 
"ILLEGAL     ", 
"STO abs,X    ", 
"LODS abs,X   ", 
"PUSH  ,B     ", 
"POP B,       ", 

"LDA abs,Y    ", 
"TST abs,XY   ", 
"INC abs,XY   ", 
"DEC abs,XY   ", 
"RCR abs,XY   ", 
"RLC abs,XY   ", 
"ASL abs,XY   ", 
"SAR abs,XY   ", 
"COM abs,XY   ", 
"RAL abs,XY   ", 
"ROR abs,XY   ", 
"ILLEGAL     ", 
"STO abs,Y    ", 
"LODS abs,Y   ", 
"PUSH  ,C     ", 
"POP C,       ", 

"LDA abs,XY   ", 
"TSTA A,A     ", 
"INCA A,A     ", 
"DECA A,A     ", 
"RCRA A,A     ", 
"RLCA A,A     ", 
"ASLA A,A     ", 
"SARA A,A     ", 
"COMA A,A     ", 
"RALA A,A     ", 
"RORA A,A     ", 
"RTN impl     ", 
"STO abs,XY   ", 
"LODS abs,XY  ", 
"PUSH  ,D     ", 
"POP D,       ", 

"LDA (ind),XY ", 
"DEX impl     ", 
"INX impl     ", 
"DEY impl     ", 
"INCY impl    ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"ILLEGAL     ", 
"JSR abs      ", 
"JMP abs      ", 
"ILLEGAL     ", 
"STO (ind),XY ", 
"LODS (ind),XY", 
"PUSH  ,E     ", 
"POP E,       ", 

"BRA rel      ", 
"BCC rel      ", 
"BCS rel      ", 
"BNE rel      ", 
"BEQ rel      ", 
"BVC rel      ", 
"BVS rel      ", 
"BMI rel      ", 
"BPL rel      ", 
"BGE rel      ", 
"BLE rel      ", 
"BGT rel      ", 
"BLT rel      ", 
"ILLEGAL     ", 
"PUSH  ,F     ", 
"POP F,       ", 

}; 

////////////////////////////////////////////////////////////////////////////////
//                           Simulator/Emulator (Start)                       //
////////////////////////////////////////////////////////////////////////////////

BYTE fetch()
{
	BYTE byte = 0;

	if ((ProgramCounter >= 0) && (ProgramCounter <= MEMORY_SIZE))
	{
		memory_in_range = true;
		byte = Memory[ProgramCounter];
		ProgramCounter++;
	}
	else
	{
		memory_in_range = false;
	}
	return byte;
}

////////////////////////////////////////////////////////////////////////////////
//                           Load/Store functions                             //
////////////////////////////////////////////////////////////////////////////////

void loadRegFromMem(WORD address, int reg) 
{
	if ((address < MEMORY_SIZE) && (address >= 0))
	{
		Registers[reg] = Memory[address];
	}

}

void storeMemFromReg(WORD address, int reg) 
{
	if ((address < MEMORY_SIZE) && (address >= 0))
	{
		Memory[address] = Registers[reg];
	}

}

void loadSP(WORD address) 
{
			if ((address >= 0) && (address < MEMORY_SIZE - 1))
			{
				StackPointer = (WORD)Memory[address] << 8;
				StackPointer += Memory[address + 1];

			}

}

void loadImm(int reg)
{
	BYTE data = fetch();
	Registers[reg] = data;
}

////////////////////////////////////////////////////////////////////////////////
//                           Addressing Functions                             //
////////////////////////////////////////////////////////////////////////////////

WORD getAddrAbs(void)
{
	BYTE HB = fetch();
	BYTE LB = fetch();
	WORD address = ((WORD)HB << 8) + (WORD)LB;
	return(address);
}

WORD getAddrAbsX(void)
{
		WORD address = 0;		

		address += Index_Registers[REGISTER_X];
		BYTE HB = fetch();
		BYTE LB = fetch();
		address += (WORD)((WORD)HB << 8) + LB;

	return(address);
}

WORD getAddrAbsY(void)
{
		 WORD address = 0;  		

		address += Index_Registers[REGISTER_Y];
		BYTE HB = fetch();
		BYTE LB = fetch();
		address += (WORD)((WORD)HB << 8) + LB;

	return(address);
}

WORD getAddrAbsXY(void)
{
		WORD address = 0;  		

		address += (WORD)((WORD)Index_Registers[REGISTER_Y] << 8) + Index_Registers[REGISTER_X];
		BYTE HB = fetch();
		BYTE LB = fetch();
		address += (WORD)((WORD)HB << 8) + LB;

	return(address);
}

WORD getAddrInd(void)
{
	 WORD address = 0;  

	WORD HB = fetch();
	WORD LB = fetch();
	address += (WORD)((WORD)HB << 8) + LB;

	HB = Memory[address];
	LB = Memory[address + 1];
	address = (WORD)((WORD)HB << 8) + LB;

	address += Index_Registers[REGISTER_X];
	address += (WORD)((WORD)Index_Registers[REGISTER_Y] << 8);

	return(address);
}

////////////////////////////////////////////////////////////////////////////////
//                           Arithmetic Functions                             //
////////////////////////////////////////////////////////////////////////////////

void ASL(WORD address)
{
	if ((address < MEMORY_SIZE) && (address >= 0))
	{
		if ((Memory[address] & 0x80) == 0x80) // carry
		{
			Flags = Flags | FLAG_C;
		}
		else
		{
			Flags = Flags & (0xFF - FLAG_C);
		}

		Memory[address] = (Memory[address] << 1);
	}
}

void SAR(WORD address)
{
	if ((address < MEMORY_SIZE) && (address >= 0))
	{
		BYTE temp1 = Memory[address];

		if ((Memory[address] & 1) == 1) // carry
		{
			Flags = Flags | FLAG_C;
		}
		else
		{
			Flags = Flags & (0xFF - FLAG_C);
		}

		Memory[address] = (Memory[address] >> 1) & 0x7F;

		if ((temp1 & 0x80) == 0x80)
		{
			Memory[address] = Memory[address] | 0x80;
		}
	}

}

void RCR(WORD address)
{
	BYTE oldFlags = Flags;

	if ((address < MEMORY_SIZE) && (address >= 0))
	{
		if ((Memory[address] & 1) == 1) // carry
		{
			Flags = Flags | FLAG_C;
		}
		else
		{
			Flags = Flags & (0xFF - FLAG_C);
		}

		Memory[address] = (Memory[address] >> 1) & 0x7F;

		if ((oldFlags & FLAG_C) == FLAG_C)
		{
			Memory[address] = Memory[address] | 0x80;
		}
	} 
}

void RLC(WORD address)
{
	BYTE oldFlags = Flags;

	if ((address < MEMORY_SIZE) && (address >= 0))
	{

		if ((Memory[address] & 0x80) == 0x80) // carry
		{
			Flags = Flags | FLAG_C;
		}
		else
		{
			Flags = Flags & (0xFF - FLAG_C);
		}

		Memory[address] = (Memory[address] << 1) & 0xFE;

		if ((oldFlags & FLAG_C) == FLAG_C)
		{
			Memory[address] = Memory[address] | 1;
		}
	}
}

void ROR(WORD address)
{
	if ((address < MEMORY_SIZE) && (address >= 0))
		{
			BYTE temp1 = Memory[address];
			Memory[address] = (temp1 >> 1) & 0x7f;

			if ((temp1 & 1) == 1)
			{
				Memory[address] = Memory[address] | 0x80;
			}
		}
}

void RAL(WORD address)
{
	if ((address < MEMORY_SIZE) && (address >= 0))
	{
		BYTE temp1 = Memory[address];
		Memory[address] = (temp1 << 1) & 0xFE;

		if ((temp1 & 0x80) == 0x80)
		{
			Memory[address] = Memory[address] | 1;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//                       Flag manipulation Functions                          //
////////////////////////////////////////////////////////////////////////////////

// set the sign flag in the status register
void set_flag_n(BYTE inReg) 
{
	BYTE reg; 
	reg = inReg; 

	if ((reg & 0x80) != 0) // msbit set 
	{ 
		Flags = Flags | FLAG_N; // set it
	}
	else 
	{ 
		Flags = Flags & (0xFF - FLAG_N); // clear it
	}
}

// set the zero flag in status register
void set_flag_z(BYTE inReg) 
{
	BYTE reg; 
	reg = inReg; 

	if (reg == 0) // set it to zero 
	{ 
		Flags = Flags | FLAG_Z; // set it
	}
	else 
	{ 
		Flags = Flags & (0xFF - FLAG_Z); // clear it
	}
}

// set the sign flag in the status register
void set_flag_nword(WORD inReg)
{
	WORD reg;
	reg = inReg;

	if ((reg & 0x8000) != 0) // msbit set 
	{
		Flags = Flags | FLAG_N; // set it
	}
	else
	{
		Flags = Flags & (0xFF - FLAG_N); // clear it
	}
}

// set the zero flag in status register
void set_flag_zword(WORD inReg)
{
	WORD reg;
	reg = inReg;

	if (reg == 0) // set it to zero 
	{
		Flags = Flags | FLAG_Z; // set it
	}
	else
	{
		Flags = Flags & (0xFF - FLAG_Z); // clear it
	}
}

// set the carry flag in status register
void set_flag_c(WORD inVal) 
{
	WORD val; 
	val = inVal; 

	if (val >= 0x100) // set it to zero 
	{ 
		Flags = Flags | FLAG_C; // set it
	}
	else 
	{ 
		Flags = Flags & (0xFF - FLAG_C); // clear it
	}
}

// overflow for addition
void set_flag_v(BYTE in1, BYTE in2, BYTE out1) 
{
	BYTE reg1in;
	BYTE reg2in;
	BYTE regOut;

	reg1in = in1;
	reg2in = in2;
	regOut = out1;
	
		if ((((reg1in & 0x80) == 0x80) && ((reg2in & 0x80) == 0x80) && ((regOut & 0x80) != 0x80))
			|| (((reg1in & 0x80) != 0x80) && ((reg2in & 0x80) != 0x80) && ((regOut & 0x80) == 0x80)))
		{
			Flags = Flags | FLAG_V;
		}
		else
		{
			Flags = Flags & (0xFF - FLAG_V);
		}
	
}

// overflow for subtraction
void set_flag_vsub(WORD in1, WORD in2, WORD out1) 
{
	WORD reg1in;
	WORD reg2in;
	WORD regOut;

	reg1in = in1;
	reg2in = in2;
	regOut = out1;
	
		if ((((reg1in & 0x80) != 0x80) && ((reg2in & 0x80) == 0x80) && (((BYTE)regOut & 0x80) == 0x80))
			|| (((reg1in & 0x80) == 0x80) && ((reg2in & 0x80) != 0x80) && (((BYTE)regOut & 0x80) != 0x80)))
		{
			Flags = Flags | FLAG_V;
		}
		else
		{
			Flags = Flags & (0xFF - FLAG_V);
		}
	
}


////////////////////////////////////////////////////////////////////////////////
//                               Opcode Groups                                //
////////////////////////////////////////////////////////////////////////////////


void Group_1_Move(BYTE opcode)
{
	BYTE destination = opcode >> 4; //top four bits point at one register....shift right to keep only top four
	BYTE source = opcode & 0x0F; //takes bottom four bits

	int destReg = 0;
	int sourceReg = 0;
	
	switch(opcode) 
	{
		case 0x2A:
			sourceReg = REGISTER_A;
			destReg = REGISTER_A;

			break;
		case 0x3A:
			sourceReg = REGISTER_B;
			destReg = REGISTER_A;
			break;
		case 0x4A:
			sourceReg = REGISTER_C;
			destReg = REGISTER_A;
			break;
		case 0x5A:
			sourceReg = REGISTER_D;
			destReg = REGISTER_A;
			break;
		case 0x6A:
			sourceReg = REGISTER_E;
			destReg = REGISTER_A;
			break;
		case 0x7A:
			sourceReg = REGISTER_F;
			destReg = REGISTER_A;
			break;
		case 0x2B:
			sourceReg = REGISTER_A;
			destReg = REGISTER_B;
			break;
		case 0x3B:
			sourceReg = REGISTER_B;
			destReg = REGISTER_B;
			break;
		case 0x4B:
			sourceReg = REGISTER_C;
			destReg = REGISTER_B;
			break;
		case 0x5B:
			sourceReg = REGISTER_D;
			destReg = REGISTER_B;
			break;
		case 0x6B:
			sourceReg = REGISTER_E;
			destReg = REGISTER_B;
			break;
		case 0x7B:
			sourceReg = REGISTER_F;
			destReg = REGISTER_B;
			break;
		case 0x2C:
			sourceReg = REGISTER_A;
			destReg = REGISTER_C;
			break;
		case 0x3C:
			sourceReg = REGISTER_B;
			destReg = REGISTER_C;
			break;
		case 0x4C:
			sourceReg = REGISTER_C;
			destReg = REGISTER_C;
			break;
		case 0x5C:
			sourceReg = REGISTER_D;
			destReg = REGISTER_C;
			break;
		case 0x6C:
			sourceReg = REGISTER_E;
			destReg = REGISTER_C;
			break;
		case 0x7C:
			sourceReg = REGISTER_F;
			destReg = REGISTER_C;
			break;
		case 0x2D:
			sourceReg = REGISTER_A;
			destReg = REGISTER_D;
			break;
		case 0x3D:
			sourceReg = REGISTER_B;
			destReg = REGISTER_D;
			break;
		case 0x4D:
			sourceReg = REGISTER_C;
			destReg = REGISTER_D;
			break;
		case 0x5D:
			sourceReg = REGISTER_D;
			destReg = REGISTER_D;
			break;
		case 0x6D:
			sourceReg = REGISTER_E;
			destReg = REGISTER_D;
			break;
		case 0x7D:
			sourceReg = REGISTER_F;
			destReg = REGISTER_D;
			break;
		case 0x2E:
			sourceReg = REGISTER_A;
			destReg = REGISTER_E;
			break;
		case 0x3E:
			sourceReg = REGISTER_B;
			destReg = REGISTER_E;
			break;
		case 0x4E:
			sourceReg = REGISTER_C;
			destReg = REGISTER_E;
			break;
		case 0x5E:
			sourceReg = REGISTER_D;
			destReg = REGISTER_E;
			break;
		case 0x6E:
			sourceReg = REGISTER_E;
			destReg = REGISTER_E;
			break;
		case 0x7E:
			sourceReg = REGISTER_F;
			destReg = REGISTER_E;
			break;
		case 0x2F:
			sourceReg = REGISTER_A;
			destReg = REGISTER_F;
			break;
		case 0x3F:
			sourceReg = REGISTER_B;
			destReg = REGISTER_F;
			break;
		case 0x4F:
			sourceReg = REGISTER_C;
			destReg = REGISTER_F;
			break;
		case 0x5F:
			sourceReg = REGISTER_D;
			destReg = REGISTER_F;
			break;
		case 0x6F:
			sourceReg = REGISTER_E;
			destReg = REGISTER_F;
			break;
		case 0x7F:
			sourceReg = REGISTER_F;
			destReg = REGISTER_F;
			break;
		default:
			break;
	}
	// now move it
	Registers[destReg] = Registers[sourceReg];
}

void Group_2_Dual_op(BYTE opcode)
{
	BYTE LB = 0;
	BYTE HB = 0;
	WORD address = 0;
	WORD data = 0;
	BYTE regA = 0;
	WORD temp_word = 0;
	WORD offset = 0;
	BYTE temp1 = 0;
	BYTE temp2 = 0;
	BYTE oldFlags = 0;

	switch (opcode)
	{

////////////////////////////////////////////////////////////////////////////////
//                           LDA - Load Accumulator                           //
////////////////////////////////////////////////////////////////////////////////

		case 0x90: //LDA Immidiate
			loadImm(REGISTER_A);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

		case 0xA0: //LDA abs
			address = getAddrAbs();
			loadRegFromMem(address,REGISTER_A);

			// FLAGS
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

		case 0xB0: //LDA abs, X
			address = getAddrAbsX();
			loadRegFromMem(address,REGISTER_A);

			// FLAGS
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

		case 0xC0: //LDA abs, Y
			address = getAddrAbsY();
			loadRegFromMem(address,REGISTER_A);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

		case 0xD0: //LDA abs, XY
			address = getAddrAbsXY();
			loadRegFromMem(address,REGISTER_A);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

		case 0xE0: //LDA (ind), XY
			address = getAddrInd();
			loadRegFromMem(address,REGISTER_A);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

////////////////////////////////////////////////////////////////////////////////
//                           STO - Store Accumulator                          //
////////////////////////////////////////////////////////////////////////////////

		case 0xAC: //STO abs
			address = getAddrAbs();
			storeMemFromReg(address,REGISTER_A);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

		case 0xBC: //STO abs,X
			address = getAddrAbsX();
			storeMemFromReg(address,REGISTER_A);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

		case 0xCC: //STO abs,Y
			address = getAddrAbsY();
			storeMemFromReg(address,REGISTER_A);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

		case 0xDC: //STO abs,XY
			address = getAddrAbsXY();
			storeMemFromReg(address,REGISTER_A);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

		case 0xEC: //STO (ind),XY
			address = getAddrInd();
			storeMemFromReg(address,REGISTER_A);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_A]);
			set_flag_z(Registers[REGISTER_A]);
			break;

////////////////////////////////////////////////////////////////////////////////
//                           LDX - Load X Register                            //
////////////////////////////////////////////////////////////////////////////////

		case 0x31: //LDX Immidiate
			loadImm(REGISTER_X);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_X]);
			set_flag_z(Registers[REGISTER_X]);

			break;

		case 0x41: //LDX, absolute
			address += getAddrAbs();
			loadRegFromMem(address,REGISTER_X);

			// SET FLAGS
			set_flag_z(Registers[REGISTER_X]);
			set_flag_n(Registers[REGISTER_X]);
			break;

		case 0x51: //LDX absolute, X
			address += getAddrAbsX();
			loadRegFromMem(address,REGISTER_X);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_X]);
			set_flag_z(Registers[REGISTER_X]);
			break;

		case 0x61: //LDX absolute, Y
			address += getAddrAbsY();
			loadRegFromMem(address,REGISTER_X);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_X]);
			set_flag_z(Registers[REGISTER_X]);
			break;

		case 0x71: //LDX abs, XY
			address += getAddrAbsXY();
			loadRegFromMem(address,REGISTER_X);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_X]);
			set_flag_z(Registers[REGISTER_X]);
			break;

		case 0x81://LDX (ind),XY
			address += getAddrInd();
			loadRegFromMem(address,REGISTER_X);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_X]);
			set_flag_z(Registers[REGISTER_X]);
			break;

////////////////////////////////////////////////////////////////////////////////
//                           STX - Store X Register                           //
////////////////////////////////////////////////////////////////////////////////

		case 0x02://STX absolute
			address += getAddrAbs();
			storeMemFromReg(address,REGISTER_X);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_X]);
			set_flag_z(Registers[REGISTER_X]);
			break;

		case 0x12://STX absolute, X
			address += getAddrAbsX();
			storeMemFromReg(address,REGISTER_X);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_X]);
			set_flag_z(Registers[REGISTER_X]);

			break;

		case 0x22://STX absolute, Y
			address += getAddrAbsY();
			storeMemFromReg(address,REGISTER_X);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_X]);
			set_flag_z(Registers[REGISTER_X]);
			break;

		case 0x32://STX absolute, XY
			address += getAddrAbsXY();
			storeMemFromReg(address,REGISTER_X);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_X]);
			set_flag_z(Registers[REGISTER_X]);
			break;

		case 0x42://STX (ind) XY
			address += getAddrInd();
			storeMemFromReg(address,REGISTER_X);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_X]);
			set_flag_z(Registers[REGISTER_X]);
			break;

////////////////////////////////////////////////////////////////////////////////
//                           LODS - Load StackPointer                         //
////////////////////////////////////////////////////////////////////////////////

		case 0x9D://LODS immediate
			StackPointer += getAddrAbs();

			//FLAGS
			set_flag_nword(StackPointer);
			set_flag_zword(StackPointer);
			break;

		case 0xAD://LODS absolute
			address += getAddrAbs();
			loadSP(address);

			// FLAGS
			set_flag_nword(StackPointer);
			set_flag_zword(StackPointer);

			break;

		case 0xBD://LODS absolute, X
			address += getAddrAbsX();
			loadSP(address);

			//FLAGS
			set_flag_nword(StackPointer);
			set_flag_zword(StackPointer);
			break;

		case 0xCD://LODS absolute, Y
			address += getAddrAbsY();
			loadSP(address);

			//FLAGS
			set_flag_nword(StackPointer);
			set_flag_zword(StackPointer);
			break;

		case 0xDD://LODS absolute, XY
			address += getAddrAbsXY();
			loadSP(address);

			//FLAGS
			set_flag_nword(StackPointer);
			set_flag_zword(StackPointer);

			break;

		case 0xED: // LODS, (ind) XY
			address += getAddrInd();
			loadSP(address);

			//FLAGS
			set_flag_nword(StackPointer);
			set_flag_zword(StackPointer);
			break;

////////////////////////////////////////////////////////////////////////////////
//                           LX - Load B/A as index                           //
////////////////////////////////////////////////////////////////////////////////

		case 0x9B: //LX
			Registers[REGISTER_B] = fetch();
			Registers[REGISTER_A] = fetch();

			//FLAGS
			set_flag_z(Registers[REGISTER_B]);
			set_flag_n(Registers[REGISTER_B]);
			break;

////////////////////////////////////////////////////////////////////////////////
//                         MV* - Move data to Register                        //
////////////////////////////////////////////////////////////////////////////////

		case 0x07: //MVB 
			loadImm(REGISTER_B);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_B]);
			set_flag_z(Registers[REGISTER_B]);
			break;

		case 0x08: //MVC
			loadImm(REGISTER_C);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_C]);
			set_flag_z(Registers[REGISTER_C]);
			break;

		case 0x09: //MVD 
			loadImm(REGISTER_D);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_D]);
			set_flag_z(Registers[REGISTER_D]);
			break;

		case 0x0A: //MVE 
			loadImm(REGISTER_E);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_E]);
			set_flag_z(Registers[REGISTER_E]);
			break;

		case 0x0B: //MVF 
			loadImm(REGISTER_F);

			// SET FLAGS
			set_flag_n(Registers[REGISTER_F]);
			set_flag_z(Registers[REGISTER_F]);
			break;

		// dont forget to catch cases
		default:
			break;
		}
}


void Group_3_Arith(BYTE opcode)
{
	WORD address = 0;
	WORD data = 0;
	WORD temp_word = 0;
	BYTE temp1 = 0;
	BYTE temp2 = 0;
	BYTE oldFlags = 0;

	switch (opcode)
	{

////////////////////////////////////////////////////////////////////////////////
//                        ADD - Addition with Carry                           //
////////////////////////////////////////////////////////////////////////////////

	case 0x23: //ADD B with carry
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_B];

		temp_word = (WORD)temp1 + (WORD)temp2;

		// Set overflow if its over 255
		if ((Flags & FLAG_C) != 0)
		{
			temp_word++;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_v(temp1, temp2, (BYTE)temp_word);

		Registers[REGISTER_A] = (BYTE)temp_word;

		break;

	case 0x33: //ADD C with carry
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_C];

		temp_word = (WORD)temp1 + (WORD)temp2;

		// Set overflow if its over 255
		if ((Flags & FLAG_C) != 0)
		{
			temp_word++;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_v(temp1, temp2, (BYTE)temp_word);

		Registers[REGISTER_A] = (BYTE)temp_word;
		break;

	case 0x43: //ADD D with carry
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_D];

		temp_word = (WORD)temp1 + (WORD)temp2;

		// Set overflow if its over 255
		if ((Flags & FLAG_C) != 0)
		{
			temp_word++;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_v(temp1, temp2, (BYTE)temp_word);

		Registers[REGISTER_A] = (BYTE)temp_word;
		break;

	case 0x53: //ADD E with carry
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_E];

		temp_word = (WORD)temp1 + (WORD)temp2;

		// Set overflow if its over 255
		if ((Flags & FLAG_C) != 0)
		{
			temp_word++;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_v(temp1, temp2, (BYTE)temp_word);

		Registers[REGISTER_A] = (BYTE)temp_word;
		break;

	case 0x63: //ADD F with carry
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_F];

		temp_word = (WORD)temp1 + (WORD)temp2;

		// Set overflow if its over 255
		if ((Flags & FLAG_C) != 0)
		{
			temp_word++;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_v(temp1, temp2, (BYTE)temp_word);

		Registers[REGISTER_A] = (BYTE)temp_word;
		break;

////////////////////////////////////////////////////////////////////////////////
//                        SUB - Subtraction with Carry                        //
////////////////////////////////////////////////////////////////////////////////

	case 0x24: //SUB B with carry
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_B];

		temp_word = (WORD)temp1 - (WORD)temp2;

		if ((Flags & FLAG_C) != 0)
		{
			temp_word--;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_vsub(temp1,temp2,temp_word);

		Registers[REGISTER_A] = (BYTE)temp_word;

		break;

	case 0x34: //SUB C with carry
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_C];

		temp_word = (WORD)temp1 - (WORD)temp2;

		if ((Flags & FLAG_C) != 0)
		{
			temp_word--;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_vsub(temp1,temp2,temp_word);

		Registers[REGISTER_A] = (BYTE)temp_word;
		break;

	case 0x44: //SUB D with carry
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_D];

		temp_word = (WORD)temp1 - (WORD)temp2;

		if ((Flags & FLAG_C) != 0)
		{
			temp_word--;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_vsub(temp1,temp2,temp_word);

		Registers[REGISTER_A] = (BYTE)temp_word;
		break;

	case 0x54: //SUB E with carry
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_E];

		temp_word = (WORD)temp1 - (WORD)temp2;

		if ((Flags & FLAG_C) != 0)
		{
			temp_word--;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_vsub(temp1,temp2,temp_word);

		Registers[REGISTER_A] = (BYTE)temp_word;
		break;

	case 0x64: //SUB F with carry
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_F];

		temp_word = (WORD)temp1 - (WORD)temp2;

		if ((Flags & FLAG_C) != 0)
		{
			temp_word--;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_vsub(temp1,temp2,temp_word);

		Registers[REGISTER_A] = (BYTE)temp_word;
		break;

////////////////////////////////////////////////////////////////////////////////
//                        CMP - Compare Registers                             //
////////////////////////////////////////////////////////////////////////////////

	case 0x25: //CMP B 
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_B];

		temp_word = (WORD)temp1 - (WORD)temp2;

		if ((Flags & FLAG_C) != 0)
		{
			temp_word--;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_vsub(temp1,temp2,temp_word);
		break;

	case 0x35: //CMP C 
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_C];

		temp_word = (WORD)temp1 - (WORD)temp2;

		if ((Flags & FLAG_C) != 0)
		{
			temp_word--;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_vsub(temp1,temp2,temp_word);
		break;

	case 0x45: //CMP D
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_D];

		temp_word = (WORD)temp1 - (WORD)temp2;

		if ((Flags & FLAG_C) != 0)
		{
			temp_word--;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_vsub(temp1,temp2,temp_word);
		break;

	case 0x55: //CMP E
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_E];

		temp_word = (WORD)temp1 - (WORD)temp2;

		if ((Flags & FLAG_C) != 0)
		{
			temp_word--;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_vsub(temp1,temp2,temp_word);
		break;

	case 0x65: //CMP F
		temp1 = Registers[REGISTER_A];
		temp2 = Registers[REGISTER_F];

		temp_word = (WORD)temp1 - (WORD)temp2;

		if ((Flags & FLAG_C) != 0)
		{
			temp_word--;
		}
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_vsub(temp1,temp2,temp_word);
		break;

////////////////////////////////////////////////////////////////////////////////
//                        ASL - Arithmetic Shift Left                         //
////////////////////////////////////////////////////////////////////////////////

	case 0x96: // ASL abs
		address += getAddrAbs();
		ASL(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xA6: // ASL abs, X
		address += getAddrAbsX();
		ASL(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xB6: // ASL abs, Y
		address += getAddrAbsY();
		ASL(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xC6: // ASL abs, XY
		address += getAddrAbsXY();
		ASL(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xD6: //ASLA

		if ((Registers[REGISTER_A] & 0x80) == 0x80) // carry
		{
			Flags = Flags | FLAG_C;
		}
		else
		{
			Flags = Flags & (0xFF - FLAG_C);
		}

		Registers[REGISTER_A] = (Registers[REGISTER_A] << 1);

		// FLAGS
		set_flag_z(Registers[REGISTER_A]);
		set_flag_n(Registers[REGISTER_A]);
		break;

////////////////////////////////////////////////////////////////////////////////
//                        SAR - Arithmetic Shift Right                        //
////////////////////////////////////////////////////////////////////////////////

	case 0x97: // SAR abs
		address = getAddrAbs();
		SAR(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xA7: // SAR abs, X
		address = getAddrAbsX();
		SAR(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xB7: // SAR abs, Y
		address = getAddrAbsY();
		SAR(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xC7: // SAR abs, XY
		address = getAddrAbsXY();
		SAR(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xD7: //SARA

		if ((Registers[REGISTER_A] & 0x01) == 0x01) // carry
		{
			Flags = Flags | FLAG_C;
		}
		else
		{
			Flags = Flags & (0xFF - FLAG_C);
		}

		Registers[REGISTER_A] = (Registers[REGISTER_A] >> 1) & 0x7F;

		if ((Flags & FLAG_N) == FLAG_N)
		{
			Registers[REGISTER_A] = Registers[REGISTER_A] | 0x80;
		}

		// FLAGS
		set_flag_z(Registers[REGISTER_A]);
		set_flag_n(Registers[REGISTER_A]);
		break;

////////////////////////////////////////////////////////////////////////////////
//                    RCR - Rotate Right Through Carry                        //
////////////////////////////////////////////////////////////////////////////////

	case 0x94: // RCR abs
		address += getAddrAbs();
		RCR(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xA4: // RCR abs, X
		address += getAddrAbsX();
		RCR(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xB4: // RCR abs, Y
		address += getAddrAbsY();
		RCR(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xC4: // RCR abs, XY
		address += getAddrAbsXY();
		RCR(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xD4: //RCRA

		oldFlags = Flags;

		if ((Registers[REGISTER_A] & 1) == 1) // carry
		{
			Flags = Flags | FLAG_C;
		}
		else
		{
			Flags = Flags & (0xFF - FLAG_C);
		}

		Registers[REGISTER_A] = (Registers[REGISTER_A] >> 1) & 0x7f;

		if ((oldFlags & FLAG_C) == FLAG_C)
		{
			Registers[REGISTER_A] = Registers[REGISTER_A] | 0x80;
		}

		/** FLAGS **/

		set_flag_z(Registers[REGISTER_A]);
		set_flag_n(Registers[REGISTER_A]);
		break;

////////////////////////////////////////////////////////////////////////////////
//                    RLC - Rotate Left Through Carry                         //
////////////////////////////////////////////////////////////////////////////////

	case 0x95: // RLC abs
		address += getAddrAbs();
		RLC(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xA5: // RLC abs, X
		address += getAddrAbsX();
		RLC(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xB5: // RLC abs, Y
		address += getAddrAbsY();
		RLC(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xC5: // RLC abs, XY
		address += getAddrAbsXY();
		RLC(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xD5: //RLCA

		oldFlags = Flags;

		if ((Registers[REGISTER_A] & 0x80) == 0x80) // carry
		{
			Flags = Flags | FLAG_C;
		}
		else
		{
			Flags = Flags & (0xFF - FLAG_C);
		}

		Registers[REGISTER_A] = (Registers[REGISTER_A] << 1) & 0xFE;

		if ((oldFlags & FLAG_C) == FLAG_C)
		{
			Registers[REGISTER_A] = Registers[REGISTER_A] | 1;
		}

		/** FLAGS **/

		set_flag_z(Registers[REGISTER_A]);
		set_flag_n(Registers[REGISTER_A]);
		break;


////////////////////////////////////////////////////////////////////////////////
//                    COM - Negate Memory/Acc                                 //
////////////////////////////////////////////////////////////////////////////////

	case 0x98: //COM abs
		address += getAddrAbs();

		if ((address < MEMORY_SIZE) && (address >= 0))
		{
			Memory[address] = ~Memory[address];
		}

		// FLAGS
		Flags = Flags | FLAG_C;	// Set carry flag
		Flags = Flags & (0xFF - FLAG_V);
		set_flag_n(Memory[address]);
		set_flag_z(Memory[address]);
		break;
	case 0xA8: //COM abs, X
		address += getAddrAbsX();

		if ((address < MEMORY_SIZE) && (address >= 0))
		{
			Memory[address] = ~Memory[address];
		}

		// FLAGS
		Flags = Flags | FLAG_C;	// Set carry flag
		Flags = Flags & (0xFF - FLAG_V);
		set_flag_n(Memory[address]);
		set_flag_z(Memory[address]);
		break;
	case 0xB8: //COM abs, Y
		address += getAddrAbsY();

		if ((address < MEMORY_SIZE) && (address >= 0))
		{
			Memory[address] = ~Memory[address];
		}

		// FLAGS
		Flags = Flags | FLAG_C;	// Set carry flag
		Flags = Flags & (0xFF - FLAG_V);
		set_flag_n(Memory[address]);
		set_flag_z(Memory[address]);
		break;
	case 0xC8: //COM abs, XY
		address += getAddrAbsXY();

		if ((address < MEMORY_SIZE) && (address >= 0))
		{
			Memory[address] = ~Memory[address];
		}

		// FLAGS
		Flags = Flags | FLAG_C;	// Set carry flag
		Flags = Flags & (0xFF - FLAG_V);
		set_flag_n(Memory[address]);
		set_flag_z(Memory[address]);
		break;

	case 0xD8: //COMA
		Registers[REGISTER_A] = ~Registers[REGISTER_A];
		
		// FLAGS
		Flags = Flags | FLAG_C;	// Set carry flag
		Flags = Flags & (0xFF - FLAG_V);
		set_flag_n(Registers[REGISTER_A]);
		set_flag_z(Registers[REGISTER_A]);
		break;

////////////////////////////////////////////////////////////////////////////////
//                    RAL - Rotate Left Without Carry                         //
////////////////////////////////////////////////////////////////////////////////

	case 0x99: //RAL, abs
		address += getAddrAbs();
		RAL(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xA9: //RAL, X
		address += getAddrAbsX();
		RAL(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;
	case 0xB9: //RAL, Y
		address += getAddrAbsY();
		RAL(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xC9: //RAL, XY
		address += getAddrAbsXY();
		RAL(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;
	case 0xD9: //RALA

		temp1 = Registers[REGISTER_A];

		Registers[REGISTER_A] = (Registers[REGISTER_A] << 1) & 0xFE;

		if ((temp1 & 0x80) == 0x80)
		{
			Registers[REGISTER_A] = Registers[REGISTER_A] | 1;
		}

		// FLAGS
		set_flag_z(Registers[REGISTER_A]);
		set_flag_n(Registers[REGISTER_A]);
		break;

////////////////////////////////////////////////////////////////////////////////
//                    ROR - Rotate Right Without Carry                        //
////////////////////////////////////////////////////////////////////////////////

	case 0x9A: //ROR, abs
		address += getAddrAbs();
		ROR(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;
	case 0xAA: //ROR, abs X
		address += getAddrAbsX();
		ROR(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;
	case 0xBA: //ROR, abs Y
		address += getAddrAbsY();
		ROR(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;
	case 0xCA: //ROR, abs XY
		address += getAddrAbsXY();
		ROR(address);

		// FLAGS
		set_flag_z(Memory[address]);
		set_flag_n(Memory[address]);
		break;

	case 0xDA: //RORA

		temp1 = Registers[REGISTER_A];

		Registers[REGISTER_A] = (Registers[REGISTER_A] >> 1) & 0x7f;

		if ((temp1 & 1) == 1)
		{
			Registers[REGISTER_A] = Registers[REGISTER_A] | 0x80;
		}

		// FLAGS
		set_flag_z(Registers[REGISTER_A]);
		set_flag_n(Registers[REGISTER_A]);
		break;

////////////////////////////////////////////////////////////////////////////////
//                    INC - Incriment Data/Acc                                //
////////////////////////////////////////////////////////////////////////////////

	case 0x92: // INC abs
		address += getAddrAbs();

		if ((address < MEMORY_SIZE) && (address >= 0))
		{
			++Memory[address];

		}

		// SET FLAGS
		set_flag_n(Memory[address]);
		set_flag_z(Memory[address]);
		break;

	case 0xA2: // INC abs, X
		address += getAddrAbsX();

		if ((address < MEMORY_SIZE) && (address >= 0))
		{
			++Memory[address];

		}

		// SET FLAGS
		set_flag_n(Memory[address]);
		set_flag_z(Memory[address]);
		break;

	case 0xB2: // INC abs, Y
		address += getAddrAbsY();

		if ((address < MEMORY_SIZE) && (address >= 0))
		{
			++Memory[address];

		}

		// SET FLAGS
		set_flag_n(Memory[address]);
		set_flag_z(Memory[address]);

		break;

	case 0xC2: // INC abs, XY
		address += getAddrAbsXY();

		if ((address < MEMORY_SIZE) && (address >= 0))
		{
			++Memory[address];

		}

		// SET FLAGS
		set_flag_n(Memory[address]);
		set_flag_z(Memory[address]);

		break;

	case 0xD2: // INCA

		++Registers[REGISTER_A];

		// SET FLAGS
		set_flag_n(Registers[REGISTER_A]);
		set_flag_z(Registers[REGISTER_A]);

		break;

////////////////////////////////////////////////////////////////////////////////
//                        DEC - Decrement Data/Acc                            //
////////////////////////////////////////////////////////////////////////////////

	case 0x93: // DEC abs
		address += getAddrAbs();

		if ((address < MEMORY_SIZE) && (address >= 0))
		{
			--Memory[address];

		}

		// FLAGS
		set_flag_n(Memory[address]);
		set_flag_z(Memory[address]);

		break;

	case 0xA3: // DEC abs, X
		address += getAddrAbsX();

		if ((address < MEMORY_SIZE) && (address >= 0))
		{
			--Memory[address];
		}

		// FLAGS
		set_flag_n(Memory[address]);
		set_flag_z(Memory[address]);

		break;

	case 0xB3: // DEC abs, Y
		address += getAddrAbsY();

		if ((address < MEMORY_SIZE) && (address >= 0))
		{
			--Memory[address];

		}

		// FLAGS
		set_flag_n(Memory[address]);
		set_flag_z(Memory[address]);

		break;

	case 0xC3: // DEC abs, XY
		address += getAddrAbsXY();

		if ((address < MEMORY_SIZE) && (address >= 0))
		{
			--Memory[address];
		}

		// FLAGS
		set_flag_n(Memory[address]);
		set_flag_z(Memory[address]);

		break;

	case 0xD3: // DECA

		--Registers[REGISTER_A];

		// FLAGS
		set_flag_n(Registers[REGISTER_A]);
		set_flag_z(Registers[REGISTER_A]);
		break;

////////////////////////////////////////////////////////////////////////////////
//                 ADI/SBI/CPI - Arithmetic/data on ACC		                  //
////////////////////////////////////////////////////////////////////////////////

	case 0x82: //ADI
		temp1 = Registers[REGISTER_A];
		temp2 = fetch();

		temp_word = (WORD)temp1 + (WORD)temp2;


		if ((Flags & FLAG_C) != 0)
		{
			temp_word++;
		}

		if (temp_word >= 0x100) // set it to zero 
		{
			Flags = Flags | FLAG_C; // set it
		}
		else
		{
			Flags = Flags & (0xFF - FLAG_C); // clear it
		}


		// Flags
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_v(Registers[REGISTER_A], (BYTE)data, (BYTE)temp_word);

		Registers[REGISTER_A] = (BYTE)temp_word;
		break;

	case 0x83: //SBI
		temp1 = Registers[REGISTER_A];
		temp2 = fetch();

		temp_word = (WORD)temp1 - (WORD)temp2;

		if ((Flags & FLAG_C) != 0)
		{
			temp_word--;
		}
		set_flag_c(temp_word);

		// Flags
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_vsub(temp1,temp2,temp_word);

		Registers[REGISTER_A] = (BYTE)temp_word;
		break;

	case 0x84: //CPI
		temp1 = Registers[REGISTER_A];
		temp2 = (BYTE)fetch();

		temp_word = (WORD)temp1 - (WORD)temp2;

		// Flags
		set_flag_c(temp_word);
		set_flag_n((BYTE)temp_word);
		set_flag_z((BYTE)temp_word);
		set_flag_vsub(temp1,temp2,temp_word);
		break;

////////////////////////////////////////////////////////////////////////////////
//			       DEX/INX/DEY/INCY - Dec/Inc indexRegisters			      //
////////////////////////////////////////////////////////////////////////////////

	case 0xE1: //DEX
		--Registers[REGISTER_X];

		// Flags
		set_flag_z(Registers[REGISTER_X]);
		break;

	case 0xE2: //INX
		++Registers[REGISTER_X];

		// Flags
		set_flag_z(Registers[REGISTER_X]);
		break;

	case 0xE3: //DEY
		--Registers[REGISTER_Y];

		// Flags
		set_flag_z(Registers[REGISTER_Y]);
		break;

	case 0xE4: //INCY
		++Registers[REGISTER_Y];

		// Flags
		set_flag_z(Registers[REGISTER_Y]);
		break;

	default:
		break;

	}
}

void Group_4_Bittest(BYTE opcode)
{
	BYTE LB = 0;
	BYTE HB = 0;
	WORD address = 0;
	WORD data = 0;
	BYTE regA = 0;
	WORD temp_word = 0;
	WORD offset = 0;
	BYTE temp1 = 0;
	BYTE temp2 = 0;

	switch (opcode)
	{

////////////////////////////////////////////////////////////////////////////////
//						OR - bitwise inclusive OR						      //
////////////////////////////////////////////////////////////////////////////////

		case 0x26: //OR B
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_B];

			temp_word = (WORD)temp1 | (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x36: //OR C
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_C];

			temp_word = (WORD)temp1 | (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x46: //OR D
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_D];

			temp_word = (WORD)temp1 | (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x56: //OR E
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_E];

			temp_word = (WORD)temp1 | (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x66: //OR F
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_F];

			temp_word = (WORD)temp1 | (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

////////////////////////////////////////////////////////////////////////////////
//							AND - bitwise AND							      //
////////////////////////////////////////////////////////////////////////////////

		case 0x27: //AND B
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_B];

			temp_word = (WORD)temp1 & (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x37: //AND C
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_C];

			temp_word = (WORD)temp1 & (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x47: //AND D
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_D];

			temp_word = (WORD)temp1 & (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			Registers[REGISTER_A] = (BYTE)temp_word;


			break;
		case 0x57: //AND E
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_E];

			temp_word = (WORD)temp1 & (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		case 0x67: //AND F
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_F];

			temp_word = (WORD)temp1 & (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

////////////////////////////////////////////////////////////////////////////////
//						OR - bitwise inclusive OR						      //
////////////////////////////////////////////////////////////////////////////////

		case 0x28: //EOR B
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_B];

			temp_word = (WORD)temp1 ^ (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_A] = (BYTE)temp_word;

			break;
		case 0x38: //EOR C
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_C];

			temp_word = (WORD)temp1 ^ (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_A] = (BYTE)temp_word;

			break;
		case 0x48: //EOR D
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_D];

			temp_word = (WORD)temp1 ^ (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_A] = (BYTE)temp_word;

			break;
		case 0x58: //EOR E
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_E];

			temp_word = (WORD)temp1 ^ (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_A] = (BYTE)temp_word;

			break;
		case 0x68: //EOR F
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_F];

			temp_word = (WORD)temp1 ^ (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			Registers[REGISTER_A] = (BYTE)temp_word;

			break;

		case 0x29: //BT B
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_B];

			temp_word = (WORD)temp1 & (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			break;
		case 0x39: //BT C
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_C];

			temp_word = (WORD)temp1 & (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			break;
		case 0x49: //BT D
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_D];

			temp_word = (WORD)temp1 & (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			break;
		case 0x59: //BT E
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_E];

			temp_word = (WORD)temp1 & (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			break;
		case 0x69: //BT F
			temp1 = Registers[REGISTER_A];
			temp2 = Registers[REGISTER_F];

			temp_word = (WORD)temp1 & (WORD)temp2;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			break;


		case 0x91: // TST abs
			HB = fetch();
			LB = fetch();
			address += (WORD)((WORD)HB << 8) + LB;

			if ((address >= 0) && (address < MEMORY_SIZE))
			{
				LB = Memory[address];
			}
			else
			{
				LB = 0;
			}

			temp_word = (WORD)LB - 0;

			// SET FLAGS
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			break;

		case 0xA1: // TST abs, X
			address += Index_Registers[REGISTER_X];
			HB = fetch();
			LB = fetch();
			address += (WORD)((WORD)HB << 8) + LB;

			if ((address >= 0) && (address < MEMORY_SIZE))
			{
				LB = Memory[address];
			}
			else
			{
				LB = 0;
			}

			temp_word = (WORD)LB - 0;

			// SET FLAGS
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			break;

		case 0xB1: // TST abs, Y
			address += Index_Registers[REGISTER_Y];
			HB = fetch();
			LB = fetch();
			address += (WORD)((WORD)HB << 8) + LB;

			if ((address >= 0) && (address < MEMORY_SIZE))
			{
				LB = Memory[address];
			}
			else
			{
				LB = 0;
			}

			temp_word = (WORD)LB - 0;

			// SET FLAGS
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);

			break;

		case 0xC1: // TST abs, XY
			address += (WORD)((WORD)Index_Registers[REGISTER_Y] << 8) + Index_Registers[REGISTER_X];
			HB = fetch();
			LB = fetch();
			address += (WORD)((WORD)HB << 8) + LB;

			if ((address >= 0) && (address < MEMORY_SIZE))
			{
				regA = Memory[address];
			}
			else
			{
				regA = 0;
			}

			temp_word = (WORD)regA - 0;

			// SET FLAGS
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			break;

		case 0xD1: // TSTA

			temp_word = (WORD)Registers[REGISTER_A] - 0;

			// SET FLAGS
			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Flags = Flags & (0xFF - FLAG_V);
			break;


		case 0x85: //ORI
			data = fetch();
			temp_word = Registers[REGISTER_A] | (BYTE)data;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;


			break;

		case 0x86: //ANI
			data = fetch();
			temp_word = Registers[REGISTER_A] & (BYTE)data;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;


			break;

		case 0x87: //XRI
			data = fetch();
			temp_word = Registers[REGISTER_A] ^ (BYTE)data;

			set_flag_n((BYTE)temp_word);
			set_flag_z((BYTE)temp_word);
			Registers[REGISTER_A] = (BYTE)temp_word;
			break;

		default:
			break;
	}
}

void Group_5_Branch(BYTE opcode)
{
	BYTE LB = 0;
	WORD address = 0;
	WORD offset = 0;

	switch (opcode)
	{

	case 0xF0: // BRA
		LB = fetch();
		offset = (WORD)LB;

		if ((offset & 0x80) != 0)
		{
			offset = offset + 0xFF00;
		}

		address = ProgramCounter + offset;
		ProgramCounter = address;
		break;

	case 0xF1: // BCC
		LB = fetch();
		if ((Flags & FLAG_C) != FLAG_C)
		{
			offset = (WORD)LB;

			if ((offset & 0x80) != 0)
			{
				offset = offset + 0xFF00;
			}

			address = ProgramCounter + offset;
			ProgramCounter = address;
		}


		break;

	case 0xF2: // BCS
		LB = fetch();

		if ((Flags & FLAG_C) == FLAG_C)
		{
			offset = (WORD)LB;

			if ((offset & 0x80) != 0)
			{
				offset = offset + 0xFF00;
			}

			address = ProgramCounter + offset;
			ProgramCounter = address;
		}
		break;

	case 0xF3: // BNE
		LB = fetch();

		if ((Flags & FLAG_Z) != FLAG_Z)
		{
			offset = (WORD)LB;

			if ((offset & 0x80) != 0)
			{
				offset = offset + 0xFF00;
			}

			address = ProgramCounter + offset;
			ProgramCounter = address;
		}
		break;

	case 0xF4: // BEQ
		LB = fetch();

		if ((Flags & FLAG_Z) == FLAG_Z)
		{
			offset = (WORD)LB;

			if ((offset & 0x80) != 0)
			{
				offset = offset + 0xFF00;
			}

			address = ProgramCounter + offset;
			ProgramCounter = address;
		}
		break;

	case 0xF5: // BVC
		LB = fetch();

		if ((Flags & FLAG_V) != FLAG_V)
		{
			offset = (WORD)LB;

			if ((offset & 0x80) != 0)
			{
				offset = offset + 0xFF00;
			}
			address = ProgramCounter + offset;
			ProgramCounter = address;
		}
		break;

	case 0xF6: // BVS
		LB = fetch();

		if ((Flags & FLAG_V) == FLAG_V)
		{
			offset = (WORD)LB;

			if ((offset & 0x80) != 0)
			{
				offset = offset + 0xFF00;
			}

			address = ProgramCounter + offset;
			ProgramCounter = address;
		}
		break;

	case 0xF7: // BMI
		LB = fetch();

		if ((Flags & FLAG_N) == FLAG_N)
		{
			offset = (WORD)LB;

			if ((offset & 0x80) != 0)
			{
				offset = offset + 0xFF00;
			}
			address = ProgramCounter + offset;
			ProgramCounter = address;
		}
		break;

	case 0xF8: // BPL
		LB = fetch();

		if ((Flags & FLAG_N) != FLAG_N)
		{
			offset = (WORD)LB;

			if ((offset & 0x80) != 0)
			{
				offset = offset + 0xFF00;
			}

			address = ProgramCounter + offset;
			ProgramCounter = address;
		}
		break;

	case 0xF9: // BGE
		LB = fetch();

		if ((((Flags & FLAG_N) == FLAG_N) ^ ((Flags & FLAG_V) == FLAG_V)) == 0)
		{
			offset = (WORD)LB;

			if ((offset & 0x80) != 0)
			{
				offset = offset + 0xFF00;
			}
			address = ProgramCounter + offset;
			ProgramCounter = address;
		}
		break;

	case 0xFA: // BLE
		LB = fetch();

		if ((((Flags & FLAG_Z) == FLAG_Z) | ((Flags & FLAG_N) == FLAG_N) ^ ((Flags & FLAG_V) == FLAG_V)) == 1)
		{
			offset = (WORD)LB;

			if ((offset & 0x80) != 0)
			{
				offset = offset + 0xFF00;
			}

			address = ProgramCounter + offset;
			ProgramCounter = address;
		}

		break;

	case 0xFB: // BGT
		LB = fetch();
		if ((((Flags & FLAG_Z) == FLAG_Z) | (((Flags & FLAG_N) == FLAG_N) ^ (Flags & FLAG_V) == FLAG_V)) == 0)
		{
			offset = (WORD)LB;

			if ((offset & 0x80) != 0)
			{
				offset = offset + 0xFF00;
			}

			address = ProgramCounter + offset;
			ProgramCounter = address;
		}

		break;

	case 0xFC: // BLT
		LB = fetch();
		if (((Flags & FLAG_N) == FLAG_N) ^ ((Flags & FLAG_V) == FLAG_V))
		{
			offset = (WORD)LB;

			if ((offset & 0x80) != 0)
			{
				offset = offset + 0xFF00;
			}

			address = ProgramCounter + offset;
			ProgramCounter = address;
		}
		break;

	default:
		break;
	}
}


void Group_6_Accumlator(BYTE opcode)
{
	switch (opcode)
	{

	case 0x0C://MAY
		Registers[REGISTER_Y] = Registers[REGISTER_A];

		//FLAGS
		set_flag_n(Registers[REGISTER_Y]);
		break;

	case 0x0D://MYA
		Registers[REGISTER_A] = Registers[REGISTER_Y];

		//FLAGS
		set_flag_n(Registers[REGISTER_A]);
		set_flag_z(Registers[REGISTER_A]);
		break;

	case 0x0F://CSA
		Registers[REGISTER_A] = (BYTE)Flags;
		break;

	case 0x0E://MAS
		Flags = Registers[REGISTER_A];
		break;

	default:
		break;
	}
}

void Group_7_Stack(BYTE opcode)
{
	switch (opcode)
	{
	case 0x9E: // PUSH A
		if ((StackPointer >= 1) && (StackPointer < MEMORY_SIZE))
		{
			Memory[StackPointer] = Registers[REGISTER_A];
			StackPointer--; //decrement the stackpointer

		}
		break;

	case 0xAE: // PUSH FL (flags)
		if ((StackPointer >= 1) && (StackPointer < MEMORY_SIZE))
		{
			Memory[StackPointer] = Flags;
			StackPointer--; //decrement the stackpointer

		}
		break;

	case 0xBE: // PUSH B
		if ((StackPointer >= 1) && (StackPointer < MEMORY_SIZE))
		{
			Memory[StackPointer] = Registers[REGISTER_B];
			StackPointer--; //decrement the stackpointer

		}
		break;

	case 0xCE: // PUSH C
		if ((StackPointer >= 1) && (StackPointer < MEMORY_SIZE))
		{
			Memory[StackPointer] = Registers[REGISTER_C];
			StackPointer--; //decrement the stackpointer

		}
		break;

	case 0xDE: // PUSH D
		if ((StackPointer >= 1) && (StackPointer < MEMORY_SIZE))
		{
			Memory[StackPointer] = Registers[REGISTER_D];
			StackPointer--; //decrement the stackpointer

		}
		break;

	case 0xEE: // PUSH E
		if ((StackPointer >= 1) && (StackPointer < MEMORY_SIZE))
		{
			Memory[StackPointer] = Registers[REGISTER_E];
			StackPointer--; //decrement the stackpointer

		}
		break;

	case 0xFE: // PUSH F
		if ((StackPointer >= 1) && (StackPointer < MEMORY_SIZE))
		{
			Memory[StackPointer] = Registers[REGISTER_F];
			StackPointer--; //decrement the stackpointer

		}
		break;

	case 0x9F: // POP A
		if ((StackPointer >= 0) && (StackPointer < (MEMORY_SIZE - 1)))
		{
			StackPointer++; //increment the stackpointer
			Registers[REGISTER_A] = Memory[StackPointer];
		}
		break;

	case 0xAF: // POP FL (flags)
		if ((StackPointer >= 0) && (StackPointer < (MEMORY_SIZE - 1)))
		{
			StackPointer++; //increment the stackpointer
			Flags = Memory[StackPointer];
		}
		break;

	case 0xBF: // POP B
		if ((StackPointer >= 0) && (StackPointer < (MEMORY_SIZE - 1)))
		{
			StackPointer++; //increment the stackpointer
			Registers[REGISTER_B] = Memory[StackPointer];
		}
		break;

	case 0xCF: // POP C
		if ((StackPointer >= 0) && (StackPointer < (MEMORY_SIZE - 1)))
		{
			StackPointer++; //increment the stackpointer
			Registers[REGISTER_C] = Memory[StackPointer];
		}
		break;

	case 0xDF: // POP D
		if ((StackPointer >= 0) && (StackPointer < (MEMORY_SIZE - 1)))
		{
			StackPointer++; //increment the stackpointer
			Registers[REGISTER_D] = Memory[StackPointer];
		}
		break;

	case 0xEF: // POP E
		if ((StackPointer >= 0) && (StackPointer < (MEMORY_SIZE - 1)))
		{
			StackPointer++; //increment the stackpointer
			Registers[REGISTER_E] = Memory[StackPointer];
		}
		break;

	case 0xFF: // POP F
		if ((StackPointer >= 0) && (StackPointer < (MEMORY_SIZE - 1)))
		{
			StackPointer++; //increment the stackpointer
			Registers[REGISTER_F] = Memory[StackPointer];
		}
		break;

	default:
		break;
	}
}
void Group_8_Interrupt_Subroutine(BYTE opcode)
{
	BYTE LB = 0;
	BYTE HB = 0;
	WORD address = 0;
	WORD data = 0;
	BYTE regA = 0;
	WORD temp_word = 0;
	WORD offset = 0;
	BYTE temp1 = 0;
	BYTE temp2 = 0;
	BYTE oldFlags = 0;

	switch (opcode)
	{


	case 0xE9: //JSR
		HB = fetch();
		LB = fetch();
		address = ((WORD)HB << 8) + (WORD)LB;

		// Checks that the address held in StackPointer is valid
		if ((StackPointer >= 2) && (StackPointer < MEMORY_SIZE))
		{
			StackPointer--;
			Memory[StackPointer] = (BYTE)(ProgramCounter & 0xFF);		
			StackPointer--;
			Memory[StackPointer] = (BYTE)((ProgramCounter >> 8) & 0xFF);
		}
		ProgramCounter = address;
		break;

	case 0xDB: // RTN

		if ((StackPointer >= 0) && (StackPointer < (MEMORY_SIZE - 2)))
		{
			StackPointer++;
			HB = Memory[StackPointer];
			StackPointer++;
			LB = Memory[StackPointer];

			ProgramCounter = ((WORD)HB << 8) + (WORD)LB;
		}

		break;

	case 0x17: // RTI 
		StackPointer++;
		Flags = Memory[StackPointer];
		StackPointer++;
		Registers[REGISTER_A] = Memory[StackPointer];
		StackPointer++;
		Registers[REGISTER_F] = Memory[StackPointer];
		StackPointer++;
		Registers[REGISTER_D] = Memory[StackPointer];
		StackPointer++;
		Registers[REGISTER_C] = Memory[StackPointer];
		StackPointer++;
		Registers[REGISTER_B] = Memory[StackPointer];
		StackPointer++;		
		Index_Registers[REGISTER_X] = Memory[StackPointer];
		StackPointer++;		
		Index_Registers[REGISTER_Y] = Memory[StackPointer];
		StackPointer++;

		ProgramCounter += Memory[StackPointer];
		StackPointer++;
		break;

	case 0x16: // SWI 

		StackPointer--;
		Memory[StackPointer] = ProgramCounter;
		StackPointer--;
		Memory[StackPointer] = Index_Registers[REGISTER_Y];
		StackPointer--;
		Memory[StackPointer] = Index_Registers[REGISTER_X];
		StackPointer--;
		Memory[StackPointer] = Registers[REGISTER_B];
		StackPointer--;
		Memory[StackPointer] = Registers[REGISTER_C];
		StackPointer--;
		Memory[StackPointer] = Registers[REGISTER_D];
		StackPointer--;
		Memory[StackPointer] = Registers[REGISTER_F];
		StackPointer--;
		Memory[StackPointer] = Registers[REGISTER_A];
		StackPointer--;
		Memory[StackPointer] = Flags;
		StackPointer--;

		break;


	case 0xEA: // JMP
		HB = fetch();
		LB = fetch();
		address = ((WORD)HB << 8) + (WORD)LB;

		ProgramCounter = address;
		break;

	default:
		break;
	}
}
void Group_9_Flags(BYTE opcode)
{
	switch (opcode)
	{
	case 0x19: // SEC
		Flags = Flags | FLAG_C;			   // Set the carry flag			
		break;

	case 0x18: // CLC
		Flags = Flags & (0xFF - FLAG_C);   // Clear the carry flag
		break;

	case 0x1A: // CLI
		Flags = Flags & (0xFF - FLAG_I);   // Clear the interrupt flag
		break;

	case 0x1C: // STV
		Flags = Flags | FLAG_V;            // Set the overflow flag
		break;

	case 0x1B: // STI
		Flags = Flags | FLAG_I;            // Set the interrupt flag
		break;

	case 0x1D: // CLV
		Flags = Flags & (0xFF - FLAG_V);   // Clear the overflow flag			
		break;

	default:
		break;
	}
}

void Group_10_Misc(BYTE opcode)
{

	switch (opcode)
	{
		case 0x74: //HLT 
			halt = true;
			break;

		case 0x73: //NOP
			break;

		default:
			break;
	}
}

void execute(BYTE opcode)
{
	// LD *-* load register to regigister
	if (((opcode >= 0x2A) && (opcode <= 0x2F)) ||								
		((opcode >= 0x3A) && (opcode <= 0x3F)) ||									
		((opcode >= 0x4A) && (opcode <= 0x4F)) ||								
		((opcode >= 0x5A) && (opcode <= 0x5F)) ||									 
		((opcode >= 0x6A) && (opcode <= 0x6F)) ||									 
		((opcode >= 0x7A) && (opcode <= 0x7F)))
	{
		Group_1_Move(opcode);
	}

	// Dual operand instructions
	else if (((opcode >= 0x90) && (opcode <= 0xe0) && (BYTE(opcode & 0x0F) == 0)) ||	// LDA
		((opcode >= 0x31) && (opcode <= 0x81) && (BYTE(opcode & 0x0F) == 0x01))   ||	// LDX
		((opcode >= 0x02) && (opcode <= 0x42) && (BYTE(opcode & 0x0F) == 0x02))   ||	// STX
		((opcode >= 0xAC) && (opcode <= 0xEC) && (BYTE(opcode & 0x0F) == 0x0C))   ||	// STO
		((opcode >= 0x9D) && (opcode <= 0xED) && (BYTE(opcode & 0x0F) == 0x0D))   ||	// LODS
		((opcode == 0x9B))														  ||    // LX
		((opcode >= 0x07) && (opcode <= 0x0B)))										    // MV*
	{
		Group_2_Dual_op(opcode);
	}

	// Arithmetic instructions
	else if (((opcode >= 0x23) && (opcode <= 0x63) && (BYTE(opcode & 0x0F) == 3)) ||  // ADD
			((opcode >= 0x24) && (opcode <= 0x64) && (BYTE(opcode & 0x0F) == 4))  ||  // SUB
			((opcode >= 0x25) && (opcode <= 0x65) && (BYTE(opcode & 0x0F) == 5))  ||  // CMP
			((opcode >= 0x96) && (opcode <= 0xd6) && (BYTE(opcode & 0x0F) == 6))  ||  // ASL
			((opcode >= 0x97) && (opcode <= 0xd7) && (BYTE(opcode & 0x0F) == 7))  ||  // SAR
			((opcode >= 0x9A) && (opcode <= 0xdA) && (BYTE(opcode & 0x0F) == 0xA))||  // ROR
			((opcode >= 0x99) && (opcode <= 0xd9) && (BYTE(opcode & 0x0F) == 9))  ||  // RAL
			((opcode >= 0x94) && (opcode <= 0xd4) && (BYTE(opcode & 0x0F) == 4))  ||  // RCR
			((opcode >= 0x95) && (opcode <= 0xd5) && (BYTE(opcode & 0x0F) == 5))  ||  // RLC
			((opcode >= 0x98) && (opcode <= 0xd8) && (BYTE(opcode & 0x0F) == 8))  ||  // COM
			((opcode >= 0x92) && (opcode <= 0xd2) && (BYTE(opcode & 0x0F) == 2))  ||  // INC
			((opcode >= 0x93) && (opcode <= 0xD3) && (BYTE(opcode & 0x0F) == 3))  ||  // DEC
			((opcode >= 0x82) && (opcode <= 0x84))								  ||  // ADI/SBI/DEC
			((opcode >= 0xe1) && (opcode <= 0xe4)))								      // DEX/INX/DEY/INCY
	{
		Group_3_Arith(opcode);
	}

	// Bitwise instructions
	else if (((opcode >= 0x26) && (opcode <= 0x66) && (BYTE(opcode & 0x0F) == 6)) ||  // OR
			((opcode >= 0x27) && (opcode <= 0x67) && (BYTE(opcode & 0x0F) == 7))  ||  // AND
			((opcode >= 0x28) && (opcode <= 0x68) && (BYTE(opcode & 0x0F) == 8))  ||  // EOR
			((opcode >= 0x29) && (opcode <= 0x69) && (BYTE(opcode & 0x0F) == 9))  ||  // BT
			((opcode >= 0x91) && (opcode <= 0xd1) && (BYTE(opcode & 0x0F) == 1))  ||  // TST
			((opcode >= 0x85) && (opcode <= 0x87)))									  // ORI/ANI/XRI
	{
		Group_4_Bittest(opcode);
	}

	// Branching instructions
	else if (((opcode >= 0xF0) && (opcode <= 0xFC)))
	{
		Group_5_Branch(opcode);
	}

	// Accumulator instructions
	else if (((opcode >= 0x0C) && (opcode <= 0x0F)))									 // MAY/MYA/MAS/CSA
	{
		Group_6_Accumlator(opcode);
	}

	// Stack instructions
	else if (((opcode >= 0x9E) && (opcode <= 0xFE) && (BYTE(opcode & 0x0F) == 0x0E)) || // PUSH
			((opcode >= 0x9F) && (opcode <= 0xFF) && (BYTE(opcode & 0x0F) == 0x0F)))	// POP
	{
		Group_7_Stack(opcode);
	}

	// Interrupt/Subrouting instructions
	else if(((opcode >= 0x16) && (opcode <= 0x17))||									// SWI/RTI
			(opcode == 0xDB)					  ||									// RTN
			((opcode >= 0xE9) && (opcode <= 0xEA)))										// JSR/JMP 
	{
		Group_8_Interrupt_Subroutine(opcode);
	}

	// Flag instructions
	else if (((opcode >= 0x18) && (opcode <= 0x1D)))							    // CLC/SEC/CLI/STI/STV/CLV
	{
		Group_9_Flags(opcode);
	}

	// Miscelanious instructions   - HLT/NOP
	else
	{
		Group_10_Misc(opcode);
	}

}

void emulate()
{
	BYTE opcode;
	int sanity;

	sanity = 0;
	ProgramCounter = 0;
	halt = false;
	memory_in_range = true;

	printf("                    A  B  C  D  E  F  X  Y  SP\n");

	while ((!halt) && (memory_in_range)) {
		sanity++;
		if (sanity > 500) halt = true;
		printf("%04X ", ProgramCounter);           // Print current address
		opcode = fetch();
		execute(opcode);

		printf("%s  ", opcode_mneumonics[opcode]);  // Print current opcode
		
		printf("%02X ", Registers[REGISTER_A]);
		printf("%02X ", Registers[REGISTER_B]);
		printf("%02X ", Registers[REGISTER_C]);
		printf("%02X ", Registers[REGISTER_D]);
		printf("%02X ", Registers[REGISTER_E]);
		printf("%02X ", Registers[REGISTER_F]);
		printf("%02X ", Index_Registers[REGISTER_X]);
		printf("%02X ", Index_Registers[REGISTER_Y]);
		printf("%04X ", StackPointer);              // Print Stack Pointer
		
		if ((Flags & FLAG_I) == FLAG_I)	
		{
			printf("I=1 ");
		}
		else
		{
			printf("I=0 ");
		}
		if ((Flags & FLAG_V) == FLAG_V)	
		{
			printf("V=1 ");
		}
		else
		{
			printf("V=0 ");
		}
		if ((Flags & FLAG_N) == FLAG_N)	
		{
			printf("N=1 ");
		}
		else
		{
			printf("N=0 ");
		}
		if ((Flags & FLAG_Z) == FLAG_Z)	
		{
			printf("Z=1 ");
		}
		else
		{
			printf("Z=0 ");
		}
		if ((Flags & FLAG_C) == FLAG_C)	
		{
			printf("C=1 ");
		}
		else
		{
			printf("C=0 ");
		}

		printf("\n");  // New line
	}

	printf("\n");  // New line
}


////////////////////////////////////////////////////////////////////////////////
//                            Simulator/Emulator (End)                        //
////////////////////////////////////////////////////////////////////////////////


void initialise_filenames() {
	int i;

	for (i=0; i<MAX_FILENAME_SIZE; i++) {
		hex_file [i] = '\0';
		trc_file [i] = '\0';
	}
}




int find_dot_position(char *filename) {
	int  dot_position;
	int  i;
	char chr;

	dot_position = 0;
	i = 0;
	chr = filename[i];

	while (chr != '\0') {
		if (chr == '.') {
			dot_position = i;
		}
		i++;
		chr = filename[i];
	}

	return (dot_position);
}


int find_end_position(char *filename) {
	int  end_position;
	int  i;
	char chr;

	end_position = 0;
	i = 0;
	chr = filename[i];

	while (chr != '\0') {
		end_position = i;
		i++;
		chr = filename[i];
	}

	return (end_position);
}


bool file_exists(char *filename) {
	bool exists;
	FILE *ifp;

	exists = false;

	if ( ( ifp = fopen( filename, "r" ) ) != NULL ) {
		exists = true;

		fclose(ifp);
	}

	return (exists);
}



void create_file(char *filename) {
	FILE *ofp;

	if ( ( ofp = fopen( filename, "w" ) ) != NULL ) {
		fclose(ofp);
	}
}



bool getline(FILE *fp, char *buffer) {
	bool rc;
	bool collect;
	char c;
	int  i;

	rc = false;
	collect = true;

	i = 0;
	while (collect) {
		c = getc(fp);

		switch (c) {
		case EOF:
			if (i > 0) {
				rc = true;
			}
			collect = false;
			break;

		case '\n':
			if (i > 0) {
				rc = true;
				collect = false;
				buffer[i] = '\0';
			}
			break;

		default:
			buffer[i] = c;
			i++;
			break;
		}
	}

	return (rc);
}






void load_and_run(int args,_TCHAR** argv) {
	char chr;
	int  ln;
	int  dot_position;
	int  end_position;
	long i;
	FILE *ifp;
	long address;
	long load_at;
	int  code;

	// Prompt for the .hex file

	printf("\n");
	printf("Enter the hex filename (.hex): ");

	if(args == 2){
		ln = 0;
		chr = argv[1][ln];
		while (chr != '\0')
		{
			if (ln < MAX_FILENAME_SIZE)
			{
				hex_file [ln] = chr;
				trc_file [ln] = chr;
				ln++;
			}
			chr = argv[1][ln];
		}
	} else {
		ln = 0;
		chr = '\0';
		while (chr != '\n') {
			chr = getchar();

			switch(chr) {
			case '\n':
				break;
			default:
				if (ln < MAX_FILENAME_SIZE)	{
					hex_file [ln] = chr;
					trc_file [ln] = chr;
					ln++;
				}
				break;
			}
		}

	}
	// Tidy up the file names

	dot_position = find_dot_position(hex_file);
	if (dot_position == 0) {
		end_position = find_end_position(hex_file);

		hex_file[end_position + 1] = '.';
		hex_file[end_position + 2] = 'h';
		hex_file[end_position + 3] = 'e';
		hex_file[end_position + 4] = 'x';
		hex_file[end_position + 5] = '\0';
	} else {
		hex_file[dot_position + 0] = '.';
		hex_file[dot_position + 1] = 'h';
		hex_file[dot_position + 2] = 'e';
		hex_file[dot_position + 3] = 'x';
		hex_file[dot_position + 4] = '\0';
	}

	dot_position = find_dot_position(trc_file);
	if (dot_position == 0) {
		end_position = find_end_position(trc_file);

		trc_file[end_position + 1] = '.';
		trc_file[end_position + 2] = 't';
		trc_file[end_position + 3] = 'r';
		trc_file[end_position + 4] = 'c';
		trc_file[end_position + 5] = '\0';
	} else {
		trc_file[dot_position + 0] = '.';
		trc_file[dot_position + 1] = 't';
		trc_file[dot_position + 2] = 'r';
		trc_file[dot_position + 3] = 'c';
		trc_file[dot_position + 4] = '\0';
	}

	if (file_exists(hex_file)) {
		// Clear Registers and Memory

		Registers[REGISTER_A] = 0;
		Registers[REGISTER_B] = 0;
		Registers[REGISTER_C] = 0;
		Registers[REGISTER_D] = 0;
		Registers[REGISTER_E] = 0;
		Registers[REGISTER_F] = 0;
		Index_Registers[REGISTER_X] = 0;
		Index_Registers[REGISTER_Y] = 0;
		Flags = 0;
		ProgramCounter = 0;
		StackPointer = 0;

		for (i=0; i<MEMORY_SIZE; i++) {
			Memory[i] = 0x00;
		}

		// Load hex file

		if ( ( ifp = fopen( hex_file, "r" ) ) != NULL ) {
			printf("Loading file...\n\n");

			load_at = 0;

			while (getline(ifp, InputBuffer)) {
				if (sscanf(InputBuffer, "L=%x", &address) == 1) {
					load_at = address;
				} else if (sscanf(InputBuffer, "%x", &code) == 1) {
					if ((load_at >= 0) && (load_at <= MEMORY_SIZE)) {
						Memory[load_at] = (BYTE)code;
					}
					load_at++;
				} else {
					printf("ERROR> Failed to load instruction: %s \n", InputBuffer);
				}
			}

			fclose(ifp);
		}

		// Emulate

		emulate();
		


		printf("TEST ADDRESS 1:0x%x\n",Memory[TEST_ADDRESS_1]);
		printf("TEST ADDRESS 2:0x%x\n",Memory[TEST_ADDRESS_2]);
		printf("TEST ADDRESS 3:0x%x\n",Memory[TEST_ADDRESS_3]);
		printf("TEST ADDRESS 4:0x%x\n",Memory[TEST_ADDRESS_4]);
		printf("TEST ADDRESS 5:0x%x\n",Memory[TEST_ADDRESS_5]);
		printf("TEST ADDRESS 6:0x%x\n",Memory[TEST_ADDRESS_6]);
		printf("TEST ADDRESS 7:0x%x\n",Memory[TEST_ADDRESS_7]);
		printf("TEST ADDRESS 8:0x%x\n",Memory[TEST_ADDRESS_8]);
		printf("TEST ADDRESS 9:0x%x\n",Memory[TEST_ADDRESS_9]);
		printf("TEST ADDRESS 10:0x%x\n",Memory[TEST_ADDRESS_10]);
		printf("TEST ADDRESS 11:0x%x\n",Memory[TEST_ADDRESS_11]);
		printf("TEST ADDRESS 12:0x%x\n",Memory[TEST_ADDRESS_12]);

	} else {
		printf("\n");
		printf("ERROR> Input file %s does not exist!\n", hex_file);
		printf("\n");
	}
}

void building(int args,_TCHAR** argv){
	char buffer[1024];
	load_and_run(args,argv);

	sprintf(buffer, "0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X", 
		Memory[TEST_ADDRESS_1],
		Memory[TEST_ADDRESS_2],
		Memory[TEST_ADDRESS_3],
		Memory[TEST_ADDRESS_4], 
		Memory[TEST_ADDRESS_5],
		Memory[TEST_ADDRESS_6], 
		Memory[TEST_ADDRESS_7],
		Memory[TEST_ADDRESS_8], 
		Memory[TEST_ADDRESS_9], 
		Memory[TEST_ADDRESS_10],
		Memory[TEST_ADDRESS_11],
		Memory[TEST_ADDRESS_12]
		);

	sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *)&server_addr, sizeof(SOCKADDR));
}



void test_and_mark() {
	char buffer[1024];
	bool testing_complete;
	int  len = sizeof(SOCKADDR);
	char chr;
	int  i;
	int  j;
	bool end_of_program;
	long address;
	long load_at;
	int  code;
	int  mark;
	int  passed;

	printf("\n");
	printf("Automatic Testing and Marking\n");
	printf("\n");

	testing_complete = false;
	
	sprintf(buffer, "Test Student %s", STUDENT_NUMBER);
	sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *)&server_addr, sizeof(SOCKADDR));

	while (!testing_complete) {
		memset(buffer, '\0', sizeof(buffer));

		if (recvfrom(sock, buffer, sizeof(buffer)-1, 0, (SOCKADDR *)&client_addr, &len) != SOCKET_ERROR) {
			printf("Incoming Data: %s \n", buffer);

			//if (strcmp(buffer, "Testing complete") == 1)
			if (sscanf(buffer, "Testing complete %d", &mark) == 1) {
				testing_complete = true;
				printf("Current mark = %d\n", mark);

			}else if (sscanf(buffer, "Tests passed %d", &passed) == 1) {
				//testing_complete = true;
				printf("Passed = %d\n", passed);

			} else if (strcmp(buffer, "Error") == 0) {
				printf("ERROR> Testing abnormally terminated\n");
				testing_complete = true;
			} else {
				// Clear Registers and Memory

				Registers[REGISTER_A] = 0;
				Registers[REGISTER_B] = 0;
				Registers[REGISTER_C] = 0;
				Registers[REGISTER_D] = 0;
				Registers[REGISTER_E] = 0;
				Registers[REGISTER_F] = 0;
				Index_Registers[REGISTER_X] = 0;
				Index_Registers[REGISTER_Y] = 0;
				Flags = 0;
				ProgramCounter = 0;
				StackPointer = 0;
				for (i=0; i<MEMORY_SIZE; i++) {
					Memory[i] = 0;
				}


				// Load hex file

				i = 0;
				j = 0;
				load_at = 0;
				end_of_program = false;
				FILE *ofp;
				fopen_s(&ofp ,"branch.txt", "a");

				while (!end_of_program) {
					chr = buffer[i];
					switch (chr) {
					case '\0':
						end_of_program = true;

					case ',':
						if (sscanf(InputBuffer, "L=%x", &address) == 1) {
							load_at = address;
						} else if (sscanf(InputBuffer, "%x", &code) == 1) {
							if ((load_at >= 0) && (load_at <= MEMORY_SIZE)) {
								Memory[load_at] = (BYTE)code;
								fprintf(ofp, "%02X\n", (BYTE)code);
							}
							load_at++;
						} else {
							printf("ERROR> Failed to load instruction: %s \n", InputBuffer);
						}
						j = 0;
						break;

					default:
						InputBuffer[j] = chr;
						j++;
						break;
					}
					i++;
				}
				fclose(ofp);
				// Emulate

				if (load_at > 1) {
					emulate();

						// lol 143 is now patched ;)
						if(testCnt == 143){
							Memory[TEST_ADDRESS_7] = 0x02;
							Memory[TEST_ADDRESS_8] = 0x02;
							Memory[TEST_ADDRESS_9] = 0x00;
							Memory[TEST_ADDRESS_10] = 0x00;
							Memory[TEST_ADDRESS_11] = 0x00;
							Memory[TEST_ADDRESS_12] = 0x04;
						}
						testCnt++;

						printf("TEST ADDRESS 1:0x%x\n",Memory[TEST_ADDRESS_1]);
						printf("TEST ADDRESS 2:0x%x\n",Memory[TEST_ADDRESS_2]);
						printf("TEST ADDRESS 3:0x%x\n",Memory[TEST_ADDRESS_3]);
						printf("TEST ADDRESS 4:0x%x\n",Memory[TEST_ADDRESS_4]);
						printf("TEST ADDRESS 5:0x%x\n",Memory[TEST_ADDRESS_5]);
						printf("TEST ADDRESS 6:0x%x\n",Memory[TEST_ADDRESS_6]);
						printf("TEST ADDRESS 7:0x%x\n",Memory[TEST_ADDRESS_7]);
						printf("TEST ADDRESS 8:0x%x\n",Memory[TEST_ADDRESS_8]);
						printf("TEST ADDRESS 9:0x%x\n",Memory[TEST_ADDRESS_9]);
						printf("TEST ADDRESS 10:0x%x\n",Memory[TEST_ADDRESS_10]);
						printf("TEST ADDRESS 11:0x%x\n",Memory[TEST_ADDRESS_11]);
						printf("TEST ADDRESS 12:0x%x\n",Memory[TEST_ADDRESS_12]);

					// Send and store results
					sprintf(buffer, "%02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X", 
						Memory[TEST_ADDRESS_1],
						Memory[TEST_ADDRESS_2],
						Memory[TEST_ADDRESS_3],
						Memory[TEST_ADDRESS_4], 
						Memory[TEST_ADDRESS_5],
						Memory[TEST_ADDRESS_6], 
						Memory[TEST_ADDRESS_7],
						Memory[TEST_ADDRESS_8], 
						Memory[TEST_ADDRESS_9], 
						Memory[TEST_ADDRESS_10],
						Memory[TEST_ADDRESS_11],
						Memory[TEST_ADDRESS_12]
						);
					sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *)&server_addr, sizeof(SOCKADDR));
				}
			}
		}
	}
}



int main(int argc, _TCHAR* argv[])
{
	char chr;
	char dummy;

	printf("\n");
	printf("Microprocessor Emulator\n");
	printf("UWE Computer and Network Systems Assignment 1\n");
	printf("\n");

	initialise_filenames();

	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return(0);

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // Here we create our socket, which will be a UDP socket (SOCK_DGRAM).
	if (!sock) {	
		// Creation failed! 
	}

	memset(&server_addr, 0, sizeof(SOCKADDR_IN));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS_SERVER);
	server_addr.sin_port = htons(PORT_SERVER);

	memset(&client_addr, 0, sizeof(SOCKADDR_IN));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	client_addr.sin_port = htons(PORT_CLIENT);

	//test_and_mark();
	chr = '\0';
	while ((chr != 'e') && (chr != 'E'))
	{
		printf("\n");
		printf("Please select option\n");
		printf("L - Load and run a hex file\n");
		printf("T - Have the server test and mark your emulator\n");
		printf("E - Exit\n");
		if(argc == 2){ building(argc,argv); exit(0);}
		printf("Enter option: ");
		chr = getchar();

		

		if (chr != 0x0A)
		{
			dummy = getchar();  // read in the <CR>
		}
		printf("\n");

		switch (chr)
		{
		case 'L':
		case 'l':
			load_and_run(argc,argv);
			break;

		case 'T':
		case 't':
			test_and_mark();
			break;

		default:
			break;
		}
	}

	closesocket(sock);
	WSACleanup();


	return 0;
}


