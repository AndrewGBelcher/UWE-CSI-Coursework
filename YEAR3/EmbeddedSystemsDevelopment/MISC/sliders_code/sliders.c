/***************************************************************************
* @file slider.c
* @date 12/12/19
* @author Andrew Belcher
* @breif Slider module code
*/
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "sliders.h"

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

/**
 * Slider initializer
 *
 * sets up ports and control register needed to use the sliders
 */
void initSliderHandler (void){
    /* Set relevant digital I/O pins for Capacitance Touch. */
    P1REN  |= BIT3;
    P1DIR  |= BIT3;
    P1OUT  &= ~BIT3;
    P1REN  |= BIT4;
    P1DIR  |= BIT4;
    P1OUT  &= ~BIT4;
    P1REN  |= BIT5;
    P1DIR  |= BIT5;
    P1OUT  &= ~BIT5;

    P3REN  |= BIT4;
    P3DIR  |= BIT4;
    P3OUT  &= ~BIT4;
    P3REN  |= BIT5;
    P3DIR  |= BIT5;
    P3OUT  &= ~BIT5;
    P3REN  |= BIT6;
    P3DIR  |= BIT6;
    P3OUT  &= ~BIT6;


    /* TA2 set as timer with RO as clock and TA3 as gated enable. CCIS is
       toggled between gnd and vcc.
	*/
    TA2CTL = TASSEL_3 + MC_2;

    /* TA3 set as gate. */
    TA3CCR0 = ACCUMULATION_CYCLES;
    TA3CTL = TASSEL_2 + MC_1;
    TA3CCTL0 = CCIE;

    /* Capacitive Touch. */
    CAPTIO0CTL = CAPTIOEN + (3<<4) + (4<<1);
}

/**
 * left slider handler
 *
 * reads in all 3 slider sensors to an array of integers
 *
 * cap_val_arr: an array of capacitance values for each slider 0 top, 1 middle, 2 bottom
 */
void left_slider_handler(int cap_val_arr[3]){

    /* Capture initialisation.
       Clear the capacitance counter, set the trigger input to GND.
       Clear gate flags.
	*/
    TA2CTL |= TACLR;
    TA2CCTL0 = CM_1+CCIS_2+CAP+SCS;
    TA3CTL &= ~CCIFG;
    TA3CTL |= TACLR;

    /* Put the CPU to sleep, for ACCUMULATION_CYCLES of SMCLK. */
    __bis_SR_register(LPM0_bits+GIE);

    /* Set capacitance counter trigger to Vcc. */
    TA2CCTL0 |= CCIS_3 ;

    /* Sense next element. */
    if (elementNumber >= 3)
    {
        elementNumber = 0;
    }


    cap_val_arr[elementNumber] = TA2CCR0;
    sprintf (left_cap_string, "%d", TA2CCR0);

    switch (elementNumber){
		case LEFT_UPPER:
			CAPTIO0CTL = CAPTIOEN + (3<<4) + (4<<1);
			break;

		case LEFT_CENTRE:
			CAPTIO0CTL = CAPTIOEN + (3<<4) + (5<<1);
			break;

		case LEFT_LOWER:
			CAPTIO0CTL = CAPTIOEN + (3<<4) + (6<<1);
			break;

		default:
			break;
	}
    elementNumber++;
}



/**
 * right slider handler
 *
 * reads in all 3 slider sensors to an array of integers
 *
 * cap_val_arr: an array of capacitance values for each slider 0 top, 1 middle, 2 bottom
 */
void right_slider_handler(int cap_val_arr[3]){

    /* Capture initialisation.
	   Clear the capacitance counter, set the trigger input to GND.
       Clear gate flags. */
    TA2CTL |= TACLR;
    TA2CCTL0 = CM_1+CCIS_2+CAP+SCS;
    TA3CTL &= ~CCIFG;
    TA3CTL |= TACLR;

    /* Put the CPU to sleep, for ACCUMULATION_CYCLES of SMCLK. */
    __bis_SR_register(LPM0_bits+GIE);

    /* Set capacitance counter trigger to Vcc. */
    TA2CCTL0 |= CCIS_3 ;

    /* Sense next element. */
    if (elementNumber >= 3)
    {
        elementNumber = 0;
    }

    cap_val_arr[elementNumber] = TA2CCR0;
    sprintf (right_cap_string, "%d", TA2CCR0);

    switch (elementNumber){

		case RIGHT_UPPER:
			CAPTIO0CTL = CAPTIOEN + (1<<4) + (5<<1);
			break;

		case RIGHT_CENTRE:
			CAPTIO0CTL = CAPTIOEN + (1<<4) + (4<<1);
			break;

		case RIGHT_LOWER:
			CAPTIO0CTL = CAPTIOEN + (1<<4) + (3<<1);
			break;

		default:
			break;
	}
    elementNumber++;
}

