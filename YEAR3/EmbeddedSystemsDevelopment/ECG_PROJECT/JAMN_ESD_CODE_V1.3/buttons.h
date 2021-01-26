/**
 * @file buttons.h
 * @author Andrew Belcher
 * @date 25 Feb 2020
 * @brief Header file for the button module for the MSP430FR5969
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_


int debounce_0;
int debounce_1;


/** @brief Initialise button pins and interrupts
 *
 *  @param void
 *  @return void
 */
void buttons_init(void);


#endif /* BUTTONS_H_ */
