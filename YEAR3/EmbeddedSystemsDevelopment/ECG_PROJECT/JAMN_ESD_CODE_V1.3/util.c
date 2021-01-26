/**
 * @file util.c
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Source file for various utility functions
 *
 */

#include <msp430.h>
#include <util.h>


#ifndef UTIL_C_
#define UTIL_C_



/** @brief reverses the bit order in a char datatype.
 *  Nessesary for the Sharp96 display I/O
 *
 *  @param char in_char
 *  @return char out_char
 */
char reverse(char in_char){

    char out_char;

    out_char = 0;

    if ((in_char & BIT0) > 0){

        out_char |= BIT7;
    }

    if ((in_char & BIT1) > 0){

        out_char |= BIT6;
    }

    if ((in_char & BIT2) > 0){

        out_char |= BIT5;
    }

    if ((in_char & BIT3) > 0){

        out_char |= BIT4;
    }

    if ((in_char & BIT4) > 0){

        out_char |= BIT3;
    }

    if ((in_char & BIT5) > 0){

        out_char |= BIT2;
    }

    if ((in_char & BIT6) > 0){

        out_char |= BIT1;
    }

    if ((in_char & BIT7) > 0){

        out_char |= BIT0;
    }

    return out_char;
}

#endif /* UTIL_C_ */
