/**
 * @file util.h
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Header file for various utility functions
 */

#ifndef UTIL_H_
#define UTIL_H_



/** @brief reverses the bit order in a char datatype.
 *  Nessesary for the Sharp96 display I/O
 *
 *  @param char in_char
 *  @return char out_char
 */
char reverse(char in_char);

#endif /* UTIL_H_ */
