#include <Display/display.h>
#include <Display/font.h>
#include <Graph/graph.h>
#include <stdlib.h>
#include <msp430.h>
#include <math.h>
#include <Sharp96/sharp96.h>
#include <util.h>

#ifndef DISPLAY_C_
#define DISPLAY_C_


/** @brief Clears the display buffer
 *
 *  @param struct Display pointer
 *  @return void
 */
void display_init(struct Display* display){

    int i;
    int j;

    for(i = 0; i < 96; i++){

        for(j = 0; j < 12; j++){

            display->displayBuffer[i][j] = 0xFF;
        }
    }
}


/** @brief Inverts the data in the display buffer
 *
 *  @param struct Display pointer
 *  @return void
 */
void display_invert(struct Display* display){

    int i;
    int j;

    for(i = 0; i < 96; i++){

        WDTCTL = WDTPW + WDTCNTCL + 3;

        for(j = 0; j < 12; j++){

            display->displayBuffer[i][j] = ~display->displayBuffer[i][j];
        }
    }
}


/** @brief Clears the graph window within the display buffer
 *
 *  @param struct Display pointer
 *  @return void
 */
void display_graph_init(struct Display* display){

    int i;
    int j;

    for(i = 35; i < 96; i++){

        for(j = 0; j < 12; j++){

            display->displayBuffer[i][j] = 0xFF;
        }
    }
}

/** @brief Conducts linear interpolation on several values
 *
 *  @param char a
 *  @param char b
 *  @param float f
 *  @return char, interpolated value
 */
char display_lerp(char a, char b, float f){

    return a + f * (b - a);
}



/** @brief Writes a line within the display buffer
 *
 *  @param struct Display pointer
 *  @param int x1, start horizontal axis point
 *  @param int y1, start vertical axis point
 *  @param int x2, end horizontal axis point
 *  @param int y2, end vertical axis point
 *  @return void
 */
void display_draw_line(struct Display* display, int x1, int y1, int x2, int y2){

    char dist = 30;
    int i;
    float amt;
    char x;
    char y;

    for(i = 0; i < dist; i++){

        WDTCTL = WDTPW + WDTCNTCL + 3;

        amt = (float)i/(float)dist;
        x = display_lerp(x1, x2, amt);
        y = display_lerp(y1, y2, amt);

        display_set_pixel(display, x, y);

    }
}


/** @brief Sets a pixel within the display buffer to 1
 *
 *  @param struct Display pointer
 *  @param int x, horizontal axis point
 *  @param int y, vertical axis point
 *  @return void
 */
void display_set_pixel(struct Display* display,  int x, int y){

    char xpos = (x/8);
    char xoffset = x - xpos*8;
    char offset_mask = 1 << (7-xoffset);
    char ypos = y;

    display->displayBuffer[ypos][xpos] &= ~(offset_mask);
}


/** @brief Clears a pixel within the display buffer to 0
 *
 *  @param struct Display pointer
 *  @param int x, horizontal axis point
 *  @param int y, vertical axis point
 *  @return void
 */
void display_clear_pixel(struct Display* display,  int x, int y){

    char xpos = (x/8);
    char xoffset = x - xpos*8;
    char offset_mask = 1 << (7-xoffset);
    char ypos = y;

    display->displayBuffer[ypos][xpos] |= (offset_mask);
}

/** @brief Writes a heart icon to the display buffer
 *
 *  @param struct Display pointer
 *  @param int heart_sel, 0 for small heart, 1 for large heart
 *  @param char column, x axis
 *  @param char row, y axis
 *  @return void
 */
void display_draw_heart(struct Display* display, int heart_sel, char column, char row){

    int i;
    char x = column;
    char y = row;
    char fontData;

    for (i=0; i<8; i++){

            if(heart_sel%2 == 0){

               fontData = ~large_heart[i];
            }

            else{

                fontData = ~small_heart[i];
            }

            display->displayBuffer[i+y][x] = reverse(fontData);
    }
}


/** @brief Writes a ascii text characters from a string to the display buffer
 *  allows inverting the display buffers data
 *
 *  @param struct Display pointer
 *  @param char pointer, the string to write
 *  @param char x, horizontal axis point
 *  @param char y, vertical axis point
 *  @param char invert, 0 for normal, 1 for inversion
 *  @return void
 */
void display_draw_text(struct Display* display, char* str, char column, char row, char invert){

    int i;
    int j;
    char index;
    int len = strlen(str);
    char fontData;
    char x = column;
    char y = row;

    for (i = 0; i < 8; i++){

        for(j = 0; j < len; j++){

            index = str[j];

            if(invert){

                fontData = font8x8_basic[index][i];
            }

            else{

                fontData = ~font8x8_basic[index][i];
            }

            display->displayBuffer[i+y][j+x] = reverse(fontData);
        }
    }
}


/** @brief Writes the home screen to the display buffer
 *
 *  @param struct Display pointer
 *  @return void
 */
void display_draw_home_screen(struct Display* display){

    display_draw_text(display, "JAMN", 4, 1, 0);
    display_draw_text(display, " ECG Device", 0, 10, 0);
    display_draw_text(display, "V1.03", 3, 20, 0);
    display_draw_text(display, "Menu | Start", 0, 88, 0);

}


/** @brief Writes the graph to the display buffer
 *
 *  @param struct Display pointer
 *  @param struct Graph pointer
 *  @return void
 */
void display_draw_graph(struct Display* display, struct Graph* graph){

    int step_width = 96 / graph->max_readings;
    int i;
    char current_x;
    char current_y;
    char next_x;
    char next_y;

    if(graph->reset_pending){

        display_graph_init(display);
        graph->reset_pending = 0;
    }

    for(i=graph->index_drawn; i<(graph->index_written-1); i++){

        current_x = i*step_width;
        current_y = graph->readings[i];
        next_x = (i+1)*step_width;
        next_y = graph->readings[i+1];

        display_draw_line(display, current_x, current_y+45, next_x, next_y+45);
    }

    graph->index_drawn = graph->index_written-1;
}


#endif /* DISPLAY_C_ */
