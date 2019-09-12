#include "MicroBit.h"
#include <vector>
#include <set>
#include <map> 

#define DOT_DURATION 230
#define DASH_DURATION 970
#define STOP_DURATION 2000
#define DIH true
#define DAH false

class Coder
{

private:

	static char alphanum[];
	static char* morseCode[];

public:

	char m;
	int msgLen;
	bool newMsg[30];	
	int newMsgLen;
	std::vector<bool> msg;
	MicroBit uBit; // eh no shared_ptr :/
	int ceasarRotation;

	char decodeMsg(int rotation);
	void encodeMsg(char plainMsg,int rotation);
};
