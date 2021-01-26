/*
 * File: main.cpp
 * Author: Andrew Belcher
 * Date: 05-10-2017
 * Desc: Simple hello world program with some interesting bells and whistles
 * Copyright: University of West of England 2017
 */

#include <stdio.h>

// include the UFCFGL301's standard library
#include <ufcfgl-30-1.h>

// uncomment if you want to use the graphics library
//#include <graphics.h>

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

using namespace uwe;

struct hello {
	char *first = "H";
	char *second = "E";
	char *third = "L";
        char *forth = "L";
	char *fifth = "O";

};

// a nice hexdump function taking from pieces of code here and there
int hexDump(const void *data, size_t size)
{
	unsigned char *d = (unsigned char *)data;
	size_t consoleSize = 16;
	char b[consoleSize + 3];
	size_t i;

	if(data == NULL){
		return -1;
		}
	b[0] = '|';
	b[consoleSize + 1] = '|';
	b[consoleSize + 2] = '\0';
	
	printf("\n-------HEX DUMP------\n");
	for (i = 0; i < size; i++)
	{
		if ((i % consoleSize) == 0)
		{
			if (i != 0){
				printf("  %s\n", b);
				}
			printf("%016lx ", (unsigned char *)data + i);
		}

		if(i % consoleSize == 8)
			printf(" ");
		printf(" %02x", d[i]);

		if (d[i] >= ' ' && d[i] <= '~')
			b[i % consoleSize + 1] = d[i];
		else
			b[i % consoleSize + 1] = '.';
	}
	while((i % consoleSize) != 0)
	{
		if(i % consoleSize == 8)
			printf("    ");
		else
			printf("   ");
		b[i % consoleSize + 1] = '.';
		i++;
	}
	printf("  %s\n", b);
	return 0;
}

// wrapper function for the getpid syscall
int pid(void){
	
	int a = getpid();
	return(a);
}


// standard function to retrieve stack pointer through assembly
uint64_t getsp( void )
{
    uint64_t sp;
    asm( "mov %%rsp, %0" : "=rm" ( sp ));
    return sp;
}

/**
 * main routine, implements ...
 * @return      success
 */
int main(void) {

	char *hello = "Hello_World!";
	char helloArray[10] = "Hello_World";

	// print some of the forms hello comes in
	printf("in string form:%s\n", hello);
	printf("in decimal form:%d\n", hello);
	printf("in hexadecimal form:%x\n", hello);
	printf("in long long form:0x%016llx\n", hello);
	printf("in octal form:%o\n", hello);
	printf("where is hello's pointer stored in memory:%p\n", &hello);
	printf("in integer form:%i\n", hello);

	// hex dump some memory
	printf("hex dumping hello directly..\n");
	hexDump(hello,strlen(hello));

	// this is bugged.. why? - garbage values in hexdump **FIX - changed char to 
	// unsigned char to eliminate negative numbers
	printf("\ncurrent stack pointer is:%p\n",getsp());						
	printf("hex dumping where hello's pointer is located in the stack..\n");
	hexDump(&hello,0x100);

	// check the process id of this current process
	printf("\nthis hello world program is running on PID:%d\n",pid());
	
	struct hello *h;
	for(int i=0;i<3;i++){

		hexDump(h,sizeof(h));	
	}

	printf("dumping hello array\n");
	hexDump(helloArray, 10);
	
	// i.e this will cause an out of bounds, resulting on linux a segmentation fault which the os will create a core dump of the proccess
	//printf("%s",helloArray[10]);

//	for(int p =3;p>= 0; p--){
		printf("%s",helloArray[4]);
//	}
	printf("\n");
    return 0;
}
