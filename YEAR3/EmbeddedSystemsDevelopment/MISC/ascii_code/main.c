/***************************************************************************
* @file main.c
* @date 25/10/19
* @author Andrew Belcher
* @breif Core program for testing LCD display/leds/buttons
***************************************************************************/

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <msp430.h>

//---------------------------------------------------------------------------
// Defined Variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------

int dec = 0; // global for button
int sw = 0;
int heartbeat = 1;
int debounce = 0;

//---------------------------------------------------------------------------
// Interrupt Sevice Routines
//---------------------------------------------------------------------------

/**
 * Timer A intterupt serivce routine
 *
 * Drives the red led on every tick of 500ms
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)    // Timer0 A0 1ms interrupt service routine
{
    // Toggle the red LED 500ms on and 500ms off
    heartbeat++;

    if (heartbeat > 500)
    {
        heartbeat = 1;
        P4OUT ^= 0x40;        // Toggle P4.6 using exclusive-OR}}
    }

}


/**
 * Port 1 interrupt service routine
 *
 * Reads in Button 1 input
 *
 * Drives green led on press
 * Increments an integer on press
 * Switchs display screen on press
 */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{

    while(debounce < 20000)
    {
        debounce++;
    }

    if ((P1IN & 2)==0)
    {
        dec++;
        sw = ~sw;
        P1OUT ^= 1;               // Set P1.0 on using OR
    }
    debounce = 0;


    P1IFG &= ~2; // P2.1 IFG cleared
}




/**
 * Main function
 *
 * Must be kept alive with a loop and must init all hardware before the loop begins
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P4DIR |= 0x40;              // Set P4.0 to output direction
    TA0CCR0 =  1024;            // Count up to 1024
    TA0CCTL0 = 0x10;            // Enable counter interrupts, bit 4=1
    TA0CTL =  TASSEL_2 + MC_1;  // Timer A using subsystem master clock, SMCLK(1.1 MHz)
                                // and count UP to create a 1ms interrupt
    PM5CTL0 &= ~LOCKLPM5;

    // leds + button 1
    P1OUT |= 0x43;
    P1DIR |= 0x41;
    P1REN |= 0x02;

    // interrupt config for button 1
    P1IE |= 2;      // P1.1 interrupt enabled
    P1IES |= 2;     // P1.1 Hi/lo edge
    P1IFG &= ~2;    // P1.1 IFG cleared

    // LCD
    P4DIR  |= 0x0C;                 // Set P4.2/3 to output direction (LCD Enable + Power On)

    // SPI Ports
    P1SEL0 &= ~0x40;                // Set P1.6 to output direction (SPI MOSI)
    P1SEL1 |=  0x40;                // Set P1.6 to output direction (SPI MOSI)
    P1DIR  |=  0x40;                // Set P1.6 to output direction (SPI MOSI)
    P2SEL0 &= ~0x04;                // Set P2.2 to SPI mode (SPI CLK)
    P2SEL1 |=  0x04;                // Set P2.2 to SPI mode (SPI CLK)
    P2DIR  |=  0x04;                // Set P2.2 to output direction (SPI CLK)
    P2DIR  |=  0x10;                // Set P2.4 to output direction (SPI CS)


    // SPI Interface
    UCB0CTLW0 |= UCSWRST;
    UCB0CTLW0 &= ~(UCCKPH + UCCKPL + UC7BIT + UCMSB);
    UCB0CTLW0 &= ~(UCSSEL_3);
    UCB0CTLW0 |= UCSSEL__SMCLK;
    UCB0BRW    = 8;
    UCB0CTLW0 |= (UCMSB + UCCKPH + 0x00 + UCMST + UCSYNC + UCMODE_0);
    UCB0CTLW0 &= ~(UCSWRST);
    P4OUT |= 0x04;                  // Turn LCD Power On
    P4OUT |= 0x08;                  // Enable LCD
    P1OUT &= ~0x01;                 // Set P1.0 off (Green LED)

    _BIS_SR(GIE);                   // interrupts enabled

    // init any globals on reset
    dec = 0;

    while(1)
    {

        initDisplayBuffer(0);

        if(!sw)
        {
            char heart_arr[] = {128+32,128+32,128+32};

            lcd_write_text(0,0,(char*)&heart_arr);

            lcd_write_text(1,2,"dec:");
            lcd_write_dec(5,2,dec);

            lcd_write_text(1,4,"float:");
            lcd_write_float(7,4,3.14);

            lcd_write_text(3,6,"Hello\n\r\tworld!");
        }
        else
            lcd_write_fullascii();

        outputDisplayBuffer();
    }

}



