/*
 * File: WorldLoader.h
 * Author: Benedict R. Gaster
 * Date: 26-06-2017
 *
 * Desc: Load Zombie world from JSON string and provide an interface to
 * the static representation of the world, including its map and items,
 * start location, finish location, and so on.
 *
 * Copyright: University of West of England 2017
 */

#pragma once

#include <string>

#include "ItemInfo.h"
#include "RoomInfo.h"
#include "EntranceInfo.h"

namespace uwe {
    /*! \class
     *  \brief class to load a world from a JSON file
     */
    class WorldLoader {
    private:
        // starting room
        std::string start_;
        std::string end_;
        std::string info_;

        std::string startHtml_;
        std::string inventoryHtml_;

        std::vector<ItemInfo> items_;
        std::vector<RoomInfo> rooms_;
    public:
        WorldLoader(std::string world);

        /**
         * starting room
         * @return room
         */
        std::string getStart() const {
            return start_;
        }

        /**
         * end room
         * @return room
         */
        std::string getEnd() const {
            return end_;
        }

        /**
         * game info
         * @return info
         */
        std::string getInfo() const {
            return info_;
        }

        /**
         * HTML for displaying with player's inventory
         * @return HTML for inventory image
         */
        std::string getInventoryHtml() const {
            return inventoryHtml_;
        }

        /**
         * HTML to display when client makes connection to server
         * @return html to display
         */
        std::string getStartHtml() const {
            return startHtml_;
        }

        /**
         * items contained within the world
         * @return list of items in world
         */
        std::vector<ItemInfo> getItems() const {
            return items_;
        }

        /**
         * items contained within the world
         * @return list of items in world
         */
        std::vector<RoomInfo> getRooms() const {
            return rooms_;
        }
    };
} // namespace uwe
