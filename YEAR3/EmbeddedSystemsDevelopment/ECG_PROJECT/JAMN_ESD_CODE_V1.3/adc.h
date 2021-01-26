/**
 * @file adc.h
 * @author Andrew Belcher
 * @date 25 Feb 2020
 * @brief Header file for analog to digital converter module for the MSP430FR5969
 */

#ifndef ADC_H_
#define ADC_H_

#define ADC_ARRAY_SIZE 192

unsigned int adc_array [ADC_ARRAY_SIZE];
unsigned int adc_index;
extern volatile int adc_val;


/** @brief Initialises port 3 as well as the adc registers for
 * polling data from ecg probes
 *
 *  @param void
 *  @return void
 */
void adc_init(void);


#endif /* ADC_H_ */


