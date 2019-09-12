//yo
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <ufcfgl-30-1.h>

// zombies includes
#include <zombies/WorldLoader.h>
#include <zombies/ZombieBot.h>
#include <zombies/ZombieServer.h>
using namespace uwe;

void print_vector(const std::vector<char> &v)
{
    for(auto i = v.begin(); i != v.end(); ++i)
    {
        std::cout << *i;
    }
}


int main(void){



       //ZombieServer(1337, zomb);
    std::vector<char> rooms;

    uwe::WorldLoader wl( uwe::readFile("world.json") );

	World world{wl.getInfo()};

	::ZombieBot zomb{world};

	printf("zombies\n");
	
	WorldLoader wl(readFile("world.json"));

	printf("info:%s\n", wl.getInfo().c_str());
	printf("startHtml:%s\n", wl.getStartHtml().c_str());
	printf("start:%s\n", wl.getStart().c_str());
	printf("inventory is:%s\n", wl.getInventoryHtml().c_str());
//	read(rooms) = wl.getRooms();
//print_vector(rooms);

	printf("end:%s\n", wl.getEnd().c_str());
	//printf("info:%s\n", wl.getInfo().c_str());
	return 0;
}
