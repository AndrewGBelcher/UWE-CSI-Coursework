/**
 * File: main.cpp
 * Author: andy
 * Date: 12-10-2017
 * Desc:
 * Copyright: University of West of England 2017
 */
#include <stdio.h>
#include <stdlib.h>
// include the UFCFGL301's standard library
#include <ufcfgl-30-1.h>

// uncomment if you want to use the graphics library
//#include <graphics.h>

using namespace uwe;

/**
 * main routine, implements ...
 * @return      success
 */
int main(void) {
    // your  code begins here

    char message[100], ch;
    int i, key;
    
    printf("Enter a message to encrypt: ");
    scanf("%s",&message);
    printf("Enter key: ");
    scanf("%d", &key);
    
    for(i = 0; message[i] != '\0'; ++i){
        ch = message[i];
	printf("char is:%s\n", &ch);
        
        if(ch >= 'a' && ch <= 'z'){
            ch = ch + key;
            
            if(ch > 'z'){
                ch = ch - 'z' + 'a' - 1;
            }
            
            message[i] = ch;
	   printf("message is:%s\n",&ch);
        }
        else if(ch >= 'A' && ch <= 'Z'){
            ch = ch + key;
            
            if(ch > 'Z'){
                ch = ch - 'Z' + 'A' - 1;
            }
            
            message[i] = ch;
          printf("message is:%s\n",&ch);

        }
    }
    
    printf("Encrypted message: %s\n", message);


    return 0;
}
