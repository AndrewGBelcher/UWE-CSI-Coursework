/*****************************************************************************
*				AUTHOR					     *
*				======					     *
* File: initials.cpp							     *	
* Author: Andrew Belcher						     *
* Date: 13-10-2017						             *
* Desc: Initials from name conversion program				     *
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

#define A 0x41
#define Z 0x58
#define a 0x61
#define z 0x7a
#define BLANK 0x20

/*****************************************************************************
*				FUNCTIONS				     *
*			        =========				     *
******************************************************************************/


int main(void)
{
    char name[100];
    char initials[5];

    int i = -1; 
    int p = -1;

    // ask user for a name
    printf("enter your name:");
    scanf ("%[^\n]%*c", name);


    for(i = 0; name[i]!='\n' && name[i] != '\0'; i++) // when the letter in the line has no newline or null char, loop
    {
        if (name[i] >= a && name[i] <= z) // if within lowercase ascii
        {
            initials[++p] = toupper(name[i]); // convert letter to uppercase and store in initials
        }
        else // if they are already uppercase
        {
            initials[++p] = name[i]; 
        }
        while ((name[i] != BLANK) && (name[i] != '\n') && (name[i] != '\0'))
        {
            i++;
        }
    }

	for(int e=0; e <= sizeof(initials); e++){
		if(initials[e] == BLANK){
			initials[e] = initials[e+1];
			initials[e+1] = '\0';
		}
	}

    initials[++p] = '\0';
    printf("Your initials are:%s\n", initials);

	return 0;

}

