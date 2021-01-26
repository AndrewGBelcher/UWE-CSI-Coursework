/**
 * @file watchdog.c
 * @author Andrew Belcher
 * @date 25 Feb 2020
 * @brief Source file for the watchdog module for the MSP430FR5969
 */

#include <msp430.h>
#include "watchdog.h"

#ifndef WATCHDOG_C_
#define WATCHDOG_C_

int restarting;



/** @brief Interrupt service routine for the watchdog timer
 *  If entered a soft reset will occur
 *
 *  @param void
 *  @return void
 */
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void){

    WDTCTL = WDTPW + WDTCNTCL + 3; //Reset the watchdog
    P4OUT |=  0x40;     // Set P4.6 on  (Red LED)
    restarting = 1;
    main();
}


#endif /* WATCHDOG_C_ */


