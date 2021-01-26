/************************************************************************************
* Author:Andrew Belcher																*
* Student ID:	17010347															*
* Created: 3/6/17																	*
* Modified: 4/15/17																	*
* Description: Computer Maze Bot using JSON world builder 							*
*************************************************************************************/



#include "stdafx.h"
#include <winsock2.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include "json.hpp"
#include "fifo_map.hpp"

using namespace nlohmann;

// A workaround to give to use fifo_map as map, we are just ignoring the "less" compare
template<class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = fifo_map<K, V, fifo_map_compare<K>, A>;
using my_json = basic_json<my_workaround_fifo_map>;

#pragma comment(lib, "wsock32.lib")

#define STUDENT_NUMBER		"17010347"
#define STUDENT_FIRSTNAME	"Andrew"
#define STUDENT_FAMILYNAME	"Belcher"

#define IP_ADDRESS_SERVER	"127.0.0.1"
//#define IP_ADDRESS_SERVER "164.11.80.69"


#define PORT_SERVER 0x1984 // We define a port that we are going to use.
#define PORT_CLIENT 0x1985 // We define a port that we are going to use.

#define MAX_FILENAME_SIZE 500

#define MAX_BUFFER_SIZE   5000
#define MAX_STRING_SIZE   200
#define MAX_NO_TOKENS     50


#define MAX_ITEMS_IN_ROOM		20
#define MAX_ITEMS_IN_BACKPACK	50
#define MAX_BACKPACK_VOLUME 40
#define MAX_OPTIONS	50



enum directions
{
	DIRECTION_NORTH = 0,
	DIRECTION_SOUTH = 1,
	DIRECTION_EAST = 2,
	DIRECTION_WEST = 3,
	DIRECTION_UP = 4,
	DIRECTION_DOWN = 5
};


enum direction_possible
{
	DIRECTION_NOT_PRESENT = -1,
	DIRECTION_LOCKED = 0,
	DIRECTION_OPEN = 1
};


enum item_types
{
	ITEM_NONE = 0,
	ITEM_BOOK = 1,
	ITEM_JUNK = 2,
	ITEM_EQUIPMENT = 3,
	ITEM_MANUSCRIPT = 4,
	ITEM_TEST = 5,
	ITEM_OTHER = 10
};

struct Item
{
	int  number;
	int  value;
	int  volume;
};


struct Student
{
	int level;
	int rooms_visited;
	int doors_openned;
	int number_of_moves;
	int score;
};


struct Room
{
	char name[5];
	int  type;
	int  direction[6];
	int  number_of_keys;
	int  keys[4];
	int  number_of_items;
	Item items[MAX_ITEMS_IN_ROOM];
};


struct Backpack
{
	int number_of_items;
	Item items[MAX_ITEMS_IN_BACKPACK];
};



int number_of_options;
int options[MAX_OPTIONS];


Student student;
Room room;
Backpack backpack;

SOCKADDR_IN server_addr;
SOCKADDR_IN client_addr;

SOCKET sock;  // This is our socket, it is the handle to the IO address to read/write packets
WSADATA Data;


char InputBuffer[MAX_BUFFER_SIZE];

char Tokens[MAX_NO_TOKENS][MAX_STRING_SIZE];

char text_student[1000];
char text_backpack[1000];
char text_room[1000];
char text_keys[1000];
char text_items[1000];
char text_options[1000];



void sentOption(int option, int key)
{
	char buffer[100];

	sprintf(buffer, "Option %d, %x", option, key);
	sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *)&server_addr, sizeof(SOCKADDR));
}


/*************************************************************/
/********* Your tactics code starts here *********************/
/*************************************************************/

// option definitions
#define OPTION_MOVE_NORTH	1
#define OPTION_MOVE_SOUTH	2
#define OPTION_MOVE_EAST	3
#define OPTION_MOVE_WEST	4
#define OPTION_MOVE_UP		5
#define OPTION_MOVE_DOWN	6

#define NUMBER_OF_DIRECTIONS 6

#define OPTION_UNLOCK_NORTH	7
#define OPTION_UNLOCK_SOUTH	8
#define OPTION_UNLOCK_EAST	9
#define OPTION_UNLOCK_WEST	10

#define OPTION_BASE_FOR_READS	200
#define OPTION_BASE_FOR_PICKUPS	500
#define OPTION_BASE_FOR_DROPS	800
#define OPTION_BASE_FOR_DOS		1100 // do stuff
#define OPTION_BASE_FOR_EVENTS	1300

// valid keys pulled from world
int keys[] = {0x12fc,0x49cd,0xc75f,0xd976,0xe333,0xab62,0xf904,0x395d,0x34c2,0x69f4,0x58b8,0xc4b6,0x19f4,0x222c,0x6ba3,0x55d5,0x7f07,0x89f3,0x88de,0xbb54,0x200f,0xbcd5,0xe4a8,0x71da,0x29fc,0xa100,0xbe27,0x9f4a,0x8e28,0xd75b,0x090d,0x0172,0xd2f7,0xc567,0x8fd4,0xfd5a,0x8f32,0x1ae1,0x0eb9,0x43ab};
//every possible room name
char roomname[500][5] = {"0Y00", "0Y01", "0Y02", "0Y03", "0Y04", "0Y05", "0Y06", "0Y07", "0Y08", "0Y09", "0Y10", "0Y11", "0Y12", "0Y13", "0Y14", "0Y15", "0Y16", "0Y17", "0Y18", "0Y19", "0Y20", "0Y21", "0Y22", "0Y23", "0Y24", "0Y25", "0Y26", "0Y27", "0Y28", "0Y29", "0Y30", "0Y31", "0Y32", "0Y33", "0Y34", "0Y35", "0Y36", "0Y37", "0Y38", "0Y39", "0Y40", "0Y41", "0Y42", "0Y43", "0Y44", "0Y45", "0Y46", "0Y47", "0Y48", "0Y49", "0Y50", "0Y51", "0Y52", "0Y53", "0Y54", "0Y55", "0Y56", "0Y57", "0Y58", "0Y59", "0Y60", "0Y61", "0Y62", "0Y63", "0Y64", "0Y65", "0Y66", "0Y67", "0Y68", "0Y69", "0Y70", "0Y71", "0Y72", "0Y73", "0Y74", "0Y75", "0Y76", "0Y77", "0Y78", "0Y79", "0Y80", "0Y81", "0Y82", "0Y83", "0Y84", "0Y85", "0Y86", "0Y87", "0Y88", "0Y89", "0Y90", "0Y91", "0Y92", "0Y93", "0Y94", "0Y95", "0Y96", "0Y97", "0Y98", "0Y99", "1Y00", "1Y01", "1Y02", "1Y03", "1Y04", "1Y05", "1Y06", "1Y07", "1Y08", "1Y09", "1Y10", "1Y11", "1Y12", "1Y13", "1Y14", "1Y15", "1Y16", "1Y17", "1Y18", "1Y19", "1Y20", "1Y21", "1Y22", "1Y23", "1Y24", "1Y25", "1Y26", "1Y27", "1Y28", "1Y29", "1Y30", "1Y31", "1Y32", "1Y33", "1Y34", "1Y35", "1Y36", "1Y37", "1Y38", "1Y39", "1Y40", "1Y41", "1Y42", "1Y43", "1Y44", "1Y45", "1Y46", "1Y47", "1Y48", "1Y49", "1Y50", "1Y51", "1Y52", "1Y53", "1Y54", "1Y55", "1Y56", "1Y57", "1Y58", "1Y59", "1Y60", "1Y61", "1Y62", "1Y63", "1Y64", "1Y65", "1Y66", "1Y67", "1Y68", "1Y69", "1Y70", "1Y71", "1Y72", "1Y73", "1Y74", "1Y75", "1Y76", "1Y77", "1Y78", "1Y79", "1Y80", "1Y81", "1Y82", "1Y83", "1Y84", "1Y85", "1Y86", "1Y87", "1Y88", "1Y89", "1Y90", "1Y91", "1Y92", "1Y93", "1Y94", "1Y95", "1Y96", "1Y97", "1Y98", "1Y99", "2Y00", "2Y01", "2Y02", "2Y03", "2Y04", "2Y05", "2Y06", "2Y07", "2Y08", "2Y09", "2Y10", "2Y11", "2Y12", "2Y13", "2Y14", "2Y15", "2Y16", "2Y17", "2Y18", "2Y19", "2Y20", "2Y21", "2Y22", "2Y23", "2Y24", "2Y25", "2Y26", "2Y27", "2Y28", "2Y29", "2Y30", "2Y31", "2Y32", "2Y33", "2Y34", "2Y35", "2Y36", "2Y37", "2Y38", "2Y39", "2Y40", "2Y41", "2Y42", "2Y43", "2Y44", "2Y45", "2Y46", "2Y47", "2Y48", "2Y49", "2Y50", "2Y51", "2Y52", "2Y53", "2Y54", "2Y55", "2Y56", "2Y57", "2Y58", "2Y59", "2Y60", "2Y61", "2Y62", "2Y63", "2Y64", "2Y65", "2Y66", "2Y67", "2Y68", "2Y69", "2Y70", "2Y71", "2Y72", "2Y73", "2Y74", "2Y75", "2Y76", "2Y77", "2Y78", "2Y79", "2Y80", "2Y81", "2Y82", "2Y83", "2Y84", "2Y85", "2Y86", "2Y87", "2Y88", "2Y89", "2Y90", "2Y91", "2Y92", "2Y93", "2Y94", "2Y95", "2Y96", "2Y97", "2Y98", "2Y99", "3Y00", "3Y01", "3Y02", "3Y03", "3Y04", "3Y05", "3Y06", "3Y07", "3Y08", "3Y09", "3Y10", "3Y11", "3Y12", "3Y13", "3Y14", "3Y15", "3Y16", "3Y17", "3Y18", "3Y19", "3Y20", "3Y21", "3Y22", "3Y23", "3Y24", "3Y25", "3Y26", "3Y27", "3Y28", "3Y29", "3Y30", "3Y31", "3Y32", "3Y33", "3Y34", "3Y35", "3Y36", "3Y37", "3Y38", "3Y39", "3Y40", "3Y41", "3Y42", "3Y43", "3Y44", "3Y45", "3Y46", "3Y47", "3Y48", "3Y49", "3Y50", "3Y51", "3Y52", "3Y53", "3Y54", "3Y55", "3Y56", "3Y57", "3Y58", "3Y59", "3Y60", "3Y61", "3Y62", "3Y63", "3Y64", "3Y65", "3Y66", "3Y67", "3Y68", "3Y69", "3Y70", "3Y71", "3Y72", "3Y73", "3Y74", "3Y75", "3Y76", "3Y77", "3Y78", "3Y79", "3Y80", "3Y81", "3Y82", "3Y83", "3Y84", "3Y85", "3Y86", "3Y87", "3Y88", "3Y89", "3Y90", "3Y91", "3Y92", "3Y93", "3Y94", "3Y95", "3Y96", "3Y97", "3Y98", "3Y99", "4Y00", "4Y01", "4Y02", "4Y03", "4Y04", "4Y05", "4Y06", "4Y07", "4Y08", "4Y09", "4Y10", "4Y11", "4Y12", "4Y13", "4Y14", "4Y15", "4Y16", "4Y17", "4Y18", "4Y19", "4Y20", "4Y21", "4Y22", "4Y23", "4Y24", "4Y25", "4Y26", "4Y27", "4Y28", "4Y29", "4Y30", "4Y31", "4Y32", "4Y33", "4Y34", "4Y35", "4Y36", "4Y37", "4Y38", "4Y39", "4Y40", "4Y41", "4Y42", "4Y43", "4Y44", "4Y45", "4Y46", "4Y47", "4Y48", "4Y49", "4Y50", "4Y51", "4Y52", "4Y53", "4Y54", "4Y55", "4Y56", "4Y57", "4Y58", "4Y59", "4Y60", "4Y61", "4Y62", "4Y63", "4Y64", "4Y65", "4Y66", "4Y67", "4Y68", "4Y69", "4Y70", "4Y71", "4Y72", "4Y73", "4Y74", "4Y75", "4Y76", "4Y77", "4Y78", "4Y79", "4Y80", "4Y81", "4Y82", "4Y83", "4Y84", "4Y85", "4Y86", "4Y87", "4Y88", "4Y89", "4Y90", "4Y91", "4Y92", "4Y93", "4Y94", "4Y95", "4Y96", "4Y97", "4Y98", "4Y99"};

