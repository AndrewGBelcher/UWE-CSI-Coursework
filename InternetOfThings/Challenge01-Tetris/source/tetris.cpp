/**
 * File: tetris.cpp
 * Author: Andrew Belcher
 * ID: 17010347
 * Date: 2-02-2019
 * Desc: Classic tetris developed for 5x5 led display, features original SNES/SuperFamicom controller support!
 */

#include "MicroBit.h"

// Input handling choice, supports a superfamicom/snes original game pad
#define SNES true

#ifdef SNES
MicroBitPin padData(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_ALL);
MicroBitPin padClock(MICROBIT_ID_IO_P1, MICROBIT_PIN_P1, PIN_CAPABILITY_ALL);
MicroBitPin padLatch(MICROBIT_ID_IO_P2, MICROBIT_PIN_P2, PIN_CAPABILITY_ALL);
#else
MicroBitButton buttonA(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);
MicroBitButton buttonB(MICROBIT_PIN_BUTTON_B, MICROBIT_ID_BUTTON_B);
#endif


// Our globals
MicroBit uBit;

int frames;
uint8_t x = 3;
uint8_t y = 0;
int score = 0;
bool brick[2][2];
bool GameOver = false;
bool tetrisBricks[4][2][2] = {{{true, true}, {true, false}}, {{true, true}, {false, true}}, {{true, true}, {true, true}}, {{true, true}, {false, false}}}; 
bool board[6][7];


// SNES gamepad class, very simple just samples controller input via a latch/clock output and data input
class Snes
{
public:
        uint16_t word;

		uint16_t getReading(void)
		{	
			// Drive latch on pin2
			padLatch.setDigitalValue(1);
			padLatch.setDigitalValue(0);
			word = 0;
			
			for(int i=0; i<16; i++)
			{
				// Drive clock on pin1			
				padClock.setDigitalValue(0);

				// Read data on pin0
				uint16_t val = padData.getDigitalValue();
				word = word + (val<<i);

				// Rising egde after read
				padClock.setDigitalValue(1);
			}
			return word;
		}
};


// Print out the state of the board for debugging over serial
void printBoardDebug(void)
{
	for(int w = 0; w < 6; w++){

			uBit.serial.printf("%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\n",
				board[w][0],
				board[w][1],
				board[w][2],
				board[w][3],
				board[w][4],
				board[w][5],
				board[w][6]);
	}
}

// Check if a line has been completed and remove it and shift the board down 1
bool checkForLines()
{
	bool removingLine = false;

	for(int i=0; i<5;i++)
	{
		if(((int)board[i][1]+
			(int)board[i][2]+
			(int)board[i][3]+
			(int)board[i][4]+
			(int)board[i][5]) == 5)
		{
			uBit.serial.send("line found\n\n\n");

			// Remove line
			removingLine = true;

			// Increment score
			score = score + 10;
			
			// Refresh screen
			for(int j = i; j > 0; j --)
			{
				// Move rows above cleared row down 1 row
				for(int d = 1; d < 6; d++)
				{ 
					board[j][d] = board[j-1][d];
				}

				// Clear top row
				board[0][0] = 1;
				board[0][1] = 0;
				board[0][2] = 0;
				board[0][3] = 0;
				board[0][4] = 0;
				board[0][5] = 0;
				board[0][6] = 1;
			}
		}
	}

	// We got one
	if(removingLine)
	{
		for(int i=0; i <5; i++)
		{
			for(int d=0; d <5; d++)
			{
				uBit.display.image.setPixelValue(i,d,board[d][i+1]);
			}	
		}
	}

	return removingLine;
}

// Get a random brick from the peices available
void chooseRandBrick(void)
{
	int rand = uBit.random(4);
	brick[0][0] = tetrisBricks[rand][0][0];
	brick[0][1] = tetrisBricks[rand][0][1];
	brick[1][0] = tetrisBricks[rand][1][0];
	brick[1][1] = tetrisBricks[rand][1][1];
}


// Turn off our current Brick from the display
void hideTetrisBrick()
{

	if(x>0)
    		uBit.display.image.setPixelValue(x-1, y, board[y][x]);
	if(x<5)
    		uBit.display.image.setPixelValue(x, y,board[y][x+1]);
	if(x>0 && y < 4)
    		uBit.display.image.setPixelValue(x-1, y+1, board[y+1][x]);
	if(x<5 && y < 4)
    		uBit.display.image.setPixelValue(x, y+1, board[y+1][x+1]);
}

