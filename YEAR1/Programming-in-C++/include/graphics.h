/*
 * File: graphics.h
 * Author: Benedict R. Gaster
 * Date: 8th June 2017
 * Desc: An easy-to-use library for drawing geometric shapes,
 *       such as points, lines, and circles.
 * Copyright: University of West of England 2017
 */

/*! \file
 *  \brief An easy-to-use library for drawing geometric shapes,
 *         such as points, lines, and circles. Support for
 *         displaying text and images, is also provided.
 *
 *         The graphics library is built using the excellent
 *         JUCE framework (https://juce.com/). JUCE is large and powerful
 *         framework, which relies heavely on advanced features of C++. This
 *         libary provides a simpler interface to some of JUCE's basic
 *         features, for graphics, but exposes some aspects when it makes
 *         sense.
 */
#pragma once

#include <string>
#include <limits.h>
#include <stdlib.h>

#include "graphics_internal.h"


// This is used to avoid internals leaking into user headers
// any header that is intended for internal use only has a #ifdef
// to check that it is not defined
#define UFCFGL301_GRAPHICS

/*! \namespace
 *  \brief namespace for all UFCFGL-30-1 library code
 *
 */
namespace uwe {
    typedef std::function<void (Graphics&)> graphics_func;
    typedef juce::KeyPress keyPress;

    /**
     * setup graphics context. no graphics should be preformed before calling
     * this function.
     * @param graphics_func callback function, which is called each frame to
     * paint onto the graphics context.
     */
    inline
    void initialiseGraphics(
        int windowWidth  = internal::WINDOW_WIDTH_DEFAULT,
        int windowHeight = internal::WINDOW_HEIGHT_DEFAULT) {

        internal::window_height = windowHeight;
        internal::window_width =  windowWidth;
    }

    /**
     * close the current graphics context
     */
    inline
    void shutDown(void) {

    }

    /**
     * loops while application window is active, calling paint callback to
     * draw to the component's window.
     *
     * @param paint_callback called when the screen is to be painted
     * @param keypress_callback called when a key is pressed, optional
     * parameter. if keypress_callback returns true, then the graphics
     * context will close and return control to the application.
     */
    template<class F>
    void loop(
        F paint_callback,
        internal::keypress_func keypress_callback =
            internal::keypress_func(internal::default_keypress_callback)) {

        internal::paint_callback = paint_callback;
        internal::keypress_callback = keypress_callback;

        char * argv[] = { (char*)"gui_thread" };
        internal::juce_init(1, argv);
    }

    /**
     * convert keyPress to a character
     * @param  keyPress representation of key pressed on keyboard
     * @return          character pressed on keyboard
     */
    inline
    char getTextCharacter (KeyPress keyPress) {
        return static_cast<char>(keyPress.getTextCharacter());
    }

    /**
     * convert keyPress to key scan code
     * @param  keyPress representation of key pressed on keyboard
     * @return          scan code of pressed key
     */
    inline
    int getKeyCode (KeyPress keyPress) {
        return keyPress.getKeyCode();
    }

    // drawing API

    /**
     * draws a line on the current graphics context
     *
     * @param start_x x start position of line
     * @param start_y y start position of line
     * @param end_x   x end position of line
     * @param end_y   y end position of line
     */
    inline
    void drawLine(float start_x, float start_y, float end_x, float end_y) {
        internal::graphics_context->drawLine(start_x, start_y, end_x, end_y);
    }

    /**
     * draws a rectangle on the current graphics context.
     * @param x             x position of top left hand corner
     * @param y             y position of top left hand corner
     * @param width         width of rectangle
     * @param height        height of rectangle
     * @param lineThickness optional line thickness of rectangle
     */
    inline
    void drawRect (float x, float y, float width, float height, float lineThickness=1) {
        internal::graphics_context->drawRect(
            x,
            y,
            width,
            height,
            lineThickness);
    }

