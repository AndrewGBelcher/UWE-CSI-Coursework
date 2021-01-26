/**
 * File: morse.cpp
 * Author: Andrew Belcher
 * ID: 17010347
 * Date: 25-03-2019
 * Desc: Morse Code Encoder/Decoder with Ceasar Cipher functionality 
 * (Radio Edition)
 */

#include "MicroBit.h"
#include "coder.h"

MicroBitButton buttonA(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);
MicroBitButton buttonB(MICROBIT_PIN_BUTTON_B, MICROBIT_ID_BUTTON_B);

// Our globals
Coder coder;
uint8_t x;
uint8_t y;
char displayChar;
bool sendMode = false;

const uint8_t errorDecoding[] = { 
	1, 1, 1, 1, 1,
	1, 1, 0, 1, 1,
	1, 0, 1, 0, 1,
	1, 1, 0, 1, 1,
	1, 1, 1, 1, 1 }; 

// Update display with new message entry
void displayMorseMsg(int morse)
{

	// Display a dot and add it to the message now that one has been detected
	if(morse==0)
	{
		coder.msg.push_back(true);
		coder.msgLen++;

    	coder.uBit.display.image.setPixelValue(x, y, 255);	

		y++;
		x=0;
	}

	// Display a dash and add it to the message now that one has been detected
	else if(morse==1)
	{
		coder.msg.push_back(false);
		coder.msgLen++;			

		coder.uBit.display.image.setPixelValue(0, y, 255);	
		coder.uBit.display.image.setPixelValue(1, y, 255);
	
		y++;			
		x=0;
	}

	// Reset our display index and clear our message
	else if(morse==2)
	{
		// Reinit
		y=0;		
		x=0;

		if(sendMode)
		{
			coder.msg.clear();
			coder.uBit.display.clear();
		}
	}
}


// Process the morse code message and diplay it
void processCode(void)
{

			// Decode morse For Display / Encode for Output
			if(sendMode)
			{
				// Decode for display
				displayChar = coder.decodeMsg(coder.ceasarRotation);
				coder.m = coder.decodeMsg(coder.ceasarRotation-2*coder.ceasarRotation);

				// Encode for sending
				coder.encodeMsg(coder.m, coder.ceasarRotation);
			}

			// Recveiving
			else
				coder.m = coder.decodeMsg(coder.ceasarRotation);

			
			// Print the newly encoded msg
			for(int i = 0; i < coder.msgLen; i++)
			{
				coder.uBit.serial.printf("msg after enc:%d\n", (coder.newMsg[i]) ? 1:0);
			}

	
			// If decoded message is null we must display and error
			if(coder.m == NULL)
			{
				coder.uBit.serial.printf("m is null\n");
				MicroBitImage i(5,5,errorDecoding); 
				coder.uBit.display.animateAsync(i,500,5); 
			}

			// Charactor exists so display it
			else
			{
				if(sendMode)
	   				coder.uBit.display.printChar(displayChar,100);

				// Receiving
				else	
	   				coder.uBit.display.printChar(coder.m,100);
			}

			// Clear the queued message now that its processed
			coder.msgLen = 0;
			coder.msg.clear();
}


// Handle radio input via datagram and display accordingly
void recvMorse(MicroBitEvent)
{

	// Only in receiving mode handle msg input
	if(!sendMode)
	{
		ManagedString s = coder.uBit.radio.datagram.recv();

		// Reset our message index
		if(coder.msgLen == 0)
			coder.uBit.display.clear();
			
		// Handle Dih
		if (s == ".")
		{
			displayMorseMsg(0);
		}
	
		// Handle Dah
		if (s == "-")
		{
			displayMorseMsg(1);
		}

		// Handle Stop
		if (s == "!")
		{
			// Process the queued message
			processCode();
			displayMorseMsg(2);
		}
	}
}


int main()
{

    // Initialise the micro:bit runtime.
    coder.uBit.init();

	// Setup the radio
	coder.uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, recvMorse);
	coder.uBit.radio.enable();

	// Determine if we are Sending or Receiving
    coder.uBit.display.scrollAsync("A=SEND B=RECEIVE",100);
	while(1)
	{
			if(buttonA.isPressed())
			{
				sendMode = true;
				break;			
			}
			if(buttonB.isPressed())
			{
				sendMode = false;
				break;
			}
	}

	// Tell user to set ceasar cipher rotation
   	coder.uBit.display.scroll("ENTER ROTATION",50);

	// Starting loop for ceasar rotation configuration (B to confirm rotation)
	while(!buttonB.isPressed())
	{
		// Increment rotation
		if(buttonA.isPressed())
		{
			coder.ceasarRotation++;

			while(buttonA.isPressed());			
		}

		// No point in a huge rotation value when we just modulo it anyways
		if(coder.ceasarRotation > 35)
			coder.ceasarRotation = 0;

		// Alert them of current rotation
    	coder.uBit.display.scrollAsync(coder.ceasarRotation,100);
	}


	// Communication handling loop
    while(1)
	{
		int code;
		char* tmpchar;
		
		// Prevent unessesary long messages	
		if(coder.msgLen > 5 || y > 5)
		{
			y=0;
			coder.msgLen = 0;
			coder.msg.clear();
			coder.uBit.display.clear();
		}


		// User input handling	
		if(sendMode)
		{
			if(buttonA.isPressed())
			{

				// Clear screen on new entry
				if(coder.msgLen == 0)
					coder.uBit.display.clear();
		
				// Set it to Dih and wait to check it again
				coder.uBit.sleep(DOT_DURATION);	
				tmpchar = ".";
				code = 0;

				if(buttonA.isPressed())
				{
		
					// Set it to Dah and wait to check it again
					coder.uBit.sleep(DASH_DURATION-DOT_DURATION);
					tmpchar = "-";
					code = 1;

					if(buttonA.isPressed())
					{

						// Set it to Stop and wait till its released
						coder.uBit.sleep(STOP_DURATION-DASH_DURATION);
						tmpchar = "!";
						code = 2;

						if(buttonA.isPressed())
							while(buttonA.isPressed());
					}
				}		

				// Send the signal and Display its entry
				coder.uBit.radio.datagram.send(tmpchar);
				displayMorseMsg(code);
			}
		}


		// Only happens in Send mode, process the message and display it
		if(code == 2)
		{
			processCode();
			code = 0;
		}

		// Needed for radio input detection
		coder.uBit.sleep(100);
	}


	// End the thread
    release_fiber();
}