// Turn on our current Brick from the display
void displayTetrisBrick()
{

	if(x>0)
    		uBit.display.image.setPixelValue(x-1, y, (brick[0][0] || board[y][x]) ? 255 : 0);
	if(x<5)
    		uBit.display.image.setPixelValue(x, y,(brick[0][1] || board[y][x+1]) ? 255 : 0);
	if(x>0 && y < 4)
    		uBit.display.image.setPixelValue(x-1, y+1,(brick[1][0] || board[y+1][x]) ? 255 : 0);
	if(x<5 && y < 4)
    		uBit.display.image.setPixelValue(x, y+1, (brick[1][1] || board[y+1][x+1]) ? 255 : 0);
}

// Shift the tetris brick in the direction supplied
bool moveTetrisBrick(int x_delta, int y_delta)
{
	bool move = false;

	// Can we move? have we collided?

	// Detect Blocks left of ours
	if(x_delta == -1 && x > 0)
	{
		if(!
			((board[y][x-1] && (int)brick[0][0]) ||
			(board[y][x] && (int)brick[0][1]) ||
			(board[y+1][x-1] && (int)brick[1][0]) ||
			(board[y+1][x] && (int)brick[1][1])
			))
		{
			move = true;
		}	
	}
		
	// Detect blocks right of ours
	else if(x_delta == 1 && x < 5)
	{
		if(!
			((board[y][x+1] && (int)brick[0][0]) ||
			(board[y][x+2] && (int)brick[0][1]) ||
			(board[y+1][x+1] && (int)brick[1][0]) ||
			(board[y+1][x+2] && (int)brick[1][1])
			))
		{
			move = true;
		}
	}

	// Detect blocks below, for moving down
	else if(y_delta == 1 && y < 4)
	{
		if(!
			((board[y+1][x] && (int)brick[0][0]) ||
			(board[y+1][x+1] && (int)brick[0][1]) ||
			(board[y+2][x] && (int)brick[1][0]) ||
			(board[y+2][x+1] && (int)brick[1][1])
			))
		{
			move = true;
		}
	}

	// All clear
	if(move)
	{
		hideTetrisBrick();
		x = x + x_delta;
		y = y + y_delta;
		displayTetrisBrick();
	}
	
	return move;
}

// Initialise our tetris grid
void clearBoard(void)
{
	for(int a = 0; a<7; a++)
	{
		for(int b = 0; b <6; b++)
		{
			if(a == 0)
				board[b][a] = true;
			else if(b == 5)
				board[b][a] = true;
			else if(a == 6)
				board[b][a] = true;			
			else		
				board[b][a] = false;	
		}
	}
}

// Rotate the brick in use based on user input
void rotateTetrisBrick(bool clockwise)
{
	bool cell00, cell01, cell10, cell11;
	cell00 = brick[0][0];
	cell01 = brick[0][1];
	cell10 = brick[1][0];
	cell11 = brick[1][1];

	// can we rotate
	if(!
		((board[y][x] && cell00) ||
		(board[y+1][x] && cell01) ||
		(board[y][x+1] && cell10) ||
		(board[y+1][x+1] && cell11)
		))
	{
		// for button A on snes pad
		if(clockwise)
		{
			hideTetrisBrick();
			brick[0][0] = cell01;
			brick[1][0] = cell00;
			brick[1][1] = cell10;
			brick[0][1] = cell11;
			displayTetrisBrick();
		}

		else
		{
			hideTetrisBrick();
			brick[0][0] = cell10;
			brick[1][0] = cell11;
			brick[1][1] = cell01;
			brick[0][1] = cell00;
			displayTetrisBrick();
		}
	}
}


