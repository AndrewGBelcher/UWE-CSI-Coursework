/**
 * @file timer.c
 * @author Andrew Belcher
 * @date 25 Feb 2020
 * @brief Source file for the timer module for the MSP430FR5969
 */

#include <msp430.h>
#include <event_manager.h>
#include <timer.h>
#include <adc.h>
#include <Events/event_types.h>
#include <Events/event_types.h>
#include <Events/Event/event.h>
#include <Events/EventQueue/event_queue.h>
#include <Events/EventQueueManager/event_queue_manager.h>

#ifndef TIMER_C_
#define TIMER_C_

int adc_reading;
unsigned int bpm;
unsigned int old_bpm;
int ms_tick;
int old_reading_count;

/** @brief Interrupt service routine for timer A0
 *  Used to drive adc sampling for ECG readings
 *
 *  @param void
 *  @return void
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_a0(void){

    // Time to take a reading
    adc_reading++;

    // If 50 ticks have gone by sample adc values
    if (adc_reading >= 150){

        event_queue_manager_push_event(manager, event_create(EVENT_ADC_DONE_READING));
        adc_reading = 1;
    }
}




/** @brief Interrupt service routine for timer B0
 *  Used for averaging BPM based on 1ms ticks
 *
 *  @param void
 *  @return void
 */
#pragma vector=TIMER0_B0_VECTOR
__interrupt void timer_b0(void){

    if(second_beat){

        bpm = (ms_tick*60)/1000;

        if(bpm > 50 && bpm < 200)
        {
            old_reading_count = 0;
            bpm = bpm;
            old_bpm = bpm;
        }
        else
        {
            old_reading_count++;
            bpm = old_bpm;
        }
        beat = 0;
        first_beat = 0;
        second_beat = 0;
        ms_tick = 0;
    }

    ms_tick++;
}




/** @brief Initialise timers
 *
 *  @param void
 *  @return void
 */
void timer_init(void){

    /* TIMER A */
    TA0CCR0 = 1023;                           // Count up to 1024
    TA0CCTL0 = 0x10;                          // Enable counter interrupts, bit 4=1
    TA0CCTL1 = OUTMOD_3;                      // TACCR1 set/reset
    TA0CCR1 = 1023;                           // TACCR1 PWM Duty Cycle
    TA0CTL = TASSEL_2 + MC_1;                 // Timer A using subsystem master clock, SMCLK(1.1 MHz)

    /* TIMER B */
    TB0CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
    TB0CCR0 = 8000;                           // 1ms tick
    TB0CTL = TBSSEL_2 | MC_1;                 // SMCLK, continuous mode

}


#endif /* TIMER_C_ */
