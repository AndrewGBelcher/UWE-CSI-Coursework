/**
 * @file serial.h
 * @author Andrew Belcher
 * @date 25 Feb 2020
 * @brief Header file for the serial module for the MSP430FR5969
 */

#ifndef SERIAL_H_
#define SERIAL_H_


extern int display_mode;
extern char rx_byte;



/** @brief Writes char to serial A0 TX register and sends it
 *
 *  @param char byte
 *  @return void
 */
void serial_write_char(char byte);



/** @brief Wrapper for sending bytes from a string over serial A0
 *
 *  @param char* str, the string to write to serial
 *  @return void
 */
void serial_write_str(char *str);




/** @brief Initialise serial A0 to 230400 baud and configure interrupts
 *
 *  @param void
 *  @return void
 */
void serial_init(void);



#endif /* SERIAL_H_ */

