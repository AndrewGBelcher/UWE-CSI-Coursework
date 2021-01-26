/*
 * File: ufcfgk-30-1.h
 * Author: Benedict R. Gaster
 * Date: 9th June 2017
 * Desc:
 * Copyright: University of West of England 2017
 */

 /*! \file
  *  \brief Support library for ufcfgk-30-1.
  */
#pragma once

#include <string>
#include <fstream>
#include <streambuf>
#include <memory>
#include <unistd.h>

/*! \file
 *  \brief An easy-to-use library for drawing geometric shapes,
 *         such as points, lines, and circles. Support for
 *         displaying text and images, is also provided.
 */
namespace uwe {

    /**
     * read an integer from the command line
     * @return read integer
     */
    extern int getInt(void);

    /**
     * read an integer from a string
     * @param  str to read integer from
     * @return     read integer
     */
    extern int getInt(char * str);

    /**
     * read 1 character without echo
     * @return  [description]
     */
    extern char getch(void);

    /**
     * read 1 character with echo
     * @return  [description]
     */
    extern char getche(void);

    /**
     * [pause description]
     * @param  usec [description]
     * @return      [description]
     */
    extern int pause(useconds_t usec);

    /**
     * reads a string from stdin
     * @return character array (note will leak if not deleted!)
     */
    char * getString();

    /**
     * read complete contents of file into a string
     * @param  filename path/name of file to read
     * @return          string containing file
     */
    std::string readFile(std::string filename);

    /**
     * convert strings to a unique int, at compile time if possible
     * @param  str string to be converted
     * @param  h   height (used in create unique ID)
     * @return     ID for string
     */
    constexpr unsigned int str2int(const char* str, int h = 0){
        return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
    }
}
