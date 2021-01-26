/**
 * @file timer.h
 * @author Andrew Belcher
 * @date 25 Feb 2020
 * @brief Header file for the timer module for the MSP430FR5969
 */

#include <msp430.h>

#ifndef TIMER_H_
#define TIMER_H_



int heart_beat;
extern int adc_reading;
unsigned int first_beat;
unsigned int second_beat;
unsigned int beat;
volatile unsigned long beat_time;
extern unsigned int bpm;
extern int ms_tick;

unsigned int old_bpm;
extern int old_reading_count;


/** @brief Initialise timers
 *
 *  @param void
 *  @return void
 */
void timer_init(void);


#endif /* TIMER_H_ */
