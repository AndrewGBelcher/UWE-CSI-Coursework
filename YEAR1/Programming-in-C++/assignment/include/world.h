/*****************************************************************************
*																			 *
* File: world.h								     							 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: world class header							 			     		 *
* Copyright: University of West of England 2017				    			 *
*									     									 *
******************************************************************************/

#pragma once

#include <ufcfgl-30-1.h>
#include <zombies/WorldLoader.h>

#include "room.h"
#include "item.h"
#include "player.h"


class World{

	private:

		// protect our variables from the user
		std::vector<Room>rooms;
		std::vector<Item>items;

		std::string info;
		std::string start;
		std::string end;
		std::string inventoryHtml;

		int score;
		bool exit;

	public:

		// our class constructors
		World();
		World (uwe::WorldLoader wl);

		// our pointer to the player class, used for changing rooms
		Player player;

		// world info methods
		std::string getInfo();
		std::string getRoomInfo();
		std::string getInventoryHtml();
	
		// methods to load the world
		void loadRooms (std::vector<uwe::RoomInfo>roomsJSON);
		void loadItems (std::vector<uwe::ItemInfo>itemsJSON);

		// methods to measure our position in the world
		Room* getTargetRoom (Room* curRoom, std::string direction, std::string* dirCheckRet);
		void setBegin();
		Room* getStart();
		Room* getEnd();

		// game score methods
		int getGameScore();
		void scored (int amount);

		// our zombie timer varibles
		bool startTimer;
		bool endTimer;

		// game over methods
		void quit();
		bool checkQuit();

};
