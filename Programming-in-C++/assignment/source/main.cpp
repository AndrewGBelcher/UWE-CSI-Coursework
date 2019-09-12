/*****************************************************************************
*																			 *
* File: main.cpp							     							 *
* Author: Andrew Belcher						     						 *
* Date: 12-11-2017							     							 *
* Desc: ZOMBIES	assignment main program				 			     		 *
* Copyright: University of West of England 2017				    			 *
*									     									 *
******************************************************************************/

// uwe headers
#include <ufcfgl-30-1.h>

// zombie assignment lib headers
#include <zombies/WorldLoader.h>
#include <zombies/ZombieServer.h>

// our custom headers
#include "ZombieBot.h"
#include "world.h"

int main(void){

	// load the world from json and stor in a pointer
    uwe::WorldLoader wl ( uwe::readFile ("world.json") );

	// pass in the json string into our world class
	World world = World{wl};

	// create the zombie bot to use this world
	::ZombieBot zomb{world};

	// start the zombie server with our zombie bot and desired port
    uwe::ZombieServer zserver (8085, zomb);

	// start the server
    zserver.run();


	return 0;
}