int option_count = 0;
int tmpUnlockCnt = 0;
char room_name[10] = " ";

// our data structure for a room based on our json format
struct roomConfig
{
	char roomName[5];
	char adjacentRooms[6][5];
	bool lockStatus[6];
	bool lockStatusPrev[6];
	bool dirStatus[6];
	int keys[6];
};

// make and array of room congigurations for every room
struct roomConfig worldConfig[500];

// method to convert world config to json and save what we found
void logWorldJSON(void)
{
	std::ofstream i("world-test.json");
	my_json j;

	std::string roomNAMES[500] = {"0Y00", "0Y01", "0Y02", "0Y03", "0Y04", "0Y05", "0Y06", "0Y07", "0Y08", "0Y09", "0Y10", "0Y11", "0Y12", "0Y13", "0Y14", "0Y15", "0Y16", "0Y17", "0Y18", "0Y19", "0Y20", "0Y21", "0Y22", "0Y23", "0Y24", "0Y25", "0Y26", "0Y27", "0Y28", "0Y29", "0Y30", "0Y31", "0Y32", "0Y33", "0Y34", "0Y35", "0Y36", "0Y37", "0Y38", "0Y39", "0Y40", "0Y41", "0Y42", "0Y43", "0Y44", "0Y45", "0Y46", "0Y47", "0Y48", "0Y49", "0Y50", "0Y51", "0Y52", "0Y53", "0Y54", "0Y55", "0Y56", "0Y57", "0Y58", "0Y59", "0Y60", "0Y61", "0Y62", "0Y63", "0Y64", "0Y65", "0Y66", "0Y67", "0Y68", "0Y69", "0Y70", "0Y71", "0Y72", "0Y73", "0Y74", "0Y75", "0Y76", "0Y77", "0Y78", "0Y79", "0Y80", "0Y81", "0Y82", "0Y83", "0Y84", "0Y85", "0Y86", "0Y87", "0Y88", "0Y89", "0Y90", "0Y91", "0Y92", "0Y93", "0Y94", "0Y95", "0Y96", "0Y97", "0Y98", "0Y99", "1Y00", "1Y01", "1Y02", "1Y03", "1Y04", "1Y05", "1Y06", "1Y07", "1Y08", "1Y09", "1Y10", "1Y11", "1Y12", "1Y13", "1Y14", "1Y15", "1Y16", "1Y17", "1Y18", "1Y19", "1Y20", "1Y21", "1Y22", "1Y23", "1Y24", "1Y25", "1Y26", "1Y27", "1Y28", "1Y29", "1Y30", "1Y31", "1Y32", "1Y33", "1Y34", "1Y35", "1Y36", "1Y37", "1Y38", "1Y39", "1Y40", "1Y41", "1Y42", "1Y43", "1Y44", "1Y45", "1Y46", "1Y47", "1Y48", "1Y49", "1Y50", "1Y51", "1Y52", "1Y53", "1Y54", "1Y55", "1Y56", "1Y57", "1Y58", "1Y59", "1Y60", "1Y61", "1Y62", "1Y63", "1Y64", "1Y65", "1Y66", "1Y67", "1Y68", "1Y69", "1Y70", "1Y71", "1Y72", "1Y73", "1Y74", "1Y75", "1Y76", "1Y77", "1Y78", "1Y79", "1Y80", "1Y81", "1Y82", "1Y83", "1Y84", "1Y85", "1Y86", "1Y87", "1Y88", "1Y89", "1Y90", "1Y91", "1Y92", "1Y93", "1Y94", "1Y95", "1Y96", "1Y97", "1Y98", "1Y99", "2Y00", "2Y01", "2Y02", "2Y03", "2Y04", "2Y05", "2Y06", "2Y07", "2Y08", "2Y09", "2Y10", "2Y11", "2Y12", "2Y13", "2Y14", "2Y15", "2Y16", "2Y17", "2Y18", "2Y19", "2Y20", "2Y21", "2Y22", "2Y23", "2Y24", "2Y25", "2Y26", "2Y27", "2Y28", "2Y29", "2Y30", "2Y31", "2Y32", "2Y33", "2Y34", "2Y35", "2Y36", "2Y37", "2Y38", "2Y39", "2Y40", "2Y41", "2Y42", "2Y43", "2Y44", "2Y45", "2Y46", "2Y47", "2Y48", "2Y49", "2Y50", "2Y51", "2Y52", "2Y53", "2Y54", "2Y55", "2Y56", "2Y57", "2Y58", "2Y59", "2Y60", "2Y61", "2Y62", "2Y63", "2Y64", "2Y65", "2Y66", "2Y67", "2Y68", "2Y69", "2Y70", "2Y71", "2Y72", "2Y73", "2Y74", "2Y75", "2Y76", "2Y77", "2Y78", "2Y79", "2Y80", "2Y81", "2Y82", "2Y83", "2Y84", "2Y85", "2Y86", "2Y87", "2Y88", "2Y89", "2Y90", "2Y91", "2Y92", "2Y93", "2Y94", "2Y95", "2Y96", "2Y97", "2Y98", "2Y99", "3Y00", "3Y01", "3Y02", "3Y03", "3Y04", "3Y05", "3Y06", "3Y07", "3Y08", "3Y09", "3Y10", "3Y11", "3Y12", "3Y13", "3Y14", "3Y15", "3Y16", "3Y17", "3Y18", "3Y19", "3Y20", "3Y21", "3Y22", "3Y23", "3Y24", "3Y25", "3Y26", "3Y27", "3Y28", "3Y29", "3Y30", "3Y31", "3Y32", "3Y33", "3Y34", "3Y35", "3Y36", "3Y37", "3Y38", "3Y39", "3Y40", "3Y41", "3Y42", "3Y43", "3Y44", "3Y45", "3Y46", "3Y47", "3Y48", "3Y49", "3Y50", "3Y51", "3Y52", "3Y53", "3Y54", "3Y55", "3Y56", "3Y57", "3Y58", "3Y59", "3Y60", "3Y61", "3Y62", "3Y63", "3Y64", "3Y65", "3Y66", "3Y67", "3Y68", "3Y69", "3Y70", "3Y71", "3Y72", "3Y73", "3Y74", "3Y75", "3Y76", "3Y77", "3Y78", "3Y79", "3Y80", "3Y81", "3Y82", "3Y83", "3Y84", "3Y85", "3Y86", "3Y87", "3Y88", "3Y89", "3Y90", "3Y91", "3Y92", "3Y93", "3Y94", "3Y95", "3Y96", "3Y97", "3Y98", "3Y99", "4Y00", "4Y01", "4Y02", "4Y03", "4Y04", "4Y05", "4Y06", "4Y07", "4Y08", "4Y09", "4Y10", "4Y11", "4Y12", "4Y13", "4Y14", "4Y15", "4Y16", "4Y17", "4Y18", "4Y19", "4Y20", "4Y21", "4Y22", "4Y23", "4Y24", "4Y25", "4Y26", "4Y27", "4Y28", "4Y29", "4Y30", "4Y31", "4Y32", "4Y33", "4Y34", "4Y35", "4Y36", "4Y37", "4Y38", "4Y39", "4Y40", "4Y41", "4Y42", "4Y43", "4Y44", "4Y45", "4Y46", "4Y47", "4Y48", "4Y49", "4Y50", "4Y51", "4Y52", "4Y53", "4Y54", "4Y55", "4Y56", "4Y57", "4Y58", "4Y59", "4Y60", "4Y61", "4Y62", "4Y63", "4Y64", "4Y65", "4Y66", "4Y67", "4Y68", "4Y69", "4Y70", "4Y71", "4Y72", "4Y73", "4Y74", "4Y75", "4Y76", "4Y77", "4Y78", "4Y79", "4Y80", "4Y81", "4Y82", "4Y83", "4Y84", "4Y85", "4Y86", "4Y87", "4Y88", "4Y89", "4Y90", "4Y91", "4Y92", "4Y93", "4Y94", "4Y95", "4Y96", "4Y97", "4Y98", "4Y99"};
	std::string directions[6] = {"north","south","east","west","up","down"};

	for (int i = 0; i < 500; i++)
	{
		for(int d = 0; d < 6; d++)
		{		
			std::stringstream stream;
			stream << std::hex << worldConfig[i].keys[d];
			std::string key( stream.str() );

			char buffer [5];
			_itoa (worldConfig[i].keys[d],buffer,16);
			std::string key1 = buffer;

			j["rooms"][roomNAMES[i]]["directions"][directions[d]]["destinationRoomId"] = worldConfig[i].adjacentRooms[d];
			j["rooms"][roomNAMES[i]]["directions"][directions[d]]["isLocked"] = worldConfig[i].lockStatus[d];
			j["rooms"][roomNAMES[i]]["directions"][directions[d]]["isLockedInitially"] = worldConfig[i].lockStatusPrev[d];
			j["rooms"][roomNAMES[i]]["directions"][directions[d]]["isPresent"] = worldConfig[i].dirStatus[d];
			j["rooms"][roomNAMES[i]]["directions"][directions[d]]["pairedKey"] = key1;
		}
	}
	i << std::setw(4) << j << std::endl;
}

