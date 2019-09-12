/**
 * File: main.cpp
 * Author: Andrew Belcher
 * Date: 12-10-2017
 * Desc:
 * Copyright: University of West of England 2017
 */
#include <stdio.h>

// include the UFCFGL301's standard library
#include <ufcfgl-30-1.h>

// uncomment if you want to use the graphics library
//#include <graphics.h>

using namespace uwe;


int adder(int a, int b){
	return (a+b);
}
/**
 * main routine, implements ...
 * @return      success
 */
int main(void) {
	
	int a;
	int b;

	printf("input number 1:");
	scanf("%d", &a);
	printf("input number 2:",b);
	scanf("%d", &b);
	int c = adder(a,b);
	printf("added %d, and %d to equal %d\n", a, b, c);
    return 0;
}
