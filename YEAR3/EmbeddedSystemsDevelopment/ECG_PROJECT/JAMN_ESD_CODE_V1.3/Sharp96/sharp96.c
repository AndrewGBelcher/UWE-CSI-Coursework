/**
 * @file sharp96.h
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Source file for the SHARP96 display for the MSP430FR5969
 */

#include <msp430.h>
#include <Sharp96/sharp96.h>
#include <util.h>

#ifndef SHARP96_C_
#define SHARP96_C_


/** @brief Initialises the SHARP96 display with port configurations
 *
 *  @param void
 *  @return void
 */
void sharp96_init(void){

    // LCD
    P4DIR |= 0x04;      // Set P4.2 to output direction (LCD Power On)
    P4DIR |= 0x08;      // Set P4.3 to output direction (LCD Enable)

    // SPI Ports
    P1SEL0 &= ~0x40;    // Set P1.6 to output direction (SPI MOSI)
    P1SEL1 |= 0x40;     // Set P1.6 to output direction (SPI MOSI)
    P1DIR |= 0x40;      // Set P1.6 to output direction (SPI MOSI)
    P2SEL0 &= ~0x04;    // Set P2.2 to SPI mode (SPI CLK)
    P2SEL1 |= 0x04;     // Set P2.2 to SPI mode (SPI CLK)
    P2DIR |= 0x04;      // Set P2.2 to output direction (SPI CLK)
    P2DIR |= 0x10;      // Set P2.4 to output direction (SPI CS)

    // SPI Interface
    UCB0CTLW0 |= UCSWRST;
    UCB0CTLW0 &= ~(UCCKPH + UCCKPL + UC7BIT + UCMSB);
    UCB0CTLW0 &= ~(UCSSEL_3);
    UCB0CTLW0 |= UCSSEL__SMCLK;
    UCB0BRW = 8;

    UCB0CTLW0 |= (UCMSB + UCCKPH + 0x00 + UCMST + UCSYNC + UCMODE_0);
    UCB0CTLW0 &= ~(UCSWRST);
    P4OUT |= 0x04;      // Turn LCD Power On
    P4OUT |= 0x08;      // Enable LCD

}


/** @brief Sends the display buffer to the SHARP96 display via SPI
 *
 *  @param struct Display
 *  @return void
 */
void sharp96_output_display(struct Display* display){

    int line;
    int column;
    char command = 0x80;            // Write lines

    command = command ^ 0x40;       // VCOM bit
    P2OUT |= 0x10;                  // LCD CS high

    while (!(UCB0IFG & UCTXIFG));

    UCB0TXBUF = command;

    for (line=0; line<96; line++) {

        while (!(UCB0IFG & UCTXIFG));

        UCB0TXBUF = reverse(line + 1);

        for (column=0; column<12; column++) {

            while (!(UCB0IFG & UCTXIFG));

            UCB0TXBUF = display->displayBuffer[line][column];
        }

        while (!(UCB0IFG & UCTXIFG));

        UCB0TXBUF = 0x00;            // End of line signal

    }

    while (!(UCB0IFG & UCTXIFG));

    UCB0TXBUF = 0x00;               // End of block signal

    while ((UCB0STATW & UCBBUSY));

    __delay_cycles(8);              //Ensure a 2us min delay to meet the LCD's thSCS

    P2OUT &= ~0x10;                 // LCD CS low
}

#endif /* SHARP96_C_ */


