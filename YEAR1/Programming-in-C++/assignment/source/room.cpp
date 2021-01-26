/*****************************************************************************
*																			 *
* File: room.cpp							     							 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: room class									 			     		 *
* Copyright: University of West of England 2017				    			 *
*									     									 *
******************************************************************************/

// our custom headers
#include "room.h"
#include "entrance.h"

// our default contructor
Room::Room(){}

// construct our room with its info and possible contained variables
Room::Room (uwe::RoomInfo ri, std::vector<Item>worldItems){

	roomName = ri.getName();
	roomDesc = ri.getDescription();
	zombCount = ri.getZombieCount();
	roomItems = ri.getItems();
	initItems (worldItems);
	loadEntrances (ri.getEntrances());

}

// method to return the worlds items
std::vector<Item>Room::getItems() {
    return itemVector;
}

// method to return the rooms name
std::string Room::getName() {
	return roomName;
}

// method to return a description of the room
std::string Room::getDesc() {
	return roomDesc;
}

// method to display html compatible room information
std::string Room::getRoomInfo() {

	return ("You are in room: <u>"
        + getName() + "</u><br>"
        + getDesc()
        + "<br>Items:<br>"
		+ getItemStrings() 
		+ "   <br>  "
        + getEntHTML());
}

// method to return the html of the items in this room
std::string Room::getItemStrings() {

	std::string iname;

	for (Item& item : itemVector) {
		iname += item.getHtml();
	}

	return iname;
}

// method to return the html of the entrances in this room
std::string Room::getEntHTML(){
	std::string entHtml = "The exits seem to be: <br>";

	for (Entrance& ent : entVect) {
		entHtml += "<br>" + ent.getDir() + "  to:  " + ent.getRoom() + " <br>";
	}

	return entHtml;
}

// method to check if there is an item in this room and return a pointer to this item
bool Room::getItem (std::string iName, Item* retItem) {

	for (int i=0; i<itemVector.size(); i++) {

		if (itemVector[i].getName() == iName) {
			*retItem = itemVector[i];
			itemVector.erase (itemVector.begin() + i);

            return true;
        }

    }

    return false;
}


// method that loads all the rooms items
void Room::initItems (std::vector<Item>worldItems) {

	for (std::string& roomItem : roomItems) {
	
		for (Item& item : worldItems) {
			
			if (item.getName() == roomItem) {
				itemVector.push_back (item);

			}

		}

	}

}


// method to load all the entrances for this room
void Room::loadEntrances (std::vector<uwe::EntranceInfo>entrancesJSON){

	for (uwe::EntranceInfo& entInfo : entrancesJSON) {
		entVect.push_back (Entrance(entInfo));
	}

}

// check this function later
// method to return a vector of this rooms entrances
std::vector<Entrance>Room::getEnts() {

	return entVect;
}


// method that places an item into the room
void Room::putItemInRoom (Item item){

	itemVector.push_back (item);
}

// method that unlocks an entrance via its unlock method
void Room::unlockEnt (std::string entName){

	for (Entrance& ent : entVect){

		if (ent.getDir() == entName){
			ent.unlock();
		}	
	
	}

}

// detect how many zombies are in this room
int Room::chkZombies(){

	return zombCount;
}

// eliminate a zombie from this room
void Room::killZombie(){

	zombCount -= 1;
}