// method to log the current rooms information
void logRoom(Room room)
{
	for (int i = 0; i < 500; i++)
	{
		if(strcmp(room.name, worldConfig[i].roomName)==0)
		{
			for(int d = 0; d < 6; d++)
			{		
				if(room.direction[d] == 0)worldConfig[i].lockStatusPrev[d] = true;
				if(room.direction[d] == -1)worldConfig[i].dirStatus[d] = false;
				if(room.direction[d] == 1 || room.direction[d] == 0)worldConfig[i].dirStatus[d] = true;
			}
		}
	}
}

// method to check if we have unlocked a door
void chkUnlockedDoors(Room room, int prevKey, int prevOpt, int prevLock)
{
	for (int i = 0; i < 500; i++)
	{
		if(strcmp(room.name, worldConfig[i].roomName)==0)
		{	
			if(student.doors_openned > tmpUnlockCnt)
			{
				int d = prevOpt - 7;
				worldConfig[i].keys[d] = prevKey;
			}
		}
	}
}

// method to log where the door we took lead to, only helpful for when we return to the room we leave
// however will get logged in json for next time :)
void logDestRoomId(Room room, int prevOpt, char prevRoom[5])
{
	if(prevOpt >= 0 && prevOpt < 5)
	{ 
		for (int i = 0; i < 500; i++)
		{
			if(strcmp(prevRoom, worldConfig[i].roomName)==0)
			{	
				strcpy(worldConfig[i].adjacentRooms[prevOpt],room.name);
			}
		}
	}
}

// method to count how many items of value are in the current room
int cntValuedItems(Room room)
{
	int valuedItemCnt = 0;
	for (int i = 0; i < room.number_of_items; i++)
	{
		if(room.items[i].value > 0)valuedItemCnt++;	
	}
	return valuedItemCnt;
}

// check if we need to read something in the room
bool doWeNeedToReadItem(Room room)
{
	for (int i = 0; i < number_of_options; i++)
	{
		if (options[i] >= OPTION_BASE_FOR_READS && options[i] < 300)
		{
			return true;
		}
	}
	return false;
}

// check if we need to pickup a valuable item in the room
bool doWeNeedToPickUpValuedItem(Room room)
{ 
	for (int i = 0; i < room.number_of_items; i++)
	{
		for (int d = 0; d < backpack.number_of_items; d++)
		{
			if (backpack.items[d].number != room.items[i].number && room.items[i].value > 0)
			{
				return true;
			}
		}
	}
	return false;
}

// method to check if its a item of value
bool weNeedItem(Room room, int index)
{
	for (int i = 0; i < backpack.number_of_items; i++)
	{
		if(backpack.items[i].number == room.items[index].number)return false;
	}
	return true;
}

// method to get number of directions in order to choose an option for item select
int getNumOfDirectionsItemSelect(Room room)
{
	int numOfDir=0;
	for (int i=0; i < 7; i++)
	{
		if (room.direction[i] == 0 || room.direction[i] == 1)
		{
			numOfDir++;
		}
	}
	return numOfDir;
}

// method to get number of directions in order to choose an option for moving
int getNumOfDirectionsMove(Room room)
{
	int numOfDir=0;
	for (int i=0; i < 6; i++)
	{
		if (room.direction[i] > -1 || room.direction[i] < 3)
		{
			numOfDir++;
		}
	}
	return numOfDir;
}

// method to check if there is a locked door
bool isLockedRoom(Room room)
{
	for (int i = 0; i < number_of_options; i++)
	{
		if(options[i] < 11 && options[i] > 6)return true;
	}
	return false;
}

// method to find the first locked door
int firstLockedDoor(Room room)
{
	char* dirArray[6] = {"north","south","east","west","up","down"}; 

	for (int i = 0; i < number_of_options; i++)
	{
		if (options[i] < 11 && options[i] > 6)
		{
			printf("first locked door is:%d %s\n",options[i],dirArray[options[i]-7]);
			//getchar();
			return options[i]; // 7 for north
		}
	}
}

// method to mark a door in a room that has been entered through
int optToDir(Room room, int option)
{
	int optIndex = 0;
	for (int i = 0; i < 7; i++)
	{
		if (room.direction[i] == 0 || room.direction[i] == 1)
		{
			if(optIndex == option)
			{
				return i;
			}
			optIndex++;
		}
	}
}

// method that finds the number of directions we can take
int numberOfDir(Room room)
{
	int dirCnt = 0;
	for (int i = 0; i < 7; i++)
	{
		if (room.direction[i] == 0 || room.direction[i] == 1)
		{
			dirCnt++;
		}
	}
	return dirCnt;
}

// method to check if there is an event we should attend
bool thereIsEvent(Room room)
{
	for (int i = 0; i < number_of_options; i++)
	{
		if(options[i] >= OPTION_BASE_FOR_EVENTS)return true;
	}
	return false;
}

// method to check if there is something to do
bool thereIsDOS(Room room)
{
	for (int i = 0; i < number_of_options; i++)
	{
		if(options[i] >= OPTION_BASE_FOR_DOS && options[i] < OPTION_BASE_FOR_DOS)return true;
	}
	return false;
}

