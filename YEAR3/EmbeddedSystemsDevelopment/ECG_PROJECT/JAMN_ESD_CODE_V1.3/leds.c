/**
 * @file leds.c
 * @author Andrew Belcher
 * @date 25 Feb 2020
 * @brief Source file for the led module for the MSP430FR5969
 */

#include <msp430.h>
#include "leds.h"


#ifndef LEDS_C_
#define LEDS_C_


/** @brief Initialises LED0/1 for driving
 *  @param void
 *  @return void
 */
void leds_init(void){

    P1DIR |=  BIT0; // Set P1.0 to output direction
    P4DIR |=  BIT6; // Set P4.6 to output direction
    P1OUT &= ~BIT0; // Set P1.0 off (Green LED)
    P4OUT &= ~BIT6; // Set P4.6 off (Red LED)

}

#endif /* LEDS_C_ */
