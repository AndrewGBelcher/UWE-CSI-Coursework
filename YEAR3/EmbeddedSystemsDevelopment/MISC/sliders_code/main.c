/***************************************************************************
* @file main.c
* @date 12/12/19
* @author Andrew Belcher
* @breif Core program for testing Slider input
*/

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <msp430.h>
#include "stdio.h"
#include "string.h"

//---------------------------------------------------------------------------
// Defined Variables
//---------------------------------------------------------------------------
typedef enum { false, true } bool;

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
int heartbeat = 1; 
unsigned long per;
char per_string[11];
bool left_slider_pos[3];
bool left_highest;
bool left_middle;
bool left_lowest;
int left_cap_vals[3];
int right_cap_vals[3];
char left_cap[12];
char right_cap[12];
int i;

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
    /* Toggle the red LED 500ms on and 500ms off */
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
 * Timer A LPM interrupt service routine
 *
 * This ISR clears the LPM bits found in the Status Register (SR/R2)
 */
#pragma vector=TIMER3_A0_VECTOR
__interrupt void TIMER3_A0_ISR(void)

{
    __bic_SR_register_on_exit(LPM3_bits);           // Exit LPM3 on reti
}



/**
 * Main function
 *
 * Must be kept alive with a loop and must init all hardware before the loop begins
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
    P4DIR |= 0x40;              // Set P4.0 to output direction
    TA0CCR0 =  1024;            // Count up to 1024
    TA0CCTL0 = 0x10;            // Enable counter interrupts, bit 4=1
    TA0CTL =  TASSEL_2 + MC_1;  // Timer A using subsystem master clock, SMCLK(1.1 MHz)
                                // and count UP to create a 1ms interrupt
    PM5CTL0 &= ~LOCKLPM5;

    /* leds + button 1 */
    P1OUT |= 0x43;
    P1DIR |= 0x41;
    P1REN |= 0x02;

    /* interrupt config for button 1 */
    P1IE |= 2; // P1.1 interrupt enabled
    P1IES |= 2; // P1.1 Hi/lo edge
    P1IFG &= ~2; // P1.1 IFG cleared

    /* LCD */
    P4DIR  |= 0x0C;                 // Set P4.2/3 to output direction (LCD Enable + Power On)

    /* SPI Ports */
    P1SEL0 &= ~0x40;                // Set P1.6 to output direction (SPI MOSI)
    P1SEL1 |=  0x40;                // Set P1.6 to output direction (SPI MOSI)
    P1DIR  |=  0x40;                // Set P1.6 to output direction (SPI MOSI)
    P2SEL0 &= ~0x04;                // Set P2.2 to SPI mode (SPI CLK)
    P2SEL1 |=  0x04;                // Set P2.2 to SPI mode (SPI CLK)
    P2DIR  |=  0x04;                // Set P2.2 to output direction (SPI CLK)
    P2DIR  |=  0x10;                // Set P2.4 to output direction (SPI CS)


    /* SPI Interface */
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

    initDisplayBuffer(0);
    initSliderHandler();

	while(1)
	{
		/* read in slider values */
        left_slider_handler(left_cap_vals);
        right_slider_handler(right_cap_vals);
		
		/* for each slider value print it to the corresponding position on screen*/
        for(i = 0; i < 3; i++)
        {
            sprintf(left_cap,"%d",left_cap_vals[i]);
            sprintf(right_cap,"%d",right_cap_vals[i]);
            lcd_write_text(0, 2*(i+1),left_cap);
            lcd_write_text(6, 2*(i+1),right_cap);
        }
		

		/* position detection high middle or low */
        if(left_cap_vals[0]<1500)
        {
            left_highest = true;
        }
        else
		{
			left_highest = false;
        }

		if(left_cap_vals[1]<1680)
        {
            left_middle = true;
        }
        else
		{
            left_middle = false;
        }
		
		if(left_cap_vals[2]<1800)
        {
            left_lowest = true;
        }
        else
		{
            left_lowest = false;
		}

		/* handle screen output based on slider position */
        if(left_highest && left_middle && !left_lowest)
        {
            lcd_write_text(0,0,"HIGHEST  ");
            lcd_write_text(0,9,"100      ");
        }

        else if((left_highest && !left_lowest && !left_middle) || (left_highest && left_lowest && !left_middle))
		{
            lcd_write_text(0,0,"LOWEST   ");
            lcd_write_text(0,9,"0         ");
        }

        else if(left_middle && left_lowest)
        {
            lcd_write_text(0,0,"MIDDLE   ");
            lcd_write_text(0,9,"50        ");
        }

		/* no position detected */
        else
        {
            lcd_write_text(0,0,"NO PRESS ");
            lcd_write_text(0,9,"NA     ");
        }

		/* update the display */
        outputDisplayBuffer();
	}
}
