/*****************************************************************************
*																			 *
* File: ZombieBot.cpp						     							 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: zombiebot class								 			     		 *
* Copyright: University of West of England 2017				    			 *
*									     									 *
******************************************************************************/


#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <utility>

#include <ufcfgl-30-1.h>

#include "ZombieBot.h"

using namespace std;


ZombieBot::ZombieBot (World w) {
	world = w;
	world.setBegin();
}


bool ZombieBot::shouldQuit() {

	if (world.checkQuit()){
		return true;
	}

	else{
		return false;
	}    

}

std::string ZombieBot::begin() {
    return "begin";
}

int ZombieBot::currentScore() {
    return world.getGameScore();
}

bool ZombieBot::enableTimer() {

	if (world.startTimer){
		world.startTimer = false;
		return true;
	}

	else{
		return false;		
	}
    
}

bool ZombieBot::disableTimer() {

	if (world.endTimer){
		world.endTimer = false;
		return true;
	}

	else{
		return false;		
	}

}



std::vector<std::string>ZombieBot::processCmd (std::string cmd) {

    std::vector<std::string>result;

	std::string listOfCMDS = "List of available commands are.. <b> info </b><b> look </b><b> move </b><b> inventory </b><b> pickup </b><b> drop </b><b> kill </b><b> quit </b><b> unlock </b><b> combine </b>";
	
    // split input in to a sequence of commands
    istringstream iss (cmd);

    vector<string>commands{istream_iterator<string>{iss}, 
							istream_iterator<string>{}};

	transform (commands[0].begin(), commands[0].end(), commands[0].begin(), ::tolower);

    switch ( uwe::str2int (commands[0].c_str()) ) {

		case uwe::str2int ("begin"):
			result.push_back (world.getInfo());
		    result.push_back ("... you have just woken up next to what appears to be a "+ world.getStart()->getName());
		    result.push_back (listOfCMDS);
			break;


		case uwe::str2int ("info"):
		    result.push_back (world.getInfo());
		    result.push_back (listOfCMDS);
		    break;


		case uwe::str2int ("look"):
		    result.push_back (world.player.getCurRoom()->getRoomInfo());
		    break;


		case uwe::str2int ("move"):

		    if (commands.size() < 2) {
		        result.push_back ("you must specify a direction to move");
		    }

		    else {

				transform (commands[1].begin(), commands[1].end(), commands[1].begin(), ::toupper);

				std::string dirCheckRet;

				Room* targetRoom = world.getTargetRoom (world.player.getCurRoom(), commands[1], &dirCheckRet);
		
				if (dirCheckRet == "valid direction"){

					if (targetRoom == world.getEnd()){
						result.push_back ("you just made it out of the building unharmed!");
						world.player.setCurRoom (targetRoom);
						
						result.push_back (world.player.getCurRoom()->getRoomInfo());

						if(world.player.getCurRoom()->chkZombies() > 0){
							world.startTimer = true;
						}
					}

					else{
						
						if(world.player.chkRoomCnt() > 1){
							world.player.notInvincible();
						}

						world.player.setCurRoom (targetRoom);
						result.push_back (world.player.getCurRoom()->getRoomInfo());

						if(world.player.getCurRoom()->chkZombies() > 0){
							world.startTimer = true;
						}

					}

				}
	
				else if (dirCheckRet == "invalid direction"){
					result.push_back ("we cant go that way");
				}

				else if (dirCheckRet == "direction locked"){
					result.push_back ("door is locked, you need a key");
				}
			
			}

		    break;

		case uwe::str2int ("unlock"):

		    if (commands.size() < 2) {
		        result.push_back ("you must specify an entrance to unlock");
		    }

			else{

				transform (commands[1].begin(), commands[1].end(), commands[1].begin(), ::toupper);

				for (Entrance& ent : world.player.getCurRoom()->getEnts()){
				
					if (ent.getDir() == commands[1]){

						if (ent.checkLock()){

							if (world.player.delItem ("KEY")){
								world.player.getCurRoom()->unlockEnt (commands[1]);
							    result.push_back ("you just unlocked the " + ent.getDir() + " entrance to" + ent.getRoom());
							}

							else{
								result.push_back ("you need a key to unlock");
							}

						}

						else{
							result.push_back ("this entrance is already unlocked");
						}

					}

				}		
		
			}

		    break;

		case uwe::str2int ("inventory"):

		    result.push_back (world.getInventoryHtml() + world.player.getInventoryHtml());
		    break;


		case uwe::str2int ("quit"):
			shouldQuit();
			world.setBegin();
		    result.push_back ("exiting game...");
		    break;


		case uwe::str2int ("pickup"):
		    if (commands.size() < 2) {
		        result.push_back ("you must specify an item to be picked up");
		    }

		    else{
				Item pickedItem;	        
				transform (commands[1].begin(), commands[1].end(), commands[1].begin(), ::toupper);
		
				if(world.player.getCurRoom()->getItem (commands[1], &pickedItem)){

					if(world.player.pickupItem (pickedItem)){
						if(pickedItem.chkHeld() == false){
							world.scored(2);				
						}
		        		result.push_back ("Picked up: " + pickedItem.getHtml());
					}
			
				}

				else{
					result.push_back ("Damn! there is no " + commands[1] + "in this room..");		
				}

			}

		    break;


		case uwe::str2int ("drop"):
		    if (commands.size() < 2) {
		        result.push_back ("you must specify an item to drop");
		    }

		    else {
				Item droppedItem;

				transform (commands[1].begin(), commands[1].end(), commands[1].begin(), ::toupper);

				if (world.player.dropItem (commands[1])){

					result.push_back ("you just dropped your: " + commands[1]);		
				}

				else{
					result.push_back ("you arent holding any: " + commands[1]);		
				}

		    }

		    break;

		case uwe::str2int ("combine"):
		    if (commands.size() < 3) {
		        result.push_back ("you need 2 items in order to combine them");
		    }

		    else {
				Item droppedItem;

				transform (commands[1].begin(), commands[1].end(), commands[1].begin(), ::toupper);
				transform (commands[2].begin(), commands[2].end(), commands[2].begin(), ::toupper);

				if ((commands[1] == "DAISY" || commands[2] == "DAISY") && world.player.delItem ("DAISY") && (commands[1] == "GOLD" || commands[2] == "GOLD") && world.player.delItem("GOLD")){
	
						world.player.delItem ("DAISY");
						world.player.delItem ("GOLD");
						
						// make serum?
						// invincibilty?

						if (world.player.setInvincibility()){

						result.push_back ("you have just created a serum that renders you undetectable to the zombies, however it will only work for a limited amoun of time, use this to move quicky to another room! ");	

						}
						
					} 

				

				else{
					result.push_back ("you arent holding any: " + commands[1]);		
				}

		    }

		    break;

		case uwe::str2int ("timerexpired"):

			if (world.player.chkInvincibility() == false){
				shouldQuit();
				world.setBegin();
				result.push_back ("<br> you have been devoured by zombies! <br>GAME OVER");
			}
	
		    break;


		case uwe::str2int ("kill"):
			if (world.player.getCurRoom()->chkZombies() > 0){

				if (world.player.delItem ("CHAINSAW") || world.player.delItem ("DAISY")){
					world.scored (25);
					world.player.getCurRoom()->killZombie();
				   	result.push_back ("ZOMBIE KILLED!");
				    world.endTimer = true;
				
					if(world.player.getCurRoom()->chkZombies() > 0){
						world.startTimer = true;
					}

					else{
						result.push_back ("Phew.. we killed them all!");
		
						if (world.player.getCurRoom()->chkZombies() == 0){
							shouldQuit();
							world.setBegin();
							result.push_back ("You made it our unscaved! But the nightmare has just begun..");
						}
			
					}

				}

				else{
						result.push_back ("RUN! you have no weapon!");		
				}

			}

			else{
					result.push_back ("There are no zombies in this room..");		
			}

		    break;


		default:
			result.push_back ("<b>That's not a verb I recognise.</b>");
			break;	
	}

	return result;

}
