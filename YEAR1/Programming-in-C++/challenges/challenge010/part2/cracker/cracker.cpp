/*****************************************************************************
*				AUTHOR					     *
*				======					     *
* File: cracker.cpp							     *	
* Author: Andrew Belcher						     *
* Date: 13-10-2017						             *
* Desc: DES encryption cracker program with enc option			     *
* Copyright: University of West of England 2017			             *
*									     *
*									     *
******************************************************************************/


/*****************************************************************************
*				INCLUDES				     *
*				========				     *
******************************************************************************/


#include <stdio.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


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

// ERROR CODES
#define NO_ERROR 0
#define INVALID_ARGS -1
#define NOT_A_STRING -2
#define NO_FILE -3
#define NO_VALID_HASH -4
#define NOT_IN_RANGE -5

/*****************************************************************************
*				FUNCTIONS				     *
*			        =========				     *
******************************************************************************/

char alphaNum[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

char *stripSalt(char *hash){
	char *salt = NULL;
    	salt = (char*)calloc(11, sizeof(char));
	
	int p = 0;
	for(int d = 0; d<=1; d++){
		salt[p] = hash[d];
		p++;
	}

	return(salt);
}	


// function to encrypt message in DES
char *encrypt(char *key, char *salt){

	char *encMsg;
        encMsg  = crypt(key,salt);

        printf("your encrypted message is:%s\n",encMsg);

	return(crypt(key,salt));

}


// the standard basic brute force method for 8 char length
int crack(char *hash){


	char *salt = stripSalt(hash);
	printf("salt:%s\n",salt);

	char guess[8] = "\0\0\0\0\0\0\0";
			

	for(int a = 0; a < strlen(alphaNum)-1; a++){
	for(int b = 0; b < strlen(alphaNum)-1; b++){
	for(int c = 0; c < strlen(alphaNum)-1; c++){
	for(int d = 0; d < strlen(alphaNum)-1; d++){
	for(int e = 0; e < strlen(alphaNum)-1; e++){
	for(int f = 0; f < strlen(alphaNum)-1; f++){
	for(int g = 0; g < strlen(alphaNum)-1; g++){
	for(int h = 0; h < strlen(alphaNum)-1; h++){

		printf("%s\n",guess);
		if(!strcmp(crypt(guess,salt), hash)){// ! because strcmp returns 0/false on a match
			printf("found it!\n");
			printf("cracked word:%s\n",guess);
	
			exit(NO_ERROR);
		}

	guess[0] = alphaNum[h];
	}//h
	guess[1] = alphaNum[g];
	}//g				
	guess[2] = alphaNum[f];
	}//f
	guess[3] = alphaNum[e];
	}//e
	guess[4] = alphaNum[d];
	}//d
	guess[5] = alphaNum[c];
	}//c	
	guess[6] = alphaNum[b];
	}//b	
	guess[7] = alphaNum[a];
	}//a	

	
	printf("\ndidnt find the word - non alphanumeric? or longer than 8 chars :(\n");
	return(NOT_IN_RANGE);
}

// load a word list function and loop till word hashs matchs DES hash to be cracked
int crack_from_file(char *hash, char *file){
	
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen(file, "r");
	if (fp == NULL){
		printf("Cant open file :/\n");
		exit(NO_FILE);
	}

	else{

		while ((read = getline(&line, &len, fp)) != -1) {

			size_t len = strlen(line);
			if (line[len - 1] == '\n') {  
				line[len -1] = '\0';
			}

						
			if(!strcmp(crypt(line,stripSalt(hash)), hash)){
				printf("found it!\n");
				printf("cracked word:%s\n",line);
				
				exit(NO_ERROR);
			}
		}
	}
	printf("didnt find the word in this file :(\n");
 	fclose(fp);

	return(NO_VALID_HASH);
}

// help menu that is printed when there is an error or no args
int printHelp(void){

	printf("DESimator v1.0\n"
	"AUTHOR: Andrew Belcher\n\n"

	"Usage: DESimator [OPTIONS] [SALT/ENC_MESSAGE] [HASH/MESSAGE] [*FILE]\n\n"

	"Cracks DES hashes into their decrypted messages\n"
	"Offers ability to encrypt messages into DES with a provided salt\n\n"
	"\tExample: DESimator -B 0AL9JBQz1cMbw\n"
	"\tExample: DESimator -W 0AL9JBQz1cMbw ./wordlist.txt\n"
	"\tExample: DESimator -E 0A password\n\n"

	"OPTIONS:\n"
	"\t-B,  Cracks a DES hash through straight bruteforcing (very slow/guarenteed(*alphanum + below 8 char))\n"
	"\t-W,  Cracks a DES hash provided with a wordlist (faster/not-guarenteed)\n"
	"\t-E,  Encrypts a message in DES encryption\n\n");

		exit(INVALID_ARGS);
		
return 0;

}


int main(int argc, char ** argv) {

	// catch arguments first to avoid memory leak
	if(argc <  3 || argc > 5){

		printHelp();
		exit(INVALID_ARGS);
	}
	
	// using a wordlist
	if(!strcmp(argv[1],"-W")){
			
		crack_from_file(argv[2],argv[3]);

	}
	// using basic bruteforce method
	else if(!strcmp(argv[1],"-B")){
			
		crack(argv[2]);

	}
	// encrypting a message with supplied salt
	else if(!strcmp(argv[1],"-E")){
		char *salt = argv[2];
		char *message = argv[3];

		if(argv[3] == NULL){
			printf("ERROR: you need a message to encrypt\n\n");
			printHelp();
		}
		else if(strlen(argv[2]) < 3){
			printf("your encrypted message is:%s\n", crypt(argv[3],argv[2]));
		}
		else{
			printf("ERROR: Use a valid salt\n\n");
			printHelp();
		}
	}
	else{
		printHelp();

	}

    return NO_ERROR;



}
