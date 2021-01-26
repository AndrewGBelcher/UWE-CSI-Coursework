/**
 * @file boot.c
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Source file for the boot animation for the MSP430FR5969
 */

#include <msp430.h>
#include <boot.h>
#include <Display/display.h>
#include <Sharp96/sharp96.h>


#ifndef BOOT_C_
#define BOOT_C_


const unsigned char jamn_logo[16][17] = {
    {7, 13, 4, 5, 2, 4, 3, 3, 3, 5, 8, 5, 0, 0, 0, 0, 0},
    {7, 13, 4, 6, 3, 4, 3, 3, 3, 5, 7, 4, 0, 0, 0, 0, 0},
    {5, 14, 4, 7, 3, 4, 3, 3, 3, 6, 6, 3, 0, 0, 0, 0, 0},
    {13,5,  4, 8, 3, 5, 2, 4, 3, 6, 5, 3, 0, 0, 0, 0, 0},
    {12,6,  4, 4, 1, 3, 3, 6, 1, 4, 3, 6, 5, 3, 0, 0, 0},
    {11,6,  4, 5, 1, 4, 2, 12,3, 6, 4, 3, 0, 0, 0, 0, 0},
    {11,5,  5, 4, 2, 4, 2, 3, 1, 4, 1, 3, 3, 7, 3, 3, 0},
    {10,6,  4, 5, 2, 4, 2, 3, 1, 4, 1, 3, 3, 8, 2, 3, 0},
    {10,6,  4, 5, 2, 4, 2, 3, 1, 4, 1, 3, 3, 8, 2, 3, 0},
    {9, 6,  4,12, 2, 3, 3, 2, 1, 3, 3, 3, 1, 9, 0, 0, 0},
    {1, 4,  4, 5, 5, 12, 2, 3, 4, 1, 1, 3, 3, 3, 2, 8,0},
    {2, 4,  2, 6, 4, 13, 2, 3, 4, 1, 1, 3, 3, 3, 3, 7,0},
    {3,10,  5, 4, 5, 4, 2, 3, 6, 3, 3, 3, 4, 6, 0, 0, 0},
    {4, 8,  5, 5, 6, 3, 2, 3, 6, 3, 3, 3, 5, 5, 0, 0, 0},
    {4, 8,  4, 5, 7, 3, 2, 3, 6, 3, 3, 3, 6, 5, 0, 0, 0},
    {5, 5,  4, 9, 2, 7, 1, 5, 4, 4, 2, 4, 6, 5, 0, 0, 0}
};


/** @brief Draws the boot animation with JAMN logo
 *
 *  @param struct Display
 *  @return void
 */
void boot_animation(struct Display* display){

    int x;
    int y;
    int r;
    int b;
    int p;
    int draw_state;
    int x_index;
    int bit_cnt;

        for(y = 0; y < 97; y = (y + 2)){

            WDTCTL = WDTPW + WDTCNTCL + 3;

            for(x=0+y; x<97-y; x++){

                display_set_pixel(display, x, y);
            }

            WDTCTL = WDTPW + WDTCNTCL + 3;

            for(x=0+y; x<97-y; x++){

                display_set_pixel(display, y, x);
            }

            WDTCTL = WDTPW + WDTCNTCL + 3;

            for(x=0+y; x<97-y; x++){

                display_set_pixel(display, x, 96-y);
            }

            WDTCTL = WDTPW + WDTCNTCL + 3;

            for(x=0+y; x<97-y; x++){

                display_set_pixel(display, 96-y, x);
            }

            WDTCTL = WDTPW + WDTCNTCL + 3;

            r = 10000;

            while(r--){

                WDTCTL = WDTPW + WDTCNTCL + 3;
            }

            sharp96_output_display(display);

        }

        draw_state = 0;

        for(y=0; y < 16; y++){

            x_index = 16;

            for(x = 0; x<16; x++){

                if(jamn_logo[y][x] != 0){

                    bit_cnt = jamn_logo[y][x];

                    if(draw_state == 1){

                        b = 0;

                        while(bit_cnt--){

                            display_set_pixel(display, x_index, y+38);

                            x_index++;
                        }
                    }

                    x_index = x_index + bit_cnt;
                }

                draw_state ^= 1;
            }
        }

        for(x=0; x<26; x++){

            WDTCTL = WDTPW + WDTCNTCL + 3;

            display_invert(display);

            sharp96_output_display(display);

            p = 5;

            while(p--){

                r = 10000;

                while(r--){

                    WDTCTL = WDTPW + WDTCNTCL + 3;
                }
            }

        }

        display_init(display);

        sharp96_output_display(display);
}



#endif /* BOOT_C_ */
