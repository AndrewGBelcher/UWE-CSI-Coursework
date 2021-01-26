/**
 * @file boot.h
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Header file for the boot animation for the MSP430FR5969
 */

#include <Display/display.h>


#ifndef BOOT_H_
#define BOOT_H_



/** @brief Draws the boot animation with JAMN logo
 *
 *  @param struct Display
 *  @return void
 */
void boot_animation(struct Display* display);



#endif /* BOOT_H_ */
