/**
 * @file menu.c
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Source file for the ECG menu for the MSP430FR5969
 */

#include <Display/display.h>
#include <Options/menu.h>
#include <stdio.h>
#include <stdlib.h>
#include <Sharp96/sharp96.h>
#include <util.h>

#ifndef MENU_C_
#define MENU_C_

/** @brief Writes the ECG menu to the display buffer
 *
 *  @param struct Menu
 *  @param struct Display
 *  @return void
 */
void menu_draw_screen(struct Menu* menu, struct Display* display){

    int i;
    char opt_str[15];

    display_init(display);

    for(i = 0; i<96; i++){

        display_set_pixel(display, i, 0);
    }

    display_draw_text(display, "    MENU    ", 0, 1, 1);

    sprintf(opt_str, " HBPM    %s", menu->toggle_opt[0] ? "ON " : "OFF" );
    display_draw_text(display, opt_str, 0, 10, menu->menu_index == 0);

    sprintf(opt_str, " GRAPH   %s", menu->toggle_opt[1] ? "ON " : "OFF" );
    display_draw_text(display, opt_str , 0, 18, menu->menu_index == 1);

    sprintf(opt_str, " BRIGHT   %d ", menu->inc_opt[0]);
    display_draw_text(display, opt_str, 0, 26, menu->menu_index == 2);

    display_draw_text(display, " EXIT       ", 0, 34, menu->menu_index == 3);

    sharp96_output_display(display);
}

#endif /* MENU_C_ */

