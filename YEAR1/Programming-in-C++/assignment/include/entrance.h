/*****************************************************************************
*																			 *
* File: entrance.h							     							 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: entrance class header						 			     		 *
* Copyright: University of West of England 2017				    			 *
*									     									 *
******************************************************************************/


#pragma once

#include <zombies/WorldLoader.h>
#include <ufcfgl-30-1.h>


class Entrance {

	private:

		// variables we want to protect from the user
		std::string roomName;
		std::string dir;
		bool lock;

	public:

		// class constructors
		Entrance();
		Entrance(uwe::EntranceInfo entInfo);

		// movement methods
		std::string getDir();
		std::string getRoom();

		// lock control methods
		bool checkLock();
		void unlock();

};
