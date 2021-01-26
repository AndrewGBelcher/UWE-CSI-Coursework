/*
 * File: ItemInfo.h
 * Author: Benedict R. Gaster
 * Date: 26-06-2017
 * Desc: Description of item in world
 * Copyright: University of West of England 2017
 */
#pragma once

#include <string>

namespace uwe {
    /*! \class
     *  \brief information about a given item
     */
    class ItemInfo {
    private:
        const std::string name_;
        const std::string html_;
    public:
        /**
         * constructor to create an item
         * @param name of item
         * @param html for displaying item in client
         */
         ItemInfo(std::string name, std::string html);

         /**
          * name of item
          * @return name
          */
         std::string getName() {
             return name_;
         }

         /**
          * html to display item in client
          * @return HTML
          */
         std::string getHtml() {
             return html_;
         }
    };
} // namespace uwe
