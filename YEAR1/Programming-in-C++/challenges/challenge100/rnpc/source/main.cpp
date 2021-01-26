/*****************************************************************************
*				AUTHOR					     *
*				======					     *
* File: main.cpp							     *
* Author: Andrew Belcher						     *
* Date: 07-11-2017							     *
* Desc: Reverse Polish Calculator using a stack				     *
* Copyright: University of West of England 2017		             	     *
*									     *
*									     *
******************************************************************************/


/*****************************************************************************
*				INCLUDES				     *
*				========				     *
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <assert.h>

// include the UFCFGL301's standard library
#include <ufcfgl-30-1.h>

// our includes
#include "stack.h"
#include "expression.h"

/*****************************************************************************
*				NAMESPACES				     *
*			        ==========				     *
******************************************************************************/

using namespace uwe;

/*****************************************************************************
*				FUNCTIONS				     *
*			        =========				     *
******************************************************************************/

// help message function
void printHelp(void){

	printf("Usage: -f filename -s size\n");
	exit(NO_ERROR);

}

/*****************************************************************************
*				MAIN				     	     *
*			        ====				             *
******************************************************************************/

int main(int argc, char *argv[]) {
   
	int size = 50;// set the default stack size to be used when none is specified

	// rpn file option variables
	bool useFile = false; 
	char *filename;

	// use our expression contructor for checking our rpn expressions
	Expression ex;
	
	// check our arguments
	for(int a = 0; a < argc; a++){

		// detect if the user wants usage info
		if(!strcmp(argv[a], "-h") || !strcmp(argv[a], "-help")){
			printHelp();
		}

		// check if we are setting the stack size
		if(!strcmp(argv[a], "-s")){
			size = atoi(argv[a+1]);
			if(size < 2){
				printf("stack size is too small, needs to be more than 2\n");
				exit(STACK_TO_SMALL);
			}
			if(size > 1000){
				printf("stack size is too large, needs to be less than 1000\n");
				exit(STACK_TO_LARGE);
			}
			
		}

		// check if we are evaluating expression file
		if(!strcmp(argv[a], "-f")){
			
			filename = argv[a+1];
			
			// lets check if it exists before we use it
			if( access(filename, F_OK) != -1 ) {
				useFile = true;	
			} 
			else {
				printf("File doesnt exist\n");
				exit(NO_FILE);	
			}
		}
	}

	// collect the size if one is specified and create the stack with our constructor
	Stack s = Stack(size);

	// rpn file reading for our expression evaluation
	if(useFile){

		// read file into string
		std::string expFILE = uwe::readFile(filename);

		// duplicate string into char * so we can use the chkExp
		char *exp = strdup(expFILE.c_str());

		// Check the expression and evaluate
		printf("Result is:%f\n", ex.chkExp(s,exp));
	}
	
	// if not using a file we need user entry of rpn expression
	else{
		printf("Enter a rpn expression:");
		char *exp = getString();

		printf("Result is:%f\n", ex.chkExp(s,exp));
	}

	// DEBUG - dumping the stack to see how its filled
	s.dumpStack(size);		

	// clear the stack after we are done
	s.clear();

    return 0;
}
