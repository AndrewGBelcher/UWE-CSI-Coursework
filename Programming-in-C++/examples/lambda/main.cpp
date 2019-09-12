/**
 * File: main.cpp
 * Author: andy
 * Date: 24-10-2017
 * Desc:
 * Copyright: University of West of England 2017
 */
#include <stdio.h>

// include the UFCFGL301's standard library
#include <ufcfgl-30-1.h>

// uncomment if you want to use the graphics library
#include <graphics.h>

using namespace uwe;


int main(void) {
    // your  code begins here
    printf("Now implement your code!\n");


initialiseGraphics(500, 800);

printf("graphics init\n");

loop(

[&](){
	drawRect(80,80, 80, 80);
});

    return 0;
}
