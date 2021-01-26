/**
 * @file menu.h
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Header file for the ECG menu for the MSP430FR5969
 */

#include <Display/display.h>

#ifndef MENU_H_
#define MENU_H_

#define NUM_OF_OPTIONS 4

struct Menu {
    char menu_index;
    char toggle_opt[NUM_OF_OPTIONS];
    int inc_opt[NUM_OF_OPTIONS];
};

/** @brief Writes the ECG menu to the display buffer
 *
 *  @param struct Menu
 *  @param struct Display
 *  @return void
 */
void menu_draw_screen(struct Menu* menu, struct Display* display);

#endif /* MENU_H_ */
