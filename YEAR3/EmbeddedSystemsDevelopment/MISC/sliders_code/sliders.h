/***************************************************************************
* @file sliders.h
* @date 25/10/19
* @author Andrew Belcher
* @breif Header file for the slider module
***************************************************************************/
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <msp430.h>

//---------------------------------------------------------------------------
// Defined Variables
//---------------------------------------------------------------------------
#define WORD unsigned int
#define ACCUMULATION_CYCLES 1000
#define NOOFELEMENTS    6
#define LEFT_UPPER      0
#define LEFT_CENTRE     1
#define LEFT_LOWER      2
#define RIGHT_UPPER     3
#define RIGHT_CENTRE    4
#define RIGHT_LOWER     5

typedef enum { false, true } bool;

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
unsigned int elementNumber;
int left_slide_capval;
int cap_vals[3];
char left_cap_string[12];
char right_cap_string[12];

//---------------------------------------------------------------------------
// Function Prototypes
//---------------------------------------------------------------------------
void initSliderHandler(void);
void left_slider_handler(int * cap_val_arr);
void right_slider_handler(int * cap_val_arr);
