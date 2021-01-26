/***************************************************************************
* @file lcd.c
* @date 25/10/19
* @author Andrew Belcher
* @breif Module code for the LCD
***************************************************************************/
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "lcd.h"

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

/**
 * Fix for the display data processing
 *
 * Reverses the bit order of the display input data
 * Since the Sharp display reads only the data in backwards
 */
char reverse(unsigned char inchar)
{
    unsigned char outchar;

    outchar = 0;

    if ((inchar & BIT0) > 0) outchar |= BIT7;
    if ((inchar & BIT1) > 0) outchar |= BIT6;
    if ((inchar & BIT2) > 0) outchar |= BIT5;
    if ((inchar & BIT3) > 0) outchar |= BIT4;
    if ((inchar & BIT4) > 0) outchar |= BIT3;
    if ((inchar & BIT5) > 0) outchar |= BIT2;
    if ((inchar & BIT6) > 0) outchar |= BIT1;
    if ((inchar & BIT7) > 0) outchar |= BIT0;

    return outchar;
}

/**
 * Display buffer wiper
 *
 * Clears the display buffer
 */
void initDisplayBuffer(char setting)
{
    int i;
    int j;

    for (i=0; i<96; i++)
    {
        for(j=0; j<12; j++)
        {
            DisplayBuffer[i][j] = setting;
        }
    }
}

/**
 * SPI Display Buffer Output handler
 *
 * sends the display buffer to the display over SPI
 */
void outputDisplayBuffer()
{
    int line;
    int column;
    char command = 0x80;            // Write lines

    command = command ^ 0x40;       // VCOM bit

    P2OUT |= 0x10;                  // LCD CS high

    while (!(UCB0IFG & UCTXIFG));
    UCB0TXBUF = command;

    for (line=0; line<96; line++)
    {
        while (!(UCB0IFG & UCTXIFG));
        UCB0TXBUF = reverse(line+1);

        for (column=0; column<12; column++)
        {
            while (!(UCB0IFG & UCTXIFG));
            UCB0TXBUF = DisplayBuffer[line][column];
        }

        while (!(UCB0IFG & UCTXIFG));
        UCB0TXBUF = 0x00;            // End of line signal
    }

    while (!(UCB0IFG & UCTXIFG));
    UCB0TXBUF = 0x00;               // End of block signal

    while ((UCB0STATW & UCBBUSY));

    __delay_cycles(8);              //Ensure a 2us min delay to meet the LCD's thSCS

    P2OUT &= ~0x10;                 // LCD CS low
}




/**
 * Integer to string converter
 *
 * Converts and displays a string on the display
 *
 * value: the number to convert
 * result: the output string
 * base: the number base i.e. decimal=10, hex=16
 */
void itoa(long unsigned int value, char* result, int base)
{
    // check that the base if valid
    if((base < 2) || (base > 36))
    {
        *result = '\0';
    }

    char* ptr = result;
    char** ptr1 = result;
    char* tmp_char;
    int tmp_value;

    do
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    }
    while (value);

    // Apply negative sign
    if(tmp_value < 0)
    {
        *ptr++ = '-';
        *ptr-- = '\0';
    }

    while(ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr = *ptr1;
        *ptr--;
        *ptr1 = tmp_char;
        *ptr1++;
    }

}

/**
 * LCD decimal handler
 *
 * Converts and displays a decimal number on the display
 *
 * x: the horizontal axis on display 0=left
 * y: the vertical axis on display 0=top
 * text: the decimal number to display
 */
void lcd_write_dec(int x, int y, int dec)
{
    int i;
    int j;
    y=y*8;


    char num_string[20];

    //itoa(dec, num_string, 5);
   // memset(num_string,0,10);

    itoa(dec, num_string, 10);
    int len = strlen(num_string);

    for(i=0; i<(dec%9)+1; i++)
    {
        char n = num_string[i];

        for(j=0; j<8; j++)
        {
            DisplayBuffer[j+y][x+i] = ~fontA[(n-32)*8+j];
        }
    }

    memset(&num_string,0,20);
}


/**
 * LCD float handler
 *
 * Converts and displays a float on the display
 *
 * x: the horizontal axis on display 0=left
 * y: the vertical axis on display 0=top
 * text: the float to display
 */
void lcd_write_float(int x, int y, float flt)
{
    int i;
    int j;
    y=y*8;

    char* num_string =  "3.14";
    int len = strlen(num_string);

    for(i=0; i<len; i++)
    {
        char n = num_string[i];
        for(j=0; j<8; j++)
        {

            DisplayBuffer[j+y][x+i] = ~fontA[(n-32)*8+j];
        }
    }
}

/**
 * LCD text handler
 *
 * Converts and displays a string on the display
 *
 * x: the horizontal axis on display 0=left
 * y: the vertical axis on display 0=top
 * text: the string to display
 */
void lcd_write_text(int x, int y, char* text)
{
    int i;
    int j;
    int x_index = x;
    int len = strlen(text);

    y=y*8;

    for(i=0; i<len; i++)
    {
        if(text[i]=='\n'||text[i]=='\r')
        {
            x_index=0;
            y=y+8;
        }
        else if(text[i]=='\t')
            x_index=x_index+3;

        else{

            for(j=0; j<8; j++)
            {
               DisplayBuffer[j+y][x_index] = ~fontA[(text[i]-32)*8+j];
            }
            x_index++;
        }
    }
}

/**
 * LCD full ascii writer
 *
 * Displays the full contents of the font set to the display
 */
void lcd_write_fullascii(void)
{
    int i;
    int j;
    int p;
    int x = 0;

    for(p=0; p<12; p++) // row
    {
        for(i=0; i<12; i++) // column
        {
            for(j=0; j<8; j++) // bit lines
            {
                DisplayBuffer[j+(p*8)][i] = ~fontA[x*8+j];
            }
            x++;
        }
    }
}
