/*****************************************************************************
*																			 *
* File: entrance.cpp						     							 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: entrance class 								 			     		 *
* Copyright: University of West of England 2017				    			 *
*									 										 *
******************************************************************************/

// our custom headers
#include "entrance.h"

// our default constructor
Entrance::Entrance(){}

// contruct an entrance and init its members form the json file
Entrance::Entrance (uwe::EntranceInfo info) {
	
	roomName = info.getTo();
	dir = info.getDirection();
	lock = info.isLocked();
}

// method to get the direction used for this entrance
std::string Entrance::getDir() {
	return dir;
}

// method to get the corresponding room that this entrance leads to
std::string Entrance::getRoom() {
	return roomName;
}

// method to unlock an entrance via setting its private lock member
void Entrance::unlock() {
	
	lock = false;
}

// method to return the boolean of the entrances lock
bool Entrance::checkLock() {

	return lock;
}


