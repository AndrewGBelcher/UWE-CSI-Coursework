/*****************************************************************************
*				AUTHOR					     *
*				======					     *
* File: ceasar.cpp							     *	
* Author: Andrew Belcher						     *
* Date: 11-10-2017						             *
* Desc: Ceasar Cipher with custom rotation				     *
* Copyright: University of West of England 2017			             *
*									     *
*									     *
******************************************************************************/


/*****************************************************************************
*				INCLUDES				     *
*				========				     *
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// include the UFCFGL301's standard library
#include <ufcfgl-30-1.h>

// uncomment if you want to use the graphics library
//#include <graphics.h>

/*****************************************************************************
*				NAMESPACES				     *
*			        ==========				     *
******************************************************************************/

using namespace uwe;

/*****************************************************************************
*				DEFINES					     *
*			        =======					     *
******************************************************************************/

// rememember* 0x7a = z 0x61 = a, 0x41 = A 0x5a = Z, range of 0x39, 26 letters or 0x1a letters

#define A 0x41
#define Z 0x58
#define a 0x61
#define z 0x7a

#define INVALID_ARGS -1
#define NOT_A_STRING -2

/*****************************************************************************
*				FUNCTIONS				     *
*			        =========				     *
******************************************************************************/

// Ceasar Cipher Algorithm
unsigned char rotate(int rotation, unsigned char letter){

	// apply rotation		
	letter += rotation;

	 // check if the letter is z and set it back to the beginning
	if( letter > z ){

		// if the new postion of the letter is beyond the scope, 
		// set to the start and add the remainder
		letter = (a + (letter - z)-1); // -1 just repositions just before a instead of becoming a
		return letter;
		}
	else{
      		return letter;	
	}
}


int main(int argc, char ** argv) {


	// catch arguments first to avoid memory leak
	if(argc <  3 || argc > 3){
		printf("usage: message rotation\n");
		exit(INVALID_ARGS);
	}
	
	// now that we have 2 arguments we can assign them
	char *word = argv[1];
	unsigned int rotation = atoi(argv[2]);


	// make sure rotation is within the scope of the alphabet
	if (rotation > 26){
		rotation = rotation % 26;
	}
	printf("rotation applied:%d\n", rotation);


	// loop through each letter in the supplied message
	for(int i=0;word[i] != 0;i++){

		// convert to lowercase
		if(word[i] >= A && word[i] <= Z){
			word[i] = tolower(word[i]);
		}
			
		// lowercase
                if(word[i] >= a && word[i] <= z){

			//word[i] = word[i] + rotation; // add the rotation to current letter
			word[i] = rotate(word[i],rotation);
		}

		// not in the alphabet
		else{
			printf("please enter a real word next time, thank you.\n");
			exit(NOT_A_STRING);
		}
	}

	// provide the user with the enc message
	printf("encrypted word is:%s\n",word);
 
   return 0;
}