// room entry structure used in our room log, its never saved
struct roomEntry
{
	int directions[6] = {0,0,0,0,0,0};
	int keys[6] = { 0,0,0,0,0,0 };
	char roomName[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}; 

};

// just a large array of room entries
struct roomEntry roomLog[4000];

// method to check if a room has not been logged
bool roomIsntLogged(Room room)
{
	for (int i = 0; i < 4000; i++)
	{
		char tmpstr[5];
		memcpy(tmpstr,roomLog[i].roomName,5);
		if(strcmp(room.name, tmpstr)==0)return false;
		//if(room.name != roomLog[i].roomName)return false;
		//printf("%s\n",roomLog[i].roomName);
	}
	return true;
}

// just prints out 40 rooms we have visited, helpful for debugging movement after the bot is finished
void printRooms(void)
{
	for (int i = 0; i < 40; i++)
	{
		printf("room name:%s  ", roomLog[i].roomName);
		printf("%d ", roomLog[i].directions[0]);
		printf("%d ", roomLog[i].directions[1]);
		printf("%d ", roomLog[i].directions[2]);
		printf("%d ", roomLog[i].directions[3]);
		printf("%d ", roomLog[i].directions[4]);
		printf("%d \n", roomLog[i].directions[5]);
	}
}

// method to log which direction we have taken
void logDirection(Room room, int option)
{
	for (int i = 0; i < 4000; i++)
	{
		if (strcmp(room.name, roomLog[i].roomName) == 0)
		{
			roomLog[i].directions[optToDir(room,option)] = 2;
		}
	}
}

// if the direction has been taken (2) then its true
bool dontGoThatWay(Room room, int option)
{
	for (int i = 0; i < 500; i++)
	{
		if (strcmp(room.name, worldConfig[i].roomName) == 0)
		{
			for(int d = 0; d < 500; i++)
			{
				if (strcmp(worldConfig[i].adjacentRooms[optToDir(room, option)], roomLog[i].roomName) == 0)
				{
					return true;
				}
			}
		}
	}
	return false;
}

// method to pull key for door from our parsed json data structure
int getKeyFromWorld(Room room, int opt,FILE* fd)
{
	int dir = -1;	
	int key;
	char* dirArray[6] = {"north","south","east","west","up","down"}; 


	if (opt < 11 && opt > 6)
	{
		dir = opt - 7;
	}
	if (opt > 10 || opt < 7)
	{
		return -1;
	}

	for (int i = 0; i < 500; i++)
	{
		if (strcmp(worldConfig[i].roomName, room.name) == 0)
		{
			key = worldConfig[i].keys[dir];
			//fprintf(fd,"6: unlocking room:%s with key:0x%x dir:%d option:%d\n",room.name, worldConfig[i].keys[dir], dir, opt);
			printf("key:0x%04x dir:%d opt:%d dirname:%s\n",key, dir, opt, dirArray[dir]);
			//getchar();
			return worldConfig[i].keys[dir];
		}
	}
}


// our hard coded path iterator
int counter = 0;

