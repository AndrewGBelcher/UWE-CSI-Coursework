#include "input.h"

// comment out debug to turn DEBUG_PRINT off
#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(...) do{ b.print(); } while( false )
#else
#define DEBUG_PRINT(...) do{ } while ( false )
#endif


bool checkInput(KeyPress keyPress, Board b){

		// quit the program
		if (getTextCharacter(keyPress) == 'q' || getTextCharacter(keyPress) == 'Q') {

		        return true; // return true to quit loop
		}
		// restart the game
		if (getKeyCode(keyPress) == KeyPress::returnKey) {

			b.initBoard();
		        return false;
		}

#ifdef DEBUG
		// press space to add new blocks, helpful for making the game easy when debugging
		if (getTextCharacter(keyPress) == ' ') {

			b.addRandCellDebug();
		        return false;
		}
#endif

		if(getKeyCode(keyPress) == KeyPress::upKey){
#ifdef DEBUG
			printf("up key used\n");
#endif			
			b.moveUp();	
			b.addRandCell();

			DEBUG_PRINT();

			return false;
		}
		if(getKeyCode(keyPress) == KeyPress::rightKey){
#ifdef DEBUG	
			printf("right key used\n");
#endif
			b.moveRight();
			b.addRandCell();	
			
			DEBUG_PRINT();

			return false;
		}
		if(getKeyCode(keyPress) == KeyPress::downKey){
#ifdef DEBUG
			printf("down key used\n");
#endif
			b.moveDown();
			b.addRandCell();	
			
			DEBUG_PRINT();

			return false;
		}
		if(getKeyCode(keyPress) == KeyPress::leftKey){
#ifdef DEBUG
			printf("left key used\n");
#endif
			b.moveLeft();
			b.addRandCell();

		 	DEBUG_PRINT();

			return false;
		}

		// when there is no input
		else{
			return false;	
		}

}
