/*****************************************************************************
*				AUTHOR					     *
*				======					     *
* File: main.cpp							     *
* Author: Andrew Belcher						     *
* Date: 03-11-2017							     *
* Desc: Program to animate balls bouncing within the window using 	     *
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
#include <sys/time.h>

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

#define WINDOW_HEIGHT 500
#define WINDOW_WIDTH 600

/*****************************************************************************
*				STRUCTS					     *
*			        =======					     *
******************************************************************************/

struct Ball{

	float x;
	float y;
	float radius;
	Colour c;
	float xv;
	float yv;

};

/*****************************************************************************
*				FUNCTIONS				     *
*			        =========				     *
******************************************************************************/


Ball createBall(float radius,Colour c,float xv, float xy){
	
	return(Ball{0 + radius,0 + radius,radius,c,xv,xy});

}

bool inWindow(Ball ball){
			
	// within window bounds
	if(
		ball.y + ball.radius < WINDOW_HEIGHT &&
		ball.x + ball.radius < WINDOW_WIDTH && 
		ball.x - ball.radius > 0 &&
		ball.y - ball.radius > 0 
	)
	{
		return true;		
	}
	else{
		return false;		
	}

};



/*****************************************************************************
*				   MAIN					     *
*			           ====					     *
******************************************************************************/

int main(void) {

	// a nice random seed for our colours
	srand(clock());

	// initialise balls
	Colour col = {255,0,0};

	int ballCnt = 1; 
	int ballMax = 5;

	float curRadius = 20;
	float radiusMax = 50;

	Ball balls[ballCnt];
	
	// set them all to 0
	for(int c = 0; c < ballCnt; c++){

		Colour noCol = {0,0,0};
		balls[c] = {0,0,0,noCol,0,0};
	}
	
	// start the program with a single ball and set speed
	balls[0] = createBall(curRadius,col,5,5);



	// assign our random colour globally so we can change it via keypress
	uint8_t randR = rand();
	uint8_t randG = rand();
	uint8_t randB = rand();

	bool up = true;
	bool right = true;

	// wall detection bools
	bool rh = false;
	bool lh = false;
	bool th = false;
	bool fh = false;

	// colour bools
	bool random = false;
	bool b = true;
	bool r = false;
	
	// our current ball being created
	int curball = 1;



	initialiseGraphics(WINDOW_WIDTH,WINDOW_HEIGHT);


	loop(
	[&](){

	// run our loop on every ball we need to create
	for(int i = 0; i < ballCnt; i++){

	// bounce detection

		// left wall
		if(balls[i].x - balls[i].radius < 0){

			balls[i].xv = balls[i].xv*-1;
		}
		// right wall
		if(balls[i].x + balls[i].radius > WINDOW_WIDTH){

			balls[i].xv = balls[i].xv*-1;
		}
		// floor
		if(balls[i].y - balls[i].radius< 0){

			balls[i].yv = balls[i].yv*-1;
		}
		// ceiling
		if(balls[i].y + balls[i].radius > WINDOW_HEIGHT){
			balls[i].yv = balls[i].yv*-1;
		}
		
		// collision detection

		for(int j= 0; j < ballCnt; j++){


			// only detect if 2 balls are in the window and are differnt balls
			if(i != j && inWindow(balls[i]) && inWindow(balls[j])){
	

				// use Pythagoras's theorem for detecting 2 balls overlaping
				int dx = balls[i].x - balls[j].x; 
				int dy = balls[i].y - balls[j].y;
				int dr = balls[i].radius + balls[j].radius;

				float distance = sqrt((dx * dx) + (dy * dy));
					 

				//balls have collided and occupy the same space
				if (distance < dr){


					// check both balls velocity and invert it upon collision
					if (balls[j].xv < 0){
						balls[j].xv = balls[j].xv*+1;
						balls[i].xv = balls[i].xv*+1;
					}
					else{
						balls[j].xv = balls[j].xv*-1;
						balls[i].xv = balls[i].xv*-1;
					}
					if (balls[j].xv < 0){
						balls[j].yv = balls[j].yv*+1;
						balls[i].yv = balls[i].yv*+1;
					}
					else{
						balls[j].yv = balls[j].yv*-1;
						balls[i].yv = balls[i].yv*-1;
					}

					printf("collision\n");
				}
				else{
					printf("no collision\n");
				}
			}
		} // end of ball array checking

			// movement
			
				if(right){
					balls[i].x = balls[i].x + balls[i].xv;
				}
				if(!right){
					balls[i].x = balls[i].x - balls[i].xv;
				}
				if(up){
					balls[i].y = balls[i].y + balls[i].yv;
				}
				if(!up){
					balls[i].y = balls[i].y - balls[i].yv;
				}


			// color changing
			
				// check if color has been set to random
				if(random){
					setColour(randR,randG,randB);
				}
				// if not random and blue is set
				else if(b){
					setColour(0,0,255);
				}
				// if not blue or random its red
				else{
					setColour(255,0,0);
				}


			// if ball is populated draw it
				if(balls[i].x != 0){
					drawFilledCircle(balls[i].x,balls[i].y, curRadius);
				}	
			}			

	},


// User input handling
[&] (keyPress keyPress) {


	
		// quit the program
		if (getTextCharacter(keyPress) == 'q' || getTextCharacter(keyPress) == 'Q') {

		        return true; // return true to quit loop
		     }

		// turn the balls blue
		if (getTextCharacter(keyPress) == 'b' || getTextCharacter(keyPress) == 'B') {

			b = true;
			r = false;
			random = false;

		        return false; 
		     }

		// turn the balls red
		if (getTextCharacter(keyPress) == 'r' || getTextCharacter(keyPress) == 'R') {
			
			b = false;
			r = true;
			random = false;

		        return false;
		     }

		// turn the balls a random colour
		if (getTextCharacter(keyPress) == 'x' || getTextCharacter(keyPress) == 'X') {
			
			b = false;
			r = false;
			random = true;

			randR = rand();
			randG = rand();
			randB = rand();
		

		        return false;
		    }

		// create new balls
		if (getTextCharacter(keyPress) == ' ') {

			if(curball <= ballMax){
				balls[curball] = createBall(curRadius,col,5,5);
				curball++;
				ballCnt++;
			
			}
		        return false;
		    }

		// decrease the balls speed
		if (getTextCharacter(keyPress) == '1' || getTextCharacter(keyPress) == '!') {

			for(int i = 0; i <= ballMax; i++){
				if (balls[i].xv >= 1 || balls[i].yv >= 1){

					--balls[i].xv;
					--balls[i].yv;
				}
			}

			return false;
		     }

		// increase the balls speed
		if (getTextCharacter(keyPress) == '2' || getTextCharacter(keyPress) == '"') {
			for(int i = 0; i <= ballMax; i++){
			
				++balls[i].xv;
				++balls[i].yv;
			}
			return false;
		     }

		// shink the balls
		if (getTextCharacter(keyPress) == '-' || getTextCharacter(keyPress) == '_') {

				for(int i = 0; i <= ballCnt; i++){
					if (curRadius > 5){
						balls[i].radius = curRadius--;
					}
				}
			
			return false;
		     }

		// grow the balls
		if (getTextCharacter(keyPress) == '=' || getTextCharacter(keyPress) == '+') {

			for(int i = 0; i <= ballCnt; i++){
				if (curRadius < radiusMax){
					balls[i].radius = curRadius++;
				}
			}
			return false;
		     }
	
		// make sure to return false if no condition is met to avoid illegal instruction error
		else{
			return false;	
		}



	});

	

    return 0;
}

