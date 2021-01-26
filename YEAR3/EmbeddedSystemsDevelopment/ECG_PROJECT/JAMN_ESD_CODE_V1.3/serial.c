/**
 * @file serial.c
 * @author Andrew Belcher
 * @date 25 Feb 2020
 * @brief Source file for the serial module for the MSP430FR5969
 */

#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <event_manager.h>
#include <Events/event_types.h>
#include <Events/event_types.h>
#include <Events/Event/event.h>
#include <Events/EventQueue/event_queue.h>
#include <Events/EventQueueManager/event_queue_manager.h>
#include <serial.h>

#ifndef SERIAL_C_
#define SERIAL_C_

char serial_cmd_checksum;
char rx_byte;


/** @brief Interrupt service routine for serial A0
 *  Detection of receiving a byte will trigger an event for the event manager
 *  to process
 *
 *  @param void
 *  @return void
 */
#pragma vector = USCI_A0_VECTOR
__interrupt void serial_a0_isr(void){

    __no_operation();

    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG)){

        case USCI_NONE:

            break;

        // Recieved a byte
        case USCI_UART_UCRXIFG:

            // Wait till the full byte has been loaded
            while(!(UCA0IFG&UCTXIFG));

            // Save the byte and push the event to the queue
            rx_byte = UCA0RXBUF;

            event_queue_manager_push_event(manager, event_create(EVENT_SERIAL_RECV_BYTE));

            break;

        case USCI_UART_UCTXIFG:

            break;

        case USCI_UART_UCSTTIFG:

            break;

        case USCI_UART_UCTXCPTIFG:

            break;

        default:

            /* catch incorrect case and do nothing */
            break;
    }
}



/** @brief Writes char to serial A0 TX register and sends it
 *
 *  @param char byte
 *  @return void
 */
void serial_write_char(char byte){

    // Wait for the serial TX to be ready to send
    while(!(UCA0IFG & UCTXIFG));

    UCA0TXBUF = byte;
}


/** @brief Wrapper for sending bytes from a string over serial A0
 *
 *  @param char* str, the string to write to serial
 *  @return void
 */
void serial_write_str(char *str){

    do{

        serial_write_char(*str);
    }

    while(*++str);
}


/** @brief Initialise serial A0 to 230400 baud and configure interrupts
 *
 *  @param void
 *  @return void
 */
void serial_init(void){

    // Configure GPIO
    P2SEL1 |= BIT0 | BIT1;                    // USCI_A0 UART operation
    P2SEL0 &= ~(BIT0 | BIT1);

    // Configure PJ.5 PJ.4 for external crystal oscillator
    PJSEL0 |= BIT4 | BIT5;                    // For XT1

    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
    UCA0CTL0=0x00;
    UCA0BR0 = 2;                              // 230400 baud
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_2 | 0xbb00;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

}

#endif /* SERIAL_C_ */