// WINNER WINNER CHICKEN DINNER 64 ON EVERY FRESH SERVER BOOT! :)
int keyPath[] = {0x0000, 0xe333, 0, 0, 0, 0xc567, 0, 0x55d5, 0xbe27, 0, 0, 0, 0x222c, 0, 0, 0x49cd, 0, 0x49cd, 0, 0, 0, 0, 0xab62, 0, 0x34c2, 0, 0, 0, 0, 0, 0, 0, 0, 0x6ba3, 0x58b8, 0, 0x7f07, 0, 0xe4a8, 0xd2f7, 0, 0x29fc, 0, 0, 0, 0xc4b6, 0x8fd4, 0, 0xc75f, 0, 0, 0, 0, 0, 0, 0, 0, 0xbcd5, 0x88de, 0, 0, 0xab62, 0x58b8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x69f4, 0, 0, 0x19f4, 0x49cd, 0, 0, 0x19f4, 0, 0, 0, 0xbe27, 0, 0xbe27, 0, 0, 0, 0x0eb9, 0x88de, 0, 0, 0x7f07, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xc4b6, 0x222c, 0, 0, 0x200f, 0xd75b, 0, 0xc567, 0xbb54, 0, 0, 0, 0, 0, 0, 0, 0x88de, 0, 0, 0, 0, 0, 0x200f, 0, 0xd2f7, 0, 0xfd5a, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x8e28, 0, 0x29fc, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xe333, 0, 0, 0, 0, 0xc75f, 0, 0, 0, 0, 0xc567, 0, 0, 0x6ba3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x8e28, 0, 0x395d, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xbb54, 0x49cd, 0, 0x6ba3, 0x69f4, 0, 0x200f, 0, 0xbe27, 0x71da, 0, 0, 0, 0, 0xd2f7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x9f4a, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x34c2, 0x395d, 0, 0, 0, 0, 0, 0, 0, 0, 0x43ab, 0, 0, 0, 0, 0x43ab, 0, 0, 0, 0x7f07, 0xbcd5, 0, 0, 0xbe27, 0, 0, 0x89f3, 0, 0, 0xc75f, 0, 0, 0, 0x71da, 0xc567, 0, 0x19f4, 0, 0, 0, 0x49cd, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1ae1, 0, 0, 0, 0x8fd4, 0, 0x7f07, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1ae1, 0xd75b, 0, 0x7f07, 0x29fc, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x222c, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x9f4a, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01f4, 0x01f4, 0x01f4, 0x01f4, 0x01f4, 0x01f4, 0x01f4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x71da, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xd75b, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x8e28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xd2f7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x49cd, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x58b8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x29fc, 0, 0, 0, 0, 0, 0, 0, 0x395d, 0x200f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xf904, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xc4b6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01f4, 0, 0x01f4, 0, 0x01f4, 0, 0, 0, 0, 0, 0xd2f7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xc567, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x19f4, 0x49cd, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0000, 0, 0, 0x0000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01f4, 0x01f4, 0x01f4, 0x01f4, 0x01f4, 0, 0x01f4, 0x01f4, 0x01f4, 0, 0x01f4, 0x01f4, 0x01f4, 0, 0x01f4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x12fc, 0, 0, 0, 0, 0, 0, 0, 0, 0x89f3, 0, 0, 0, 0, 0, 0x34c2, 0, 0xc4b6, 0xc4b6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x200f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xbcd5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x43ab, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x49cd, 0x6ba3, 0, 0, 0, 0, 0, 0x1ae1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x200f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xbb54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x71da, 0, 0xa100, 0x8fd4, 0, 0x090d, 0, 0x7f07, 0, 0, 0, 0, 0x090d, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x12fc, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int optPath[] = {0, 9, 0, 0, 3, 8, 3, 8, 9, 0, 0, 3, 8, 0, 2, 10, 2, 8, 2, 2, 2, 4, 7, 2, 8, 1, 0, 1322, 1, 0, 1322, 0, 4, 8, 10, 4, 7, 4, 7, 10, 1, 10, 2, 1, 1, 7, 10, 4, 8, 239, 239, 2, 2, 530, 530, 1, 4, 8, 10, 1, 4, 7, 10, 261, 3, 3, 561, 4, 4, 1, 3, 2, 261, 261, 2, 2, 10, 582, 2, 8, 10, 1355, 2, 10, 1, 882, 3, 8, 2, 9, 3, 609, 2, 8, 10, 609, 4, 8, 531, 1, 4, 1, 3, 831, 2, 609, 909, 2, 2, 8, 9, 633, 4, 7, 10, 4, 7, 8, 516, 2, 933, 633, 4, 816, 2, 10, 3, 933, 633, 2, 3, 9, 3, 8, 1, 9, 500, 2, 933, 2, 1, 4, 3, 4, 1, 3, 1233, 1233, 3, 3, 1201, 800, 500, 4, 1100, 2, 8, 2, 9, 4, 502, 1, 4, 4, 1, 4, 3, 633, 633, 3, 633, 633, 4, 3, 2, 4, 1, 4, 4, 8, 3, 633, 633, 2, 8, 1, 4, 4, 5, 9, 1233, 2, 8, 2, 633, 635, 635, 1, 1, 3, 3, 4, 3, 4, 633, 1, 10, 1, 9, 2, 4, 1233, 3, 1233, 2, 633, 3, 633, 4, 1, 4, 3, 1233, 1233, 3, 616, 602, 616, 6, 4, 3, 6, 4, 4, 6, 7, 9, 1, 7, 9, 1, 9, 1, 7, 9, 1357, 1357, 1357, 1, 9, 2, 2, 2, 2, 1, 2, 5, 6, 3, 2, 2, 516, 3, 1, 2, 4, 2, 9, 2, 518, 1, 216, 1, 1, 4, 216, 216, 5, 3, 2, 8, 9, 4, 592, 1326, 1326, 4, 2, 592, 3, 9, 615, 615, 315, 3, 7, 1, 2, 2, 8, 10, 509, 3, 9, 4, 2, 10, 612, 2, 9, 2, 627, 4, 7, 10, 1, 10, 327, 327, 1, 7, 327, 327, 3, 3, 1, 327, 327, 3, 4, 2, 4, 2, 2, 4, 4, 3, 619, 619, 327, 4, 2, 619, 4, 619, 2, 8, 3, 619, 3, 9, 2, 10, 270, 270, 1, 3, 507, 507, 327, 4, 2, 570, 507, 327, 570, 3, 4, 1, 507, 327, 4, 2, 507, 327, 1, 4, 3, 3, 3, 558, 4, 3, 4, 2, 597, 4, 4, 597, 1158, 1158, 1, 2, 3, 1, 4, 3, 1158, 4, 3, 2, 1158, 1158, 1, 1, 7, 9, 4, 8, 10, 3, 4, 2, 3, 2, 3, 1, 2, 597, 4, 597, 3, 597, 4, 597, 597, 1, 1, 2, 597, 1158, 1, 2, 1158, 4, 2, 2, 9, 1, 1, 3, 1158, 597, 1, 1158, 3, 4, 3, 2, 4, 1, 527, 1158, 1, 4, 527, 1158, 1158, 527, 3, 2, 1, 4, 2, 4, 4, 527, 527, 2, 1, 527, 3, 4, 527, 3, 1, 4, 527, 3, 1, 3, 3, 4, 1158, 1158, 4, 2, 2, 3, 1, 4, 1158, 3, 4, 603, 1158, 4, 3, 603, 3, 1, 603, 3, 4, 3, 603, 3, 4, 2, 563, 3, 8, 2, 11, 4, 527, 2, 4, 527, 1199, 4, 3, 527, 527, 2, 1, 1, 527, 4, 4, 2, 527, 1199, 1, 527, 3, 4, 1, 2, 1199, 3, 1, 2, 3, 3, 11, 527, 2, 1, 4, 3, 2, 4, 527, 527, 2, 3, 1, 1199, 4, 3, 527, 1199, 4, 4, 527, 1199, 1, 4, 2, 2, 1, 3, 3, 2, 4, 1, 3, 527, 2, 4, 2, 4, 8, 3, 527, 4, 1199, 6, 527, 527, 3, 4, 527, 2, 1199, 1199, 1, 5, 5, 8, 9, 8, 10, 8, 9, 8, 1, 56431, 11, 6, 56431, 1199, 1199, 3, 56431, 1199, 1, 56431, 56431, 1199, 4, 2, 2, 3, 8, 576, 576, 56431, 4, 576, 3, 4, 576, 576, 56431, 4, 6, 3, 3, 1199, 1199, 3, 8, 1199, 1199, 4, 56431, 1199, 1199, 3, 56431, 2, 9, 1, 1317, 1199, 2, 4, 2, 1, 232, 1, 4, 532, 542, 1, 532, 532, 542, 4, 1, 1, 7, 1, 2, 1302, 1302, 1, 2, 1302, 1302, 574, 2, 2, 5, 6, 569, 1302, 1302, 1, 569, 569, 2, 3, 1, 1, 506, 12, 4, 3, 12, 4, 506, 2, 206, 3, 2, 506, 3, 12, 4, 3, 12, 4, 206, 4, 3, 4, 4, 3, 3, 4, 506, 206, 206, 4, 506, 1, 206, 3, 4, 3, 12, 506, 3, 4, 534, 3, 4, 534, 12, 4, 534, 3, 12, 534, 12, 4, 234, 2, 1, 1, 9, 4, 534, 3, 12, 4, 2, 234, 234, 4, 534, 2, 5, 6, 534, 1, 234, 3, 2, 534, 234, 3, 4, 2, 234, 234, 1, 4, 1, 234, 2, 534, 534, 234, 4, 5, 534, 534, 234, 3, 1, 3, 2, 234, 3, 1, 234, 234, 1, 12, 4, 3, 12, 4, 234, 3, 12, 4, 3, 2, 4, 3, 534, 234, 4, 3, 2, 534, 4, 534, 534, 4, 3, 1, 534, 1, 234, 4, 534, 534, 3, 1, 12, 1, 234, 1, 4, 4, 553, 2, 8, 1, 562, 2, 3, 4, 562, 262, 234, 234, 1, 562, 262, 2, 1, 562, 262, 2, 262, 262, 2, 3, 1, 2, 234, 4, 2, 2, 3, 2, 1, 3, 234, 234, 4, 3, 234, 234, 4, 262, 234, 234, 2, 262, 234, 3, 1, 1, 3, 12, 262, 2, 4, 1, 2, 234, 4, 3, 262, 2, 4, 1, 2, 262, 234, 4, 3, 2, 262, 234, 1, 262, 262, 3, 4, 1, 3, 234, 234, 3, 4, 1, 3, 12, 3, 4, 2, 3, 12, 4, 597, 3, 12, 4, 3, 2, 597, 12, 1333, 597, 3, 13, 1, 226, 2, 4, 1, 597, 2, 13, 1, 4, 3, 13, 1, 4, 2, 4, 3, 2, 599, 13, 4, 4, 3, 4, 1, 3, 2, 4, 1, 3, 2, 3, 1, 1199, 1199, 4, 599, 2, 3, 1, 1199, 4, 3, 599, 1199, 1199, 4, 599, 1, 1199, 1, 13, 4, 599, 599, 1199, 1199, 1, 599, 2, 3, 13, 1, 599, 599, 1, 2, 2, 1, 3, 13, 4, 599, 1199, 1199, 1, 4, 2, 4, 1, 4, 599, 2, 3, 2, 2, 3, 3, 4, 599, 1, 4, 599, 1199, 4, 1, 3, 1199, 1, 4, 2, 1199, 4, 4, 2, 1199, 1199, 2, 2, 1199, 1199, 1, 2, 1199, 1199, 4, 4, 599, 599, 1199, 4, 599, 3, 1199, 599, 1199, 4, 599, 1, 3, 4, 599, 2, 1, 1, 1, 3, 1, 4, 2, 3, 1, 3, 1, 1199, 1, 3, 8, 605, 14, 1, 4, 3, 14, 3, 7, 9, 4, 3, 2, 623, 14, 1, 623, 213, 213, 1, 4, 8, 213, 213, 2, 4, 2, 1317, 2, 1, 4, 1317, 1317, 213, 4, 1317, 1317, 3, 4, 4, 1317, 2, 8, 540, 1318, 1, 2, 3, 3, 4, 2, 2, 3, 1, 1325, 612, 612, 2, 4, 2, 3, 2, 4, 1325, 1325, 3, 1, 4, 3, 13, 4, 2, 3, 4, 1, 2, 612, 1, 1, 2, 3, 13, 4, 1325, 3, 13, 4, 3, 2, 4, 2, 1325, 612, 612, 4, 1325, 1, 3, 1, 4, 2, 3, 2, 1, 612, 1, 4, 612, 4, 4, 1325, 3, 1, 2, 1325, 612, 4, 4, 1, 3, 4, 1, 1325, 1325, 3, 4, 1, 2, 612, 612, 4, 1325, 612, 612, 4, 4, 2, 4, 1, 1325, 1325, 1, 2, 2, 3, 4, 1, 2, 3, 4, 1325, 612, 1, 3, 1, 1, 612, 4, 3, 4, 612, 1, 3, 4, 2, 4, 592, 592, 1326, 1, 1, 5, 5, 8, 13, 8, 13, 8, 1, 56431, 13, 3, 1, 9, 1326, 1, 4, 252, 3, 13, 4, 252, 252, 4, 2, 8, 252, 1, 552, 552, 2, 3, 1, 4, 2, 252, 4, 4, 552, 4, 1, 2, 2, 3, 4, 1, 252, 252, 1, 1, 252, 3, 8, 9, 552, 4, 4, 3, 3, 252, 252, 4, 3, 3, 294, 14, 2, 4, 294, 3, 1, 594, 4, 2, 14, 1, 252, 2, 14, 294, 3, 14, 4, 294, 14, 1, 4, 4, 3, 3, 252, 252, 10, 3, 2, 4, 294, 3, 4, 294, 3, 14, 4, 294, 3, 14, 4, 294, 14, 1, 252, 252, 2, 1, 252, 252, 2, 3, 1, 294, 14, 4, 294, 4, 252, 3, 294, 294, 252, 2, 14, 1, 4, 4, 2, 1, 3, 252, 4, 10, 3, 4, 10, 2, 294, 3, 2, 4, 2, 552, 252, 6, 3, 2, 4, 2, 1, 5, 8, 10, 8, 10, 8, 56431, 8, 10, 8, 56431, 8, 9, 8, 14, 8, 1, 1, 3, 4, 2, 56431, 3, 56431, 4, 56431, 3, 56431, 14, 4, 56431, 252, 6, 6, 56431, 1, 2, 3, 2, 252, 252, 1, 4, 252, 6, 3, 4, 3, 14, 2, 516, 252, 252, 4, 2, 2, 8, 4, 4, 2, 324, 1, 2, 324, 3, 9, 4, 324, 324, 503, 2, 8, 3, 7, 9, 2, 1, 1, 2, 2, 546, 555, 555, 2, 8, 1, 546, 3, 4, 555, 3, 1, 4, 2, 1, 4, 1, 624, 2, 223, 3, 555, 3, 4, 4, 2, 1, 3, 575, 3, 4, 1, 546, 2, 546, 3, 575, 4, 1, 2, 2, 546, 1, 575, 4, 523, 304, 2, 304, 2, 1, 543, 3, 569, 3, 1302, 1302, 3, 7, 16, 4, 579, 279, 4, 2, 1, 2, 3, 2, 3, 4, 16, 1, 579, 279, 3, 4, 1, 3, 1, 1, 579, 16, 4, 7, 606, 2, 1, 1206, 1206, 4, 1, 1337, 2, 1337, 3, 3, 4, 4, 1, 1337, 1206, 4, 4, 1, 1337, 1, 1206, 3, 1, 3, 1, 1334, 1334, 2, 1206, 565, 1, 4, 7, 8, 1344, 1206, 3, 1206, 1, 9, 4, 2, 529, 3, 229, 3, 229, 1, 4, 2, 529, 3, 229, 4, 4, 1, 529, 2, 529, 3, 229, 229, 4, 529, 2, 516, 1, 3, 229, 4, 516, 2, 516, 229, 229, 2, 1, 516, 1, 2, 229, 3, 1, 4, 1, 4, 2, 1357, 229, 3, 1, 4, 1, 2, 1357, 3, 229, 3, 4, 4, 2, 229, 3, 229, 4, 4, 1, 529, 3, 229, 3, 229, 1, 7, 2, 529, 2, 634, 4, 1, 595, 2, 595, 2, 634, 3, 1, 4, 2, 595, 1, 3, 634, 4, 1, 4, 2, 595, 595, 1, 573, 1, 573, 2, 595, 3, 4, 1, 4, 2, 2, 595, 634, 4, 1, 4, 595, 595, 2, 4, 1, 4, 595, 2, 595, 2, 634, 3, 2, 1, 595, 1, 634, 3, 634, 4, 1, 4, 2, 3, 634, 4, 1, 595, 2, 595, 3, 634, 1, 1, 7, 2, 1300, 3, 634, 4, 1, 1300, 2, 1300, 634, 3, 634, 4, 1, 2, 2, 634, 3, 1, 595, 595, 2, 634, 3, 634, 4, 1, 2, 595, 634, 3, 634, 1, 4, 4, 566, 2, 629, 629, 3, 4, 4, 2, 566, 1, 566, 629, 3, 629, 1, 4, 2, 566, 3, 629, 3, 4, 1, 4, 1, 566, 566, 3, 3, 629, 4, 4, 4, 2, 1319, 2, 3, 629, 1, 4, 2, 1319, 3, 629, 3, 1, 1, 1319, 2, 2, 629, 3, 1, 4, 566, 2, 566, 3, 629, 4, 1, 2, 2, 566, 3, 3, 629, 4, 4, 1, 566, 2, 3, 629, 3, 629, 1, 4, 1, 566, 3, 629, 3, 1, 4, 1, 568, 2, 1346, 1346, 4, 1, 4, 2, 520, 2, 1327, 3, 1327, 1, 4, 4, 2, 520, 1327, 1327, 3, 1, 1, 2, 520, 3, 1327, 3, 1, 1, 2, 2, 582, 3, 1327, 4, 1, 582, 2, 582, 3, 1327, 1327, 1, 1, 531, 531, 1327, 3, 1230, 4, 630, 1230, 3, 3, 4, 1, 589, 2, 1230, 3, 1230, 1, 9, 1, 7, 9, 3, 8, 1, 9, 2, 559, 2, 3, 7, 2, 1, 4, 2, 519, 2, 4, 1315, 1, 1, 4, 1, 539, 1306, 3, 620, 4, 1, 9, 2, 586, 286, 4, 1, 3, 2, 586, 3, 1336, 3, 4, 593, 2, 545};


//bool to control whether we are using a route or not
#define DONT_RUN_ROUTE FALSE

// init our variables before running your move
int prevLockedDoor = -1;
int prevLock = -2;
int prevKey = 0;
int keyIndex = 0;
int prevDir = 2;
char prevRoom[5] = "";
int prevOpt = 0;
int roomCounter = 0;
int randOpt = 0;

// the part that makes this a bot :)
void yourMove(FILE* fd, FILE* keylog, FILE* keyP, FILE* optP)
{
	if (strcmp(room_name, room.name) != 0) option_count = 0;

	int optNum;
	int keyNum;
	int key;

	bool dontMove = false;

	if(counter >= (sizeof(optPath)/sizeof(int)))// && DONT_RUN_ROUTE )
	{
		chkUnlockedDoors(room,prevKey,prevOpt,prevLock);

		if (strcmp(room.name, prevRoom)!=0)
		{
			logDestRoomId(room,prevOpt,prevRoom);		
		}

		if (roomIsntLogged(room))
		{
			logRoom(room);

			sprintf(roomLog[roomCounter].roomName, "%s", room.name);

			for (int i = 0; i < 6; i++)
			{
				roomLog[roomCounter].directions[i] = room.direction[i];
			}
			roomCounter++;
		}


		if(student.doors_openned > tmpUnlockCnt)
		{	
			printf("prevkey:0x%04x prevdir:%d prevOpt:%d\n",prevKey, prevOpt - 7, prevOpt);
			//getchar();
		}

		tmpUnlockCnt = student.doors_openned;
		keyIndex = 0;
		//int seed = time(NULL);
		int seed = clock();
		srand(seed);
	
		randOpt = rand() % getNumOfDirectionsMove(room)-1; //breaks item pickup???
		optNum = options[randOpt];
		key = keys[rand()%(sizeof(keys)/sizeof(int))];
		//key = pop();
		//key = getKeyFromJSON(room,optNum,j);
		//key = getKeyFromWorld(room, optNum,fd);

		printf("number of items:%d\n", room.number_of_items);
		printf("number of dir:%d\n", randOpt);
		printf("option picked:%d\n", optNum);
		printf("key picked:0x%x\n", key);
		printf("dir picked:%d\n", optToDir(room,randOpt));

		int valuedItemsInRoom = cntValuedItems(room);


			if (thereIsDOS(room))
			{
				for (int i = 0; i < number_of_options; i++)
				{
					if (options[i] >= OPTION_BASE_FOR_DOS && options[i] < OPTION_BASE_FOR_EVENTS)
					{
						optNum = options[i];
					}
				}
				dontMove = false;

			}


		
				
			else if (thereIsEvent(room))
			{
				for (int i = 0; i < number_of_options; i++)
				{
					if (options[i] >= OPTION_BASE_FOR_EVENTS)
					{
						optNum = options[i];
					}
				}
				dontMove = false;
			}
				

			else if(valuedItemsInRoom > 0 && doWeNeedToReadItem(room))
			{
				printf("something to read\n");
				for (int i = 0; i < number_of_options; i++)
				{
				//	printf("option %d: %d  ",i, options[i]);
					if (options[i] >= OPTION_BASE_FOR_READS && options[i] < OPTION_BASE_FOR_PICKUPS)
					{
						//fprintf(fd, "4: option chose:%d, key chosen:%x , current room:%s, Backpack:%d, score:%d\n", options[i], 0, room.name, backpack.number_of_items, student.score);
						//sentOption(options[i], 0);
						optNum = options[i];
					}
				}
				printf("\n");
				dontMove = false;
			}

				
			else if(valuedItemsInRoom > 0 && doWeNeedToPickUpValuedItem(room) && backpack.number_of_items <= MAX_ITEMS_IN_BACKPACK)
			{
				printf("in first if\n");
				printf("number of valued items:%d\n", valuedItemsInRoom);
				printf("number of items:%d\n", room.number_of_items);


				// options 1 2 3 4 5 6 7 8 9
				// moves   1 2 3 4
				// items		   5   7		// for number of options above dir if pickup
				// read				 6   8			
				//system("timeout /t 60");
				//exit(1);
				for (int i = 0; i < room.number_of_items; i++)
				{
					if (room.items[i].value > 0 && weNeedItem(room, i))
					{
						int itemOpt2 = getNumOfDirectionsMove(room)-1 + i;

						if (options[itemOpt2] >= OPTION_BASE_FOR_PICKUPS && options[itemOpt2] < OPTION_BASE_FOR_DROPS)
						{
							int optCount = number_of_options - getNumOfDirectionsMove(room);

							fprintf(fd, "3: option:%d, option chose:%d, key chosen:%x , current room:%s, Backpack:%d, score:%d backpack = ", itemOpt2, options[itemOpt2], 0, room.name, backpack.number_of_items, student.score);
							for (int e = 0; e < backpack.number_of_items; e++)
							{
								fprintf(fd," %d", backpack.items[e]);
							}
							fprintf(fd,"\n");

							fprintf(fd, "3-2: options = ");
							for (int d = 0; d < number_of_options; d++)
							{
								fprintf(fd," %d", options[d]);
							}
							fprintf(fd,"\n");
							optNum = options[itemOpt2];
							//sentOption(options[itemOpt2], key);
							//option_count = (option_count + 1) % number_of_options;
							//return;
							//optNum = itemOpt;
							//printf("item is:%s\n", optNum);
							//system("timeout /t 20");
						}
					}
				}

				for (int i = 0; i < room.number_of_items; i++)
				{
					//fprintf(fd, "Item=%d, Value=%d, Volume=%d\n", room.items[i].number, room.items[i].value, room.items[i].volume);
				}
				dontMove = false;
			}
			


			else if (isLockedRoom(room))
			{
				optNum = firstLockedDoor(room);
				key = getKeyFromWorld(room,optNum,fd);
				//key = keys[rand()%(sizeof(keys)/sizeof(int))];

				printf("optnum:%d\n",optNum);
				printf("key:0x%04x\n",key);
				//getchar();
				while(key == 0 || optNum == prevOpt)
				{
					int seed = clock();
					srand(seed);
					randOpt = rand() % getNumOfDirectionsMove(room)-1; //breaks item pickup???
					optNum = options[randOpt];
					//key = keys[rand()%(sizeof(keys)/sizeof(int))];

					key = getKeyFromWorld(room, optNum,fd);
				}
			}
			else
			{
				//fprintf(fd, "1: move:%d option:%d, option chose:%d, key chosen:%x , current room:%s, Backpack:%d, score:%d\n", student.number_of_moves, optNum, options[optNum], key, room.name, backpack.number_of_items, student.score);		
				logDirection(room,optNum);

				bool daway = dontGoThatWay(room, optNum);
				while (daway)
				{
					int seed = clock();
					srand(seed);
					optNum = options[rand()%getNumOfDirectionsMove(room)];
					daway = dontGoThatWay(room, optNum);
					//key = getKeyFromWorld(room,optNum,fd);
					key = keys[rand()%(sizeof(keys)/sizeof(int))];

					if (daway)break;
					//dontMove = true;
				}

				dontMove = false;
			}
	}

	else
	{

		//chkUnlockedDoors(room,prevKey,prevOpt,prevLock);

		optNum = optPath[counter];
		key = keyPath[counter];
		//if(optNum < 11)logDirection(room,optNum);

		//fprintf(fd, "0: option:%d, option chose:%d, key chosen:%x , current room:%s, Backpack:%d, score:%d backpack = ", optPath[counter], options[optNum], key, room.name, backpack.number_of_items, student.score);
		fprintf(fd,"0:counter:%d option:%d key:0x%x room:%s score:%d\n",counter,prevOpt, prevKey, room_name, student.score);

		printf("optnum:%d keynum:%d\n", optNum, key);
	}

	/*
	if (strcmp(room_name, room.name) != 0)
	{
		int dir = optToDir(room,prevOpt);
		if (dir > 0 && dir < 5)
		{
			if (room.direction[dir] != prevDir)
			{
				fprintf(fd,"1: option:%d key:0x%x room:%s score:%d\n",prevOpt, prevKey, prevRoom, student.score);
			}
			else
			{
				fprintf(fd,"2: option:%d key:0x%x room:%s score:%d\n",prevOpt, prevKey, prevRoom, student.score);
			}
		}
	}
*/

	fprintf(fd,"1: option:%d key:0x%x room:%s score:%d\n",optNum, key, room.name, student.score);

	strcpy(room_name, room.name);
	prevOpt = optNum;
	prevKey = key;
	prevLock = room.direction[optToDir(room,optNum)];
	prevDir = room.direction[optToDir(room,optNum)];

	strcpy(prevRoom, room.name);

	fprintf(optP,"%d, ",optNum);
	fprintf(keyP,"0x%04x, ",key);
	//getchar();

		// conditions for buggy rooms with staircases


	if(!dontMove)
	{
		 sentOption(optNum, key); 
	}

	counter++;

	option_count = (option_count + 1) % number_of_options;
}


/*************************************************************/
/********* Your tactics code ends here ***********************/
/*************************************************************/

int getTokens(char* instring, char seperator)
{
	int  number_of_tokens;
	char chr;
	int  state;
	int  i;
	int  j;


	for (i = 0; i < MAX_NO_TOKENS; i++)
	{
		for (j = 0; j < MAX_STRING_SIZE; j++)
		{
			Tokens[i][j] = '\0';
		}
	}

	number_of_tokens = -1;
	chr = instring[0];
	state = 0;
	i = 0;

	while (chr != '\0')
	{
		switch (state)
		{
		case 0:  // Initial state
			if (chr == seperator)
			{
				number_of_tokens++;
				state = 1;
			}
			else if ((chr == ' ') || (chr == '\t') || (chr == '\n'))
			{
				state = 1;
			}
			else
			{
				number_of_tokens++;
				j = 0;
				Tokens[number_of_tokens][j] = chr;
				Tokens[number_of_tokens][j + 1] = '\0';
				state = 2;
			}
			break;

		case 1:  // Leading white space
			if (chr == seperator)
			{
				number_of_tokens++;
				state = 1;
			}
			else if ((chr == ' ') || (chr == '\t') || (chr == '\n'))
			{
				state = 1;
			}
			else
			{
				number_of_tokens++;
				j = 0;
				Tokens[number_of_tokens][j] = chr;
				Tokens[number_of_tokens][j + 1] = '\0';
				state = 2;
			}
			break;

		case 2:  // Collecting chars
			if (chr == seperator)
			{
				//number_of_tokens++;
				state = 1;
			}
			else
			{
				j++;
				Tokens[number_of_tokens][j] = chr;
				Tokens[number_of_tokens][j + 1] = '\0';
				state = 2;
			}
			break;

		default:
			break;
		}

		i++;
		chr = instring[i];
	}

	return (number_of_tokens + 1);
}



bool getline(FILE *fp, char *buffer)
{
	bool rc;
	bool collect;
	char c;
	int  i;

	rc = false;
	collect = true;

	i = 0;
	while (collect)
	{
		c = getc(fp);

		switch (c)
		{
		case EOF:
			if (i > 0)
			{
				rc = true;
			}
			collect = false;
			break;

		case '\n':
			if (i > 0)
			{
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



void printRoom()
{
	int i;

	printf("Room\n");
	printf("Room = %s, Room type = %d\n", room.name, room.type);

	printf("Directions = ");
	for (i = 0; i < 6; i++)
	{
		printf("%d  ", room.direction[i]);
	}
	printf("\n");

	if (room.number_of_keys > 0)
	{
		printf("Keys = ");
		for (i = 0; i < room.number_of_keys; i++)
		{
			printf("0x%X  ", room.keys[i]);
		}
		printf("\n");
	}
	else
	{
		printf("No keys in this room\n");
	}

	if (room.number_of_items > 0)
	{
		for (i = 0; i < room.number_of_items; i++)
		{
			printf("Item=%d, Value=%d, Volume=%d\n", room.items[i].number, room.items[i].value, room.items[i].volume);
		}
	}
	else
	{
		printf("No items in this room\n");
	}

	printf("\n");
}


void printStudent()
{
	printf("Student\n");
	printf("Level=%d,  Number of rooms visited = %d,  Number of doors openned = %d,  Number of moves = %d,  Score = %d\n", student.level, student.rooms_visited, student.doors_openned, student.number_of_moves, student.score);
	printf("\n");
}


void printBackpack()
{
	int i;

	printf("Backpack\n");

	if (backpack.number_of_items > 0)
	{
		for (i = 0; i < backpack.number_of_items; i++)
		{
			printf("Item=%d, Value=%d, Volume=%d\n", backpack.items[i].number, backpack.items[i].value, backpack.items[i].volume);
		}
	}
	else
	{
		printf("Your backpack is empty\n");
	}
	printf("\n");
}


void printOptions()
{
	int i;

	printf("Options\n");
	printf("Options = ");
	for (i = 0; i < number_of_options; i++)
	{
		printf("%d  ", options[i]);
	}
	printf("\n");
	printf("\n");
}


void communicate_with_server(FILE* fd, FILE* keylog,FILE* keyP, FILE* optP)
{
	char buffer[4096];
	int  len = sizeof(SOCKADDR);
	int  i;
	char* p;
	int	 number_of_tokens;


	sprintf_s(buffer, "Register  %s %s %s", STUDENT_NUMBER, STUDENT_FIRSTNAME, STUDENT_FAMILYNAME);
	sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *)&server_addr, sizeof(SOCKADDR));

	while (true)
	{
		memset(buffer, '\0', sizeof(buffer));

		if (recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (SOCKADDR *)&client_addr, &len) != SOCKET_ERROR)
		{
			p = ::inet_ntoa(client_addr.sin_addr);

			if ((strcmp(IP_ADDRESS_SERVER, "127.0.0.1") == 0) || (strcmp(IP_ADDRESS_SERVER, p) == 0))
			{
				printf("%s\n\n", buffer);

				number_of_tokens = getTokens(buffer, '|');

				if (number_of_tokens == 6)
				{
					strcpy(text_student, Tokens[0]);
					strcpy(text_backpack, Tokens[1]);
					strcpy(text_room, Tokens[2]);
					strcpy(text_keys, Tokens[3]);
					strcpy(text_items, Tokens[4]);
					strcpy(text_options, Tokens[5]);

					printf("Student  = '%s'\n", text_student);
					printf("Backpack = '%s'\n", text_backpack);
					printf("Room     = '%s'\n", text_room);
					printf("Keys     = '%s'\n", text_keys);
					printf("Items    = '%s'\n", text_items);
					printf("Options  = '%s'\n", text_options);

					student.level = -1;
					student.rooms_visited = -1;
					student.doors_openned = -1;
					student.number_of_moves = -1;

					if (sscanf(text_student, "%d,%d,%d,%d,%d", &student.level, &student.rooms_visited, &student.doors_openned, &student.number_of_moves, &student.score) == 5)
					{
					}

					if (strlen(text_backpack) > 0)
					{
						backpack.number_of_items = getTokens(text_backpack, '&');

						if (backpack.number_of_items > 0)
						{
							for (i = 0; i < backpack.number_of_items; i++)
							{
								if (i < MAX_ITEMS_IN_BACKPACK)
								{
									backpack.items[i].number = -1;

									if (sscanf(Tokens[i], "%d, %d, %d", &backpack.items[i].number, &backpack.items[i].value, &backpack.items[i].volume) == 3)
									{
									}
								}
							}
						}
					}
					else
					{
						backpack.number_of_items = 0;
					}

					sscanf(text_room, "%s ,%d, %d, %d, %d, %d, %d, %d", &room.name, &room.type, &room.direction[DIRECTION_NORTH], &room.direction[DIRECTION_SOUTH], &room.direction[DIRECTION_EAST], &room.direction[DIRECTION_WEST], &room.direction[DIRECTION_UP], &room.direction[DIRECTION_DOWN]);

					if (strlen(text_keys) > 0)
					{
						room.number_of_keys = getTokens(text_keys, '&');

						if (room.number_of_keys > 0)
						{
							for (i = 0; i < room.number_of_keys; i++)
							{
								if (i < 4)
								{
									room.keys[i] = -1;

									if (sscanf(Tokens[i], "%x", &room.keys[i]) == 1)
									{
									}
								}
							}
						}
					}
					else
					{
						room.number_of_keys = 0;
					}

					if (strlen(text_items) > 0)
					{
						room.number_of_items = getTokens(text_items, '&');

						if (room.number_of_items > 0)
						{
							for (i = 0; i < room.number_of_items; i++)
							{
								if (i < MAX_ITEMS_IN_ROOM)
								{
									room.items[i].number = -1;

									if (sscanf(Tokens[i], "%d, %d, %d", &room.items[i].number, &room.items[i].value, &room.items[i].volume) == 3)
									{
									}
								}
							}
						}
					}
					else
					{
						room.number_of_items = 0;
					}

					if (strlen(text_options) > 0)
					{
						number_of_options = getTokens(text_options, ',');

						if (number_of_options > 0)
						{
							for (i = 0; i < number_of_options; i++)
							{
								if (i < MAX_OPTIONS)
								{
									options[i] = -1;

									if (sscanf(Tokens[i], "%d", &options[i]) == 1)
									{
									}
								}
							}
						}
					}
					else
					{
						number_of_options = 0;
					}
				}

				printStudent();
				printBackpack();
				printRoom();
				printOptions();

				//system("timeout /t 60");

				if (student.number_of_moves >= 2000 || student.score > 60)
				{

					char buffer[256];
					FILE* fd2 = fopen("scores.txt", "a");
					fprintf(fd2,"final score:%d\n",student.score);
					fclose(fd2);

					char buffer2[256];
					FILE* fd3 = fopen("roomCnt.txt", "a");
					fprintf(fd3,"total rooms:%d doors opened:%d\n",student.rooms_visited, student.doors_openned);
					fclose(fd3);

					break;
				}
					yourMove(fd,keylog,keyP,optP);

			}
		}
		else
		{
			printf_s("recvfrom error = %d\n", WSAGetLastError());
		}
	}

	printf_s("Student %s\n", STUDENT_NUMBER);
}




int main()
{
	char chr = '\0';

	printf("\n");
	printf("The Computer Maze Student Program\n");
	printf("UWE Computer and Network Systems Assignment 2 \n");
	printf("\n");

	if (WSAStartup(MAKEWORD(2, 2), &Data) != 0) return(0);

	//sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // Here we create our socket, which will be a UDP socket (SOCK_DGRAM).
	//if (!sock)
	//{	
	//	printf("Socket creation failed!\n"); 
	//}

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // Here we create our socket, which will be a UDP socket (SOCK_DGRAM).
	if (!sock) 
	{
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


	FILE* fd = fopen("test.txt", "w+");
	FILE* keylog = fopen("keylog.txt", "w+");
	FILE* keyP = fopen("keypath.txt", "w+");
	FILE* optP = fopen("optpath.txt", "w+");

	// load the json file and then parse it with the json library https://github.com/nlohmann/json
	std::ifstream i("world.json");
	json j = json::parse(i);

	char* dirArray[6] = {"north","south","east","west","up","down"}; 

	for (int i = 0; i < 500; i++)
	{
		char roomName[5];

		strcpy(roomName,roomname[i]);
		strcpy(worldConfig[i].roomName,roomname[i]);		

		//printf("roomName: %s\n",roomName);
		//printf("worldConfig roomname: %s\n",worldConfig[i].roomName);

		std::string room = roomName;
		
		for (int dir = 0; dir < 6; dir++)
		{
			std::string d = dirArray[dir];

			std::string parsedRoomId = j["rooms"][room]["directions"][d]["destinationRoomId"];
			bool parsedLockStatus = j["rooms"][room]["directions"][d]["isLocked"];
			bool parsedLockStatusPrev = j["rooms"][room]["directions"][d]["isLockedInitially"];
			bool parsedDirStatus = j["rooms"][room]["directions"][d]["isPresent"];
			std::string parsedKey = j["rooms"][room]["directions"][d]["pairedKey"];

			// convert a string representing a hex key to an integer
			char* p;
			int key = strtol( parsedKey.c_str(), & p, 16 );
		
			worldConfig[i].keys[dir] = key;		
			worldConfig[i].lockStatusPrev[dir] = parsedLockStatusPrev;		
			worldConfig[i].lockStatus[dir] = parsedLockStatus;		
			worldConfig[i].dirStatus[dir] = parsedDirStatus;		
			
			strcpy(worldConfig[i].adjacentRooms[dir],parsedRoomId.c_str());

		}

	}


	communicate_with_server(fd,keylog,keyP,optP);

 	fclose(fd);
 	fclose(keylog);
 	fclose(keyP);
 	fclose(optP);

	// results file
	FILE* fd2 = fopen("test.txt", "r");
    char ch, target_file[20];
    FILE *target;
 
	sprintf(target_file, "%d.txt", student.score);
    target = fopen(target_file, "w+");
 
    while( ( ch = fgetc(fd2) ) != EOF )
      fputc(ch, target);
 
 	fclose(fd2);
    fclose(target);

	// path files
	// key path
	FILE* fd3 = fopen("keypath.txt", "r");
    char ch2, target_file2[20];
    FILE *target2;
 
	sprintf(target_file2, "%d-keypath.txt", student.score);
    target2 = fopen(target_file2, "w+");
 
    while( ( ch2 = fgetc(fd3) ) != EOF )
      fputc(ch2, target2);
 
 	fclose(fd3);
    fclose(target2);

	// opt path
	FILE* fd4 = fopen("optpath.txt", "r");
    char ch3, target_file3[20];
    FILE *target3;
 
	sprintf(target_file3, "%d-optPath.txt", student.score);
    target3 = fopen(target_file3, "w+");
 
    while( ( ch3 = fgetc(fd4) ) != EOF )
      fputc(ch3, target3);
 
 	fclose(fd4);
    fclose(target3);

	FILE* rc = fopen("roomConfig.txt", "w+");

	for (int i = 0; i < 500; i++)
	{	
		fprintf(rc,"room:%s North:%x\t\tEast:%x\t\tSouth:%x\t\tWest:%x\t\tUp:%x\t\tDown:%x\n", worldConfig[i].roomName, worldConfig[i].keys[0], worldConfig[i].keys[1], worldConfig[i].keys[2], worldConfig[i].keys[3], worldConfig[i].keys[4], worldConfig[i].keys[5]);
	}

	fclose(rc);

	closesocket(sock);
	WSACleanup();


	//printRooms();
	logWorldJSON();

	
	while (chr != '\n')
	{
		chr = getchar();
	}
	
	return 0;
}