    /**
     * draws a rectangle on the current graphics context,
     * which is filled with the current pen colour.
     * @param x      [description]
     * @param y      [description]
     * @param width  [description]
     * @param height [description]
     */
    inline
    void drawFilledRect(float x, float y, float width, float height) {
        internal::graphics_context->fillRect(
            x,
            y,
            width,
            height);
    }

    /**
     * draws a circle on the current graphics context.
     * @param x      centre x position of circle
     * @param y0     centre y position of circle
     * @param radius circle's radius
     */
    inline
    void drawCircle(float x, float y, float radius, float lineThickness=1) {
        internal::graphics_context->drawEllipse(
            x-radius,
            y-radius,
            radius * 2,
            radius * 2,
            lineThickness);
    }

    /**
     * draws a filled circle on the current graphics context,
     * using the current pen colour.
     * @param x      centre x position of circle
     * @param y0     centre y position of circle
     * @param radius circle's radius
     */
    inline
    void drawFilledCircle(float x, float y, int radius) {
        internal::graphics_context->fillEllipse(
            x-radius,
            y-radius,
            radius * 2,
            radius * 2);
    }

    /**
     * draw text on current graphics context, using the current
     * pen colour.
     * @param text   text to be displayed
     * @param x      top left x position to draw text
     * @param y      top right y position to draw text
     * @param width  max width of text
     * @param height max height of text
     */
    inline
    void drawText(
        const String & 	text,
        int 	x,
        int 	y,
        int 	width,
        int 	height) {
        internal::graphics_context->drawText(
            text,
            x,
            y,
            width,
            height,
            Justification::left);
    }

    inline
    void drawTextCenter(
        const String & 	text,
        int 	x,
        int 	y,
        int 	width,
        int 	height) {
        internal::graphics_context->drawText(
            text,
            x,
            y,
            width,
            height,
            Justification::centred);
    }

    /**
     * set pen colour
     * @param newColour colour to set pen
     */
    inline
    void setColour(Colour newColour) {
        internal::graphics_context->setColour(newColour);
    }

    /**
     * set the size of the font used to display text
     * @param size font size
     */
    inline
    void setFontSize(float size) {
        internal::graphics_context->setFont(size);
    }

    /**
     * fill the window with a given colour
     * @param backgroundColour colour to fill screen
     */
    inline
    void fillAll(Colour backgroundColour) {
        internal::graphics_context->fillAll(backgroundColour);
    }

    /**
     * fille the window with a given colour
     * @param red   valye of red channel
     * @param green value of green channel
     * @param blue  valye of blue channel
     */
    inline
    void fillAll(uint8 red, uint8 green, uint8 blue) {
        internal::graphics_context->fillAll(Colour{red, green, blue});
    }

    /**
     * set colour of pen
     * @param red   value of red channel
     * @param green value of green channel
     * @param blue  value of blue channel
     */
    inline
    void setColour(uint8 red, uint8 green, uint8 blue) {
        internal::graphics_context->setColour(Colour{red,green,blue});
    }

    /**
     * calculate the width of the graphics window
     * @return width
     */
    inline
    int getWindowWidth() {
        return internal::component->getWidth();
    }

    /**
     * calculate the height of the graphics window
     * @return height
     */
    inline
    int getWindowHeight() {
        return internal::component->getHeight();
    }

    /**
     * load and image into an image object from a file
     * @param  imageFile path and name of image file
     * @return           loaded image
     */
    inline
    Image loadImage(const char * imageFile) {
        char * full_path = realpath(imageFile, NULL);
        if ( full_path == nullptr) {
            printf("ERROR: loadImage file not found = %s\n", imageFile);
            exit(-1);
        }
        return ImageFileFormat::loadFrom(File(std::string(imageFile)));
    }

    /**
     * draw a previously loaded image onto current graphics context
     * @param image image to be displayed
     * @param x     top left x position to draw image
     * @param y     top left y position to draw image
     */
    inline
    void drawImageAt(const Image& image, int x, int y) {
        internal::graphics_context->drawImageAt(image, x, y);
    }
} // namespace uwe
