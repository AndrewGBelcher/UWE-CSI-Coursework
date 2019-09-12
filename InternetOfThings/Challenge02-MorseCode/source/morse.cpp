/**
 * File: morse.cpp
 * Author: Andrew Belcher
 * ID: 17010347
 * Date: 11-02-2019
 * Desc: Morse Code Encoder/Decoder with Ceasar Cipher functionality
 */

#include "MicroBit.h"
#include "coder.h"

MicroBitButton buttonA(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);
MicroBitButton buttonB(MICROBIT_PIN_BUTTON_B, MICROBIT_ID_BUTTON_B);
MicroBitPin morseSig(MICROBIT_ID_IO_P2, MICROBIT_PIN_P2, PIN_CAPABILITY_ALL);

// Our globals
Coder coder;
uint8_t x;
uint8_t y;
bool dash;
bool dot;
bool stopped;
bool stop;
bool sending = false;
bool receiving = true;
char displayChar;

const uint8_t errorDecoding[] = { 
	1, 1, 1, 1, 1,
	1, 1, 0, 1, 1,
	1, 0, 1, 0, 1,
	1, 1, 0, 1, 1,
	1, 1, 1, 1, 1 }; 


// Handle morse signal from other MicroBit
void checkMorseInput()
{
	// Detect signal high
	if(morseSig.getDigitalValue() && !sending)
	{

		// Reset our message index
		if(coder.msgLen == 0)
			coder.uBit.display.clear();
			
		
		// Wait for dot length
		coder.uBit.sleep(DOT_DURATION);

		// if still pressed its atleast a dot
		if(morseSig.getDigitalValue())
		{

			dot = true;
			dash = false;
			stop = false;

			// now wait for dash length remaining
			coder.uBit.sleep(DASH_DURATION-DOT_DURATION);
				
			// its now deff a dash
			if(morseSig.getDigitalValue())
			{
				dash = true;
				dot = false;
				stop = false;
				
				coder.uBit.sleep(STOP_DURATION-DASH_DURATION);

				if(morseSig.getDigitalValue())
				{		
					dash = false;
					dot = false;
					stop = true;
					stopped = true;
					sending = false;

					while(morseSig.getDigitalValue());
				}
			}
		}
	}
}


// Handle morse code entry from user
void checkUserInput()
{
	// Detect button press
	if(buttonA.isPressed())
	{
		// clear screen on new entry
		if(coder.msgLen == 0)
			coder.uBit.display.clear();
		
		// wait for dot length
		coder.uBit.sleep(DOT_DURATION/3);

		// if still pressed its atleast a dot
		if(buttonA.isPressed())
		{
			sending = true;
			dot = true;
			dash = false;
			stop = false;

			// now wait for dash length remaining
			coder.uBit.sleep(DASH_DURATION-DOT_DURATION);
				
			// its now deff a dash
			if(buttonA.isPressed())
			{
				dash = true;
				dot = false;
				stop = false;
				
				coder.uBit.sleep(STOP_DURATION-DASH_DURATION);

				if(buttonA.isPressed())
				{		
					dash = false;
					dot = false;
					stop = true;
					stopped = true;

					// wait for button to be released
					while(buttonA.isPressed());
				}
			}
		}
	}
}

