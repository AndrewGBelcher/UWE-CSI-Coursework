/*****************************************************************************
*				AUTHOR					     *
*				======					     *
*									     *
* File: main.cpp							     *
* Author: Andrew Belcher						     *
* Date: 11-11-2017							     *
* Desc: 2048 on console and graphics at the same time 			     *
* Copyright: University of West of England 2017				     *
*									     *
*									     *
******************************************************************************/


/*****************************************************************************
*				INCLUDES				     *
*				========				     *
******************************************************************************/
// system includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// include the UFCFGL301's standard library
#include <ufcfgl-30-1.h>

// juce lib
#include <graphics.h>

// our includes
#include "input.h"

/*****************************************************************************
*				NAMESPACES				     *
*			        ==========				     *
******************************************************************************/
using namespace uwe;
/*****************************************************************************
*				FUNCTIONS				     *
*			        =========				     *
******************************************************************************/

int main(void) {

	initialiseGraphics(410,460);

	// construct the board
	Board board;


	// clear the board
	board.initBoard();

	// our main loop for each frame
	loop(

	// the draw loop
	[&](){

		// set the background to white
		fillAll(Colours::white);
		
		// display the board
		board.display();
		
		// check every frame if the game is won - it will also check if there is game over
		board.won();

	},

	// User input handling
	[&] (keyPress keyPress) {

		return(checkInput(keyPress,board));

	});


    return 0;
}