int main()
{

	// Input variables
	uint16_t lastpad =0;
	uint16_t word = 0;
	uint16_t curpad = 0;

    // Initialise the micro:bit runtime.
    uBit.init();
	
	// Create and instance of our SNES pad class if we are using it
#ifdef SNES
	Snes pad;
#endif

	// init the board	
	clearBoard();

    // Welcome message
	uBit.serial.printf("Starting\n");

	while(1)
	{
    	uBit.display.scroll("TETRIS",50);

#ifdef SNES
		word = (~pad.getReading() & 0x0000ffff);

		if(word == 8)
			break;
#else
		if(buttonA.isPressed() && buttonB.isPressed())
			break;
#endif

	}
	
	// Start off the game with 1st brick
	chooseRandBrick();
	displayTetrisBrick();

	while(1){

		while(!GameOver) 
		{
			// Frame rate based on 50ms
			uBit.sleep(50);
			frames++;

		    // Input handling code, for snes controller or microbit buttons a/b

	#ifdef SNES
			// Get the 16bit number from our SNES controller
			word = pad.getReading();

			// Invert our input word a bit is set as 0 when button is pressed
			// Mask upper 2 bytes to remove any signing
			curpad = (~word & 0x0000ffff);
			uBit.serial.printf("button:%d\n",curpad);		

			// Detect if button input changes
			if(curpad != lastpad)
			{
				// UP
				if(curpad == 16)
					uBit.serial.send("up\n");
				
				// DOWN
				else if(curpad == 32)
				{
					moveTetrisBrick(0,1);
					uBit.serial.send("down\n");
				}

				// RIGHT
				else if(curpad == 128)
				{
					moveTetrisBrick(1,0);
					uBit.serial.send("right\n");
				}

				// LEFT
				else if(curpad == 64)
				{
					uBit.serial.send("left\n");
					moveTetrisBrick(-1,0);
				}

				// A
				else if(curpad == 256)
				{
					rotateTetrisBrick(true);			
					uBit.serial.send("A\n");
				}			

				// B
				else if(curpad == 1)
				{
					rotateTetrisBrick(false);			
					uBit.serial.send("B\n");
				}	
			}

			// You need to press the button again for a new input :)
			lastpad = curpad;

	#else
			
			// Standard input control for the Microbit buttons, if we dont have a SNES controller hooked up
			if(buttonA.isPressed() && buttonB.isPressed())
			{
				// Always counter clockwise
				rotateTetrisBrick(false);
			}

			else if(buttonA.isPressed())
			{		
				// LEFT
				moveTetrisBrick(-1,0);
			}

			else if(buttonB.isPressed())
			{
				// RIGHT
				moveTetrisBrick(1,0);
			}

	#endif

			// If we cant move at refresh rate, we do some checking here..
			if(frames == 20 && !(moveTetrisBrick(0,1)))	
			{
				// Reset frame count
				frames = 0;

				// Keep brick in place till we figure out if we can actually move just yet		
				board[y][x] = (brick[0][0] || board[y][x]);
				board[y][x+1] = (brick[0][1] || board[y][x+1]);
				board[y+1][x] = (brick[1][0] || board[y+1][x]);
				board[y+1][x+1] = (brick[1][1] || board[y+1][x+1]);

				// Lets see if we can move in a sec by clearing any lines			
				if(!checkForLines() && y==0)
					GameOver = true;

				else
				{
					// We can move, so push new block to screen
					x = 3;
					y = 0;
					chooseRandBrick();
					displayTetrisBrick();
				}
			}

			// Reset frame count
			if(frames == 20)	
				frames = 0;
		}

		// Clear screen and show game over screen		
		uBit.display.clear();
		uBit.serial.send("GAME OVER!\n");
	
		// Stay in game over screen till start is hit, *Start needs holding, or B on microbit if not using SNES pad
		while(GameOver)
		{
			// Show the player their score and check if they have held down the button
			uBit.display.scroll("GAME OVER! SCORE:",50);		
			uBit.display.scroll(score,50);		

	#ifdef SNES
			word = (~pad.getReading() & 0x0000ffff);

			if(word == 8)
				GameOver = false;
	#else
			if(buttonB.isPressed())
				GameOver = false;
	#endif
		}

            // Player exited loop so now time to start the game over		
            score = 0;
            clearBoard();
    		
            // setup board for a new run
            x = 3;
            y = 0;
        		
            // Start off the game with 1st brick
            chooseRandBrick();
            displayTetrisBrick();
	}
	
	// End the thread
    release_fiber();
}

