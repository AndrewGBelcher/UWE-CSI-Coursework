/*
 * File: graphics.h
 * Author: Benedict R. Gaster
 * Date: 8th June 2017
 * Desc: Internal definitions and so on for graphics API.
 * Copyright: University of West of England 2017
 */
#pragma once

// #include <thread>
// #include <atomic>
// #include <mutex>
// #include <condition_variable>

// #include <vector>
// #include <functional>

#include "juce/JuceHeader.h"

#if defined(UFCFGL301_GRAPHICS)
    #error ("UFCFGL301_GRAPHICS must not be defined for internal headers")
#endif

namespace internal {

    extern int juce_init(int argc, char* argv[]);

    typedef std::function<void (void)> graphics_func;
    typedef std::function<bool (juce::KeyPress)> keypress_func;


    /**
     * called from the component's (window) paint method
     */
    extern graphics_func paint_callback;
    extern keypress_func keypress_callback;
    extern Graphics * graphics_context;
    extern Component * component;

    static const int WINDOW_WIDTH_DEFAULT = 600;
    static const int WINDOW_HEIGHT_DEFAULT = 400;
    extern int window_width;
    extern int window_height;

    extern Image framebuffer;

    inline
    bool default_keypress_callback(juce::KeyPress keyCode) {
        return false;
    }

} // namespace internal
