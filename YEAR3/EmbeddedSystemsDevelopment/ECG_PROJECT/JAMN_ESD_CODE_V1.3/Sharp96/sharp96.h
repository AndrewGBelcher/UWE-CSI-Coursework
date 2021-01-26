/**
 * @file sharp96.h
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Header file for the SHARP96 display for the MSP430FR5969
 */

#include <Display/display.h>

#ifndef SHARP96_H_
#define SHARP96_H_

/** @brief Initialises the SHARP96 display with port configurations
 *
 *  @param void
 *  @return void
 */
void sharp96_init(void);

/** @brief Sends the display buffer to the SHARP96 display via SPI
 *
 *  @param struct Display
 *  @return void
 */
void sharp96_output_display(struct Display* display);

#endif /* SHARP96_H_ */
