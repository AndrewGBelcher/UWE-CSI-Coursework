
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "expression.h"

// function to apply arithmetic on stack depending on what operator is used
double Expression::eval(Stack s, char *op){

	// pop 2nd number first because stack is FILO
	double tmp2 = s.pop();
	double tmp1 = s.pop();
	double result;

	// detect alternate but correct operator characters and convert to our standard
	if(*op == 'x'){
		*op = '*';
	}
	
	// check the current operator and apply the arithmetic to the stack
	switch(*op){
		case '+':
			result = tmp1 + tmp2;
			printf("%.2f + %.2f = %.2f\n",tmp1,tmp2,result);
			break;
		case '-':
			result = tmp1 - tmp2;
			printf("%.2f - %.2f = %.2f\n",tmp1,tmp2,result);
			break;
		case '*':
			result = tmp1 * tmp2;
			printf("%.2f x %.2f = %.2f\n",tmp1,tmp2,result);
			break;
		case '/':
			if(tmp1 == 0 && tmp2 == 0){
				printf("Dont Divide by zero foo!\n");
				exit(DIVIDED_BY_ZERO);		
			}
			result = tmp1 / tmp2;
			printf("%.2f / %.2f = %.2f\n",tmp1,tmp2,result);
			break;
		default:
			printf("invalid operand:%c\n", *op);
			exit(INVALID_OPERAND);
	}
	s.push(result);
	// not sure why, but not poping after pushing the result
	// makes it so we cant evaluate the next number
	s.pop();
	
	return result;
	
}

// function to check our expression numbers
bool Expression::checkNum(char* exp){

	if(*exp < '0' || *exp > '9'){
		//DEBUG_PRINT("not a number\n");
		return false;
	}
	return true;
}

// determines whether the expression is using a operand or operator
double Expression::chkExp(Stack s, char *exp){

	double n;
	double result;
	char *newExp = strdup(exp);

	while(*newExp != '\0'){
		if(*newExp != ' ' && *newExp != '\n'){
			if((*newExp == '+') || (*newExp == 'x') || (*newExp == '*') || (*newExp == '-') || (*newExp == '/')){

				result = eval(s,newExp);
				s.pop();
			}
			else{
				if(checkNum(newExp)){
					double n = atof(newExp);
					DEBUG_PRINT("%.2f\n",n);
					s.push(n);
				}
				else{
					printf("invalid operand:%c\n", *newExp);
					exit(INVALID_OPERAND);
				}
				while(*newExp != ' ' && *newExp != '\n'){
					newExp++;
				}
			}
		}
		*newExp++;	
	}
	return(result);
}

