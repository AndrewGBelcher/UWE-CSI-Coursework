
#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <Graph/graph.h>

#define DISPLAY_MODE_HOME 0
#define DISPLAY_MODE_PULSE 1
#define DISPLAY_MODE_MENU 2

struct Display{

    int mode;
    char displayBuffer[96][12]; // Display buffer
};

/** @brief Clears the display buffer
 *
 *  @param struct Display pointer
 *  @return void
 */
void display_init(struct Display* display);


/** @brief Writes the home screen to the display buffer
 *
 *  @param struct Display pointer
 *  @return void
 */
void display_draw_home_screen(struct Display* display);


/** @brief Inverts the data in the display buffer
 *
 *  @param struct Display pointer
 *  @return void
 */
void display_invert(struct Display* display);

/** @brief Sets a pixel within the display buffer to 1
 *
 *  @param struct Display pointer
 *  @param int x, horizontal axis point
 *  @param int y, vertical axis point
 *  @return void
 */
void display_set_pixel(struct Display* display, int x, int y);

/** @brief Clears a pixel within the display buffer to 0
 *
 *  @param struct Display pointer
 *  @param int x, horizontal axis point
 *  @param int y, vertical axis point
 *  @return void
 */
void display_clear_pixel(struct Display* display, int x, int y);

/** @brief Writes a line within the display buffer
 *
 *  @param struct Display pointer
 *  @param int x1, start horizontal axis point
 *  @param int y1, start vertical axis point
 *  @param int x2, end horizontal axis point
 *  @param int y2, end vertical axis point
 *  @return void
 */
void display_draw_line(struct Display* display, int x1, int y1, int x2, int y2);


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
void display_draw_text(struct Display* display, char* str, char x, char y, char invert);

/** @brief Writes a heart icon to the display buffer
 *
 *  @param struct Display pointer
 *  @param int heart_sel, 0 for small heart, 1 for large heart
 *  @param char column, x axis
 *  @param char row, y axis
 *  @return void
 */
void display_draw_heart(struct Display* display, int heart_sel, char column, char row);


/** @brief Writes the graph to the display buffer
 *
 *  @param struct Display pointer
 *  @param struct Graph pointer
 *  @return void
 */
void display_draw_graph(struct Display* display, struct Graph* graph);


/** @brief Clears the graph window within the display buffer
 *
 *  @param struct Display pointer
 *  @return void
 */
void display_graph_init(struct Display* display);

/** @brief Conducts linear interpolation on several values
 *
 *  @param char a
 *  @param char b
 *  @param float f
 *  @return char, interpolated value
 */
char display_lerp(char a, char b, float f);




#endif /* DISPLAY_H_ */
