/**
 * @file buttons.c
 * @author Andrew Belcher
 * @date 25 Feb 2020
 * @brief Source file for the button module for the MSP430FR5969
 */

#include <Events/event_types.h>
#include <Events/Event/event.h>
#include <Events/EventQueue/event_queue.h>
#include <Events/EventQueueManager/event_queue_manager.h>
#include <msp430.h>
#include "event_manager.h"
#include "buttons.h"

#ifndef BUTTONS_C_
#define BUTTONS_C_

int debounce_0;
int debounce_1;



/** @brief Initialise button pins and interrupts
 *
 *  @param void
 *  @return void
 */
void buttons_init(void){

    P4OUT |= BIT5;  // P4.5 enable
    P4DIR |= BIT5;  // P4.5 set as input
    P4REN |= BIT5;  // P4.5 resistor enable

    P4IE  |= BIT5;  // P4.5 interrupt enabled
    P4IES |= BIT5;  // P4.5 Hi/lo edge
    P4IFG &= ~BIT5; // P4.5 IFG cleared

    P1OUT |= BIT1;  // P1.1 enable
    P1DIR |= BIT1;  // P1.1 set as input
    P1REN |= BIT1;  // P1.1 resistor enable

    P1IE  |= BIT1;  // P1.1 interrupt enabled
    P1IES |= BIT1;  // P1.1 Hi/lo edge
    P1IFG &= ~BIT1; // P1.1 IFG cleared
}



/** @brief Interrupt service routine for port 4 button 0
 *
 *
 *  @param void
 *  @return void
 */
#pragma vector=PORT4_VECTOR
__interrupt void button_0_isr(void){

    debounce_0 = 0;

    while(debounce_0 < 30000){

        debounce_0++;
        WDTCTL = WDTPW + WDTCNTCL + 3; //Kick the watchdog
    }

    if ((P4IN & BIT5)==0){

        event_queue_manager_push_event(manager, event_create(EVENT_BUTTON0_PRESS));
    }

    debounce_0 = 0;

    P4IFG &= ~BIT5; // P2.1 IFG cleared
}



/** @brief Interrupt service routine for port 1 button 1
 *
 *
 *  @param void
 *  @return void
 */
#pragma vector=PORT1_VECTOR
__interrupt void button_1_isr(void){

    debounce_1 = 0;

    while(debounce_1 < 30000){

        debounce_1++;
        WDTCTL = WDTPW + WDTCNTCL + 3; //Kick the watchdog
    }

    if ((P1IN & BIT1) == 0){

        event_queue_manager_push_event(manager, event_create(EVENT_BUTTON1_PRESS));
    }

    debounce_1 = 0;

    P1IFG &= ~BIT1; // P2.1 IFG cleared
}


#endif /* BUTTONS_C_ */
