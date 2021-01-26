/*****************************************************************************
*																			 *
* File: world.cpp							     							 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: world class									 			     		 *
* Copyright: University of West of England 2017				    			 *
*									     									 *
******************************************************************************/

// our custom headers
#include "world.h"


// our default constructor
World::World(){}

// construct the world by passing it in the json file as a string
World::World (uwe::WorldLoader wl){

    startTimer = false;
    endTimer = false;
    exit = false;
    score = 0;

    info = wl.getInfo();
    start = wl.getStart();
    end = wl.getEnd();
    inventoryHtml = wl.getInventoryHtml();

    player = Player();

    loadItems (wl.getItems());
    loadRooms (wl.getRooms());
    
}

// method to return the world info string
std::string World::getInfo(){

    return info;
    
}

// method to get the pointer to the last room in the game
Room* World::getEnd(){

    for (Room& room : rooms) {

        if (room.getName() == end) {
            return &room;
        }

    }

	return NULL;
}

// method to get the room that the game starts in	
Room* World::getStart(){
    
	for (Room& room : rooms) {

        if (room.getName() == start) {
            return &room;
        }

    }

	return NULL;
}

// method to reinitilze the world when you finish the game
void World::setBegin() {
    player.setCurRoom (getStart());
	score = 0;
	
	player.clearItems();

}

// method to return our inventories html representation
std::string World::getInventoryHtml() {
    return inventoryHtml;
}

// method to create our item class via the json string
void World::loadItems (std::vector<uwe::ItemInfo>itemsJSON) {

    for (uwe::ItemInfo& item : itemsJSON) {
        items.push_back (Item (item.getName(), item.getHtml()));
    }

}

// method to create our world class via the json string
void World::loadRooms (std::vector<uwe::RoomInfo>roomsJSON) {
    for (uwe::RoomInfo& room : roomsJSON) {
		rooms.push_back (Room (room, items));
    }

}

// method to grab the pointer to the room we are moving into based on direction/current room
Room* World::getTargetRoom (Room* curRoom, std::string direction, std::string* dirCheckRet){

    for (Entrance& ent : curRoom->getEnts()){

        if (ent.getDir() == direction){

			if (ent.checkLock()){
		    	*dirCheckRet = "direction locked";
		        return NULL;
		    }

			for (Room& room : rooms){

		    	if (room.getName() == ent.getRoom()){
		        	*dirCheckRet = "valid direction";
					return &room;			
		        }
		    }
    	}
	}

    *dirCheckRet = "invalid direction";
    return NULL;
}

// returns the current score in the game
int World::getGameScore(){
    return score;
}

// sets the score based on the amount we give it
void World::scored (int amount){
    score += amount;
}

// returns our exit flag
bool World::checkQuit(){
    return exit;
}

// sets our exit flag
void World::quit(){
	exit = true;
}
