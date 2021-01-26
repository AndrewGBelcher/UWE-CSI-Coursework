#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

// init the stack
Stack::Stack(){
	s = 0;
	top = 0;
	stk = new double[s];		
};

Stack::Stack(int size){
	s = size;
	top = 0;
	stk = new double[s];		
	printf("using stack size:%d\n",s);
};

double Stack::pop(){

	if(empty()){
		printf("too many operators or empty stack\n"); // also too operators
		exit(STACK_UNDERFLOW);
		return -99.999;
	}
	else{
		return stk[--top];
	}		
}	

bool Stack::push(double value){

	if(full()){

		exit(STACK_OVERFLOW);		
		return false;
	}
	else{	

		stk[top++] = value;
		return true;
	}
}

bool Stack::empty(void){
	if(top < 0){
		return true;
	}
	else{
		return false;
	}
}

bool Stack::full(){
	if(top >= s){
		return true;
	}
	else{
		return false;
	}
}


void Stack::clear(){
	stk = new double[s];		
}

void Stack::dumpStack(int size){
	
		for(int i = 0; i < size; i++){
			DEBUG_PRINT("%d:\t%f |\n",i,stk[i]);	
		}
		DEBUG_PRINT("\n");
}




