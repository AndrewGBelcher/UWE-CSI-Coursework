/*****************************************************************************
*																			 *
* File: player.cpp							     							 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: player class 								 			     		 *
* Copyright: University of West of England 2017				    			 *
*									     									 *
******************************************************************************/

// our custom headers
#include "player.h"


// construct the player and set its inventories max item limit
Player::Player(){
	maxItems = 3;
	invincible = false;
	roomCnt = 0;

}

// method to return the pointer to the room we are in
Room* Player::getCurRoom(){
	return curRoom;
}

// method to set the current room to what room pointer we pass it
void Player::setCurRoom (Room* room) {
	curRoom = room;
}

// method to add and item to our inventory
bool Player::pickupItem (Item item){

	if (inventory.size() >= maxItems){
		return false;
	}

	else{
		item.setHeld();
		inventory.push_back (item);
		return true;	
	}

}


// method to remove and item from our inventory and place in the world
bool Player::dropItem (std::string item){

	int i = 0;
	for (Item& ourItem : inventory){

		if (ourItem.getName() == item){
			Item droppedItem = ourItem;

			getCurRoom()->putItemInRoom (droppedItem);
			inventory.erase (inventory.begin() + i);

			return true;
		}
		
		i++;	

	}

	return false;

}

// method to delete an item from the game ie* weapons and keys
bool Player::delItem (std::string item){

	int i = 0;
	for (Item& ourItem : inventory){

			if (ourItem.getName() == item){
				inventory.erase (inventory.begin() + i);
				
				return true;
			}

		i++;

	}

	return false;
}


// method to clear items from the players inventory
void Player::clearItems(){

	int i = 0;

	for (Item& ourItem : inventory){
		inventory.erase (inventory.begin() + i);		
		i++;
	}

}

// method to check if our inventory contains an item
bool Player::chkItem (std::string item){

	for (Item& ourItem : inventory){

		if (ourItem.getName() == item){
			return true;
		}

	}

	return false;
}

// method to return our inventories html representation
std::string Player::getInventoryHtml(){

	std::string invHtml = "";

	for (Item& item : inventory) {
		invHtml += item.getHtml();
	}

	return invHtml;
}

bool Player::notInvincible(){

	if (invincible){
		invincible = false;
		return true;
	}
	
	return false;

}

bool Player::setInvincibility(){

	if (invincible){
		invincible = false;
		return false;
	}
	
	invincible = true;
	return true;

}

bool Player::chkInvincibility(){

	return invincible;

}

int Player::chkRoomCnt(){

	return roomCnt;
}

void Player::incRoomCnt(){

	roomCnt++;
}
