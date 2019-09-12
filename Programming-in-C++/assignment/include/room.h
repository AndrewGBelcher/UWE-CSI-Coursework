/*****************************************************************************
*																			 *
* File: room.h							     								 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: room class header							 			     		 *
* Copyright: University of West of England 2017				    			 *
*									     									 *
******************************************************************************/

#pragma once


#include "item.h"
#include "entrance.h"


class Room{

	private:

		// protect our variables from the user
		std::vector<std::string>roomItems;
		std::vector<Entrance>entVect;
		std::vector<Item>itemVector;

		std::string roomName;
		std::string roomDesc;

		int zombCount;

	public:

		// our class constructors
		Room();
		Room (uwe::RoomInfo ri, std::vector<Item>itemVector);

		// methods for getting room/item/ent information
		std::string getRoomInfo();
		std::string getItemStrings();
		std::string getEntHTML();
		std::string getName();
		std::string getDesc();

		// our item methods
		void initItems (std::vector<Item>worldItems);
		std::vector<Item>getItems();
		bool getItem (std::string iName, Item* retItem);
		void putItemInRoom (Item item);

		// our entrance methods
		std::vector<Entrance>getEnts();
		void unlockEnt (std::string entName);
		void loadEntrances (std::vector<uwe::EntranceInfo>entrancesJSON);

		// our zombie methods
		int chkZombies();
		void killZombie();

};
