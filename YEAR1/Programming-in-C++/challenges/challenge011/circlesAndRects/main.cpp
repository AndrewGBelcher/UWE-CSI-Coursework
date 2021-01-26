/*****************************************************************************
*				AUTHOR					     *
*				======					     *
* File: main.cpp							     *
* Author: Andrew Belcher						     *
* Date: 28-10-2017							     *
* Desc: program to create random circles and rectangles using 		     *
* the JUCE library						             *
* Copyright: University of West of England 2017		             	     *
*									     *
*									     *
******************************************************************************/


/*****************************************************************************
*				INCLUDES				     *
*				========				     *
******************************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// include the UFCFGL301's standard library
#include <ufcfgl-30-1.h>

// uncomment if you want to use the graphics library
#include <graphics.h>

/*****************************************************************************
*				NAMESPACES				     *
*			        ==========				     *
******************************************************************************/

using namespace uwe;

/*****************************************************************************
*				DEFINES					     *
*			        =======					     *
******************************************************************************/

#define WINDOW_HEIGHT	600
#define WINDOW_WIDTH	800

#define SHAPES_MAX 2000

/*****************************************************************************
*				STRUCTS					     *
*			        =======					     *
******************************************************************************/



struct Circle{

	float x;
	float y;
	float radius;

	uint8_t r;
	uint8_t g;
	uint8_t b;
	
};

struct Rect{
	float x;
	float y;
	float h;
	float w;

	uint8_t r;
	uint8_t g;
	uint8_t b;
};

union Shape{

Circle circle;
Rect rect;

};

struct ShapeCont{
	Shape shape;
	bool select;

};

/*****************************************************************************
*				FUNCTIONS				     *
*			        =========				     *
******************************************************************************/

// gererate a circle with random properties
Circle createCircle(){

	// make sure within window
	float x = rand()%WINDOW_WIDTH;
	float y = rand()%WINDOW_HEIGHT;

 	// not too big
	float radius = rand()%100;
	
	// randomise colour
	uint8_t r = rand();
	uint8_t	g = rand();
	uint8_t	b = rand();

	
	return(Circle{x,y,radius,r,g,b});

}

// gererate a rectangle with random properties
Rect createRect(){

	// make sure within window
	float x = rand()%WINDOW_WIDTH; 
	float y = rand()%WINDOW_HEIGHT;

 	// not too big
	float h = rand()%WINDOW_HEIGHT/2;
	float w = rand()%WINDOW_WIDTH/2;

	// randomise colour
	uint8_t r = rand();
	uint8_t	g = rand();
	uint8_t	b = rand();

	
	return(Rect{x,y,h,w,r,g,b});

}



// create a random shape with randomised properties
ShapeCont randShape(){

	// use our structs
	ShapeCont sb;
	Shape shape;

	// randomly select a circle/rectangle
	bool random = rand()%2;

	if(random){
	
		// set the flag
		sb.select = true;

		//create circle and blank rect
		sb.shape.circle = createCircle();
		shape.rect = {0,0,0,0,0,0,0};
		return(sb);
	}
	
	else{
		// set the flag
		sb.select = false;

		//create rect and blank circle
		sb.shape.rect = createRect();
		shape.circle = {0,0,0,0,0,0};
		return(sb);

	}


}

// wrapper function for drawing circles
void drawCir(ShapeCont sb){

	setColour(
		sb.shape.circle.r,
		sb.shape.circle.g,
		sb.shape.circle.b
		);

	drawFilledCircle(
		sb.shape.circle.x,
		sb.shape.circle.y,
		sb.shape.circle.radius
		);

}

// wrapper function for drawing Rectangles
void drawRect(ShapeCont sb){


	setColour(
		sb.shape.rect.r,
		sb.shape.rect.g,
		sb.shape.rect.b
		);

	drawFilledRect(
		sb.shape.rect.x,
		sb.shape.rect.y,
		sb.shape.rect.w,
		sb.shape.rect.h
		);

}

/*****************************************************************************
*				   MAIN					     *
*			           ====					     *
******************************************************************************/


int main(void) {
    

	initialiseGraphics(WINDOW_WIDTH,WINDOW_HEIGHT);

	// array of shape containers
	ShapeCont shapes[SHAPES_MAX];
	
	// initialise all to 0
	memset(shapes,0,sizeof(shapes));

	// runtime variables
	bool pause = false;
	int x = 0;

	// the draw frame loop
	loop(

	[&](){
		// randomise via the clock for speedy generataion of shapes
		srand(clock());

		// catch when too many shapes for our array is created and reset array
		if(x >= SHAPES_MAX){
			x = 0;
			memset(shapes,0,sizeof(shapes));
		}

		// only make new shapes if program isnt paused
		if(pause == false){

			shapes[x] = randShape();

		}

		// our loop for drawing every shape we create
		for(int i=0; i<SHAPES_MAX; i++){

			// only draw shape if one exists
			if(shapes[i].shape.circle.x != 0 && shapes[i].shape.rect.x != 0){

				// if shape is a circle
				if(shapes[i].select == true){

					drawCir(shapes[i]);
				}
				// if shape is a rectangle
				else if(shapes[i].select == false){
					
					drawRect(shapes[i]);
				}
			}

		}
		
		// our shape counter
		printf("x:%d\n",x);

		// only increment counter if pause is off
		if(!pause){
			x++;
		}
	

	},

	// user input
	[&] (keyPress keyPress) {

		// Pause our shape generator
		if (getTextCharacter(keyPress) == 'p' || getTextCharacter(keyPress) == 'P') {
			if(pause == true){
				pause = false;
			}
			else{
				pause = true;		
			}
			return false; // prevent program from crashing
		}
		// Close our program
		if (getTextCharacter(keyPress) == 'q' || getTextCharacter(keyPress) == 'Q') {

		        return true; // return true to exit program
		     }

		else{
			return false; // prevent program from crashing
		}
	});

    return 0;
}

