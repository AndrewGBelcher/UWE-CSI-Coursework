#include "coder.h"

char Coder::alphanum[] = {
		'A',
		'B',
		'C',
		'D',
		'E',
		'F',
		'G',
		'H',
		'I',
		'J',
		'K',
		'L',
		'M',
		'N',
		'O',
		'P',
		'Q',
		'R',
		'S',
		'T',
		'U',
		'V',
		'W',
		'X',
		'Y',
		'Z',
		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9'
}; 

char* Coder::morseCode[] = {
		".-",   //A
		"-...", //B
		"-.-.", //C
		"-..",  //D
		".",    //E
		"..-.", //F
		"--.",  //G
		"....", //H
		"..",   //I
		".---", //J
		"-.-",  //K
		".-..", //L
		"--",   //M
		"-.",   //N
		"---",  //O
		".--.", //P
		"--.-", //Q
		".-.",  //R
		"...",  //S
		"-",    //T
		"..-",  //U
		"...-", //V
		".--",  //W
		"-..-", //X
		"-.--", //Y
		"--..", //Z
		"-----", //0
		".----", //1
		"..---", //2
		"...--", //3
		"....-", //4
		".....", //5
		"-....", //6
		"--...", //7
		"---..", //8
		"----." //9
	};
	
// Turn morse signal message into a charactor using the corresponding Ceasar Cipher rotation
char Coder::decodeMsg(int rotation)
{
		
	std::string newMsgString = "";

	for(int i = 0; i < msgLen; i++)
	{
		uBit.serial.printf("morse value:%d\n",(msg[i]) ? 1 : 0);

		if(msg[i])
			newMsgString+='.';
		else
			newMsgString+='-';
	}

	for(int d = 0; d < 36; d++)
	{
		if(!strcmp(morseCode[d],newMsgString.c_str()))
		{
			if(d-rotation< 0)
				return alphanum[36+((d-rotation)%36)];
		
			else
				return	alphanum[(d-rotation)%36];	
		}
	}
	return NULL;
}

// Turn a charactor into a morse code message using the corresponding Ceasar Cipher rotation
void Coder::encodeMsg(char plainMsg, int rotation)
{

	for(int d = 0; d < 36; d++)
	{
		if(alphanum[(d+rotation)%36] == plainMsg)
		{
			uBit.serial.printf("found it! morse:%s  letter:%c, remainder:%d\n",morseCode[d], alphanum[d], (d-rotation)%36);

			for(int i = 0; i < strlen(morseCode[(d+rotation)%36]); i++)
			{
				uBit.serial.printf("morse value:%d\n",(msg[i]) ? 1 : 0);
				if(morseCode[(d+rotation)%36][i] == '.')
				{
					newMsg[i] = true;
					newMsgLen++;
				}
				else
				{
					newMsg[i] = false;
					newMsgLen++;
				}
			}
		}
	}
}


