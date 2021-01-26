/**
 * @file adc.c
 * @author Andrew Belcher
 * @date 25 Feb 2020
 * @brief Source file for analog to digital converter module for the MSP430FR5969
 */

#include <Events/event_types.h>
#include <Events/Event/event.h>
#include <Events/EventQueue/event_queue.h>
#include <Events/EventQueueManager/event_queue_manager.h>
#include <msp430.h>
#include "adc.h"


#ifndef ADC_C_
#define ADC_C_

volatile int adc_val;


/** @brief Initialises port 3 as well as the adc registers for
 * polling data from ecg probes
 *
 *  @param void
 *  @return void
 */
void adc_init(void){

    int i;

    for (i = 0; i < ADC_ARRAY_SIZE; i++){

        adc_array[i] = 0;
    }

    P3DIR  |= BIT0;
    P3REN  &= ~BIT0;
    P3OUT  |= BIT0;
    P3DIR  &= (0xFF ^ BIT0);
    P3SEL1 |= BIT1;
    P3SEL0 |= BIT1;

    // Configure ADC12
    ADC12CTL0 = ADC12SHT0_10 | ADC12SHT1_10 | ADC12MSC | ADC12ON;
    ADC12CTL1 = ADC12SHP | ADC12SHS_1 | ADC12CONSEQ_2 | ADC12DIV_7;
    ADC12CTL2 |= ADC12RES_2;                // 12-bit conversion results
    ADC12CTL3 |= 0x0C;
    ADC12MCTL12 = ADC12INCH_12 | ADC12WINC | ADC12VRSEL_0;
    ADC12IER0 |= ADC12IE12;                 // Enable ADC conv complete interrupt

    // Configure internal reference
    while(REFCTL0 & REFGENBUSY);            // If ref generator busy, WAIT

    REFCTL0 |= REFVSEL_2 |REFON;            // Select internal ref = 2.5V
                                            // Internal Reference ON
    while(!(REFCTL0 & REFGENRDY));          // Wait for reference generator
                                            // to settle

    __delay_cycles(5000);

    ADC12CTL0 |= ADC12ENC | ADC12SC;        // Start sampling/conversion

    __bis_SR_register(GIE);
    __no_operation();                       // For debugger

}

/** @brief Interrupt service routine for the 12 vector adc
 *  Sets adc_val to adc vector, adc_val will be used in the
 *  timer module and sampled periodically
 *
 *  @param void
 *  @return void
 */
#pragma vector = ADC12_VECTOR
__interrupt void adc_vector12_isr(void){

  switch(__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG)){

    case ADC12IV_ADC12IFG12:    // Vector 36:  ADC12MEM12

        adc_val = adc_array[adc_index++] = ADC12MEM12;

        if(adc_index >= ADC_ARRAY_SIZE){

            adc_index = 0;
        }

        __bis_SR_register(GIE);
        break;

    default:

        /* catch all other cases and do nothing */
        break;
  }
}

#endif /* ADC_C_ */