// Display char for code entered and send it encoded over 1 wire via morse code
void checkForMessageStop(void)
{

	// Either we detect a stop or B button press will be considered a stop
	if(buttonB.isPressed() || stopped)
	{
		// reinit
		x = 0;
		y = 0;
		coder.uBit.display.clear();

		// Decode morse For Display / Encode for Output
		if(sending)
		{
			// decode for display
			displayChar = coder.decodeMsg(0);
			coder.m = coder.decodeMsg(coder.ceasarRotation-2*coder.ceasarRotation);

			// encode for sending
			coder.encodeMsg(coder.m,coder.ceasarRotation);
		}

		// recveiving
		else
			coder.m = coder.decodeMsg(coder.ceasarRotation);

			
		// print the newly encoded msg
		for(int i = 0; i < coder.msgLen; i++)
		{
			coder.uBit.serial.printf("msg after enc:%d\n", (coder.newMsg[i]) ? 1:0);
		}

	
		// if decoded message is null we must display and error
		if(coder.m == NULL)
		{
			coder.uBit.serial.printf("m is null\n");
			MicroBitImage i(5,5,errorDecoding); 
			coder.uBit.display.animateAsync(i,500,5); 
		}

		// charactor exists so display it
		else
		{
			if(sending)
   				coder.uBit.display.printChar(displayChar,100);

			// receiving
			else	
   				coder.uBit.display.printChar(coder.m,100);
		}

		// send the newly encoded message
		if(sending)
		{
			for(int i = 0; i < coder.newMsgLen; i++)
			{
				coder.uBit.serial.printf("morse:%s :%d\n",(coder.newMsg[i]) ? "DIH" : "DAH",(coder.newMsg[i]) ? 1 : 0);

				// dot signal
				if(coder.newMsg[i])
				{
						morseSig.setDigitalValue(1);	
						coder.uBit.sleep(DOT_DURATION+100); // overlap		
						morseSig.setDigitalValue(0);	
						coder.uBit.sleep(500); // delay to give receiver a chance
				}

				// dash signal
				else
				{

						morseSig.setDigitalValue(1);	
						coder.uBit.sleep(DASH_DURATION+100); // overlap	
						morseSig.setDigitalValue(0);	
						coder.uBit.sleep(500); // delay to give receiver a chance				
				}			
			}

			// stop signal				
			morseSig.setDigitalValue(1);	
			coder.uBit.sleep(STOP_DURATION+100); // overlap	
			morseSig.setDigitalValue(0);	
			coder.uBit.sleep(500); // delay to give receiver a chance
				
			coder.newMsgLen = 0;
		}

		// reinit
		y=0;
		coder.msgLen = 0;
		coder.msg.clear();
	}
}


// Update display with new message entry
void displayMorseMsg(void)
{

	// display a dot and add it to the message now that one has been detected
	if(dot)
	{
		coder.msg.push_back(true);
		coder.msgLen++;

    	coder.uBit.display.image.setPixelValue(x, y, 255);	

		y++;
		x=0;

		dot = false;
		dash = false;
		stop = false;
	}

	// display a dash and add it to the message now that one has been detected
	else if(dash)
	{
		coder.msg.push_back(false);
		coder.msgLen++;			

		coder.uBit.display.image.setPixelValue(0, y, 255);	
		coder.uBit.display.image.setPixelValue(1, y, 255);
	
		y++;			
		x=0;

		dot = false;
		dash = false;
		stop = false;
	}

	// reset our state controllers, display index and clear our message
	else if(stop)
	{
		// reinit
		y=0;		
		x=0;
		dash = false;
		stop = false;
		stopped = false;
		sending = false;

		coder.msg.clear();
	}
}

int main()
{

    // Initialise the micro:bit runtime.
    coder.uBit.init();

    // Welcome message
	coder.uBit.serial.printf("Starting\n");

	// instruct the user
   	coder.uBit.display.scroll("ENTER ROTATION",50);

	// starting loop for ceasar rotation configuration (B to confirm rotation)
	while(!buttonB.isPressed())
	{
		// increment rotation
		if(buttonA.isPressed())
		{
			coder.ceasarRotation++;

			while(buttonA.isPressed());			
		}

		// no point in a huge rotation value when we just modulo it anyways
		if(coder.ceasarRotation > 35)
			coder.ceasarRotation = 0;

		// alert them of current rotation
    	coder.uBit.display.scrollAsync(coder.ceasarRotation,100);
	}


	// Main loop
	while(1)
	{
		// Prevent unessesary long messages	
		if(coder.msgLen > 5 || y > 5)
		{
			y=0;
			coder.msgLen = 0;
			coder.msg.clear();
			coder.uBit.display.clear();
		}

		// wire input takes priority over user input
		checkMorseInput();
		checkUserInput();
		
		// now check for stop and display msg
		checkForMessageStop();
		displayMorseMsg();
	}

	// End the thread
    release_fiber();
}

