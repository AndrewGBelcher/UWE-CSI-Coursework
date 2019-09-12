/**
 * File: main.cpp
 * Author: Andrew Belcher
 * Date: 26-10-2017
 * Desc: graphics lib test
 * Copyright: University of West of England 2017
 */
#include <stdio.h>

// include the UFCFGL301's standard library
#include <ufcfgl-30-1.h>

// uncomment if you want to use the graphics library
#include <graphics.h>

using namespace uwe;



int main(void) {

	float h = 80;
	float w = 80;
	float x = 80;
	float y = 80;
	
	bool upright= true;
	bool downright= false;
	bool downleft= false;
	bool upleft= false;

	printf("calling graphics init\n");
	initialiseGraphics(800,500);

	    	printf("after graphics init\n");


		loop(

		[&](){
			//Colour red;
			  //  setColour(red);
			//    setFontSize(60);
			//drawText("s",50,50,50,50);	
		
			
			drawFilledRect(x,y, h, w);

			if(upright==true){
			x=x+0.5;
			y=y-0.5;

			printf("x is:%f\n",x);
			printf("y is:%f\n",y);
			printf("upright\n");
			printf("upright:%d\n",upright);
			}

			if(y <10){
				upright=false;			
			}


			else{
			x=x+0.5;
			y=y+1;

			printf("x is:%f\n",x);
			printf("y is:%f\n",y);
			printf("upright\n");
			printf("upright:%d\n",upright);
			}
		});
	    	printf("after fill circle\n");
	


    return 0;
}
