/*****************************************************************************
*																			 *
* File: player.h							     							 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: player class header							 			     		 *
* Copyright: University of West of England 2017				    			 *
*									     									 *
******************************************************************************/

#pragma once

#include "item.h"
#include "room.h"


class Player{

	private:

		// the variables we need to protect from the user
		std::vector<Item>inventory;
		int maxItems;
		bool invincible;
		int roomCnt;

		Item* itemEquip;
		Room* curRoom;

	public:

		// our class constructor
		Player();

		// inventory management methods
		bool pickupItem(Item item);
		bool dropItem(std::string item);
		bool chkItem(std::string item);
		bool delItem(std::string item);
		void clearItems();

		// methods for inventory information
		std::string getInventoryHtml();

		// player movement methods
		Room* getCurRoom();
		void setCurRoom(Room* room);

		// player invincibility methods
		bool setInvincibility();
		bool chkInvincibility();
		bool notInvincible();

		// room count methods
		void incRoomCnt();
		int chkRoomCnt();

};
