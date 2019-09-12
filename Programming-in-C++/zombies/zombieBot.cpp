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


	ZombieBot zomb;

	ZombieBot::shouldQuit(){
		return 0;
	};

        /**
         * text to be displayed at beginning of game
         * @return text to be displayed
         */
        ZombieBot::begin(){return 0};

        /**
         * compute current score
         * @return current score
         */
        ZombieBot::currentScore(){return 0};

        /**
         * should zombie timer be enabled
         * @return true if timer should be enabled, otherwise false
         */
        ZombieBot::enableTimer(){return 0};

        /**
         * should zombie timer be disabled
         * @return true if timer should be disabled, otherwise false
         */
        ZombieBot::disableTimer(){return 0};

  
        ZombieBot::processCmd(std::string cmd){return 0};
    };

} // namespace uwe
