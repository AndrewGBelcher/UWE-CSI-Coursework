/*
 * File: RoomInfo.h
 * Author: Benedict R. Gaster
 * Date: 26-06-2017
 * Desc: Represents static information about a room in the world.
 * Copyright: University of West of England 2017
 */

#pragma once

#include <string>
#include <vector>
#include "EntranceInfo.h"

namespace uwe {
    /*! \class
     *  \brief information about a particular room
     */
    class RoomInfo {
    private:
        // name of room
        const std::string name_;

        // description of room, displayed when room is looked at for example
        const std::string description_;

        // list of entrances found in room
        const std::vector<EntranceInfo> entrances_;

           // list of initial items in room
        const std::vector<std::string> items_;

           // number of zombies in room at world create time
        const unsigned int zombieCount_;
    public:
        /**
         * constructor
         * @param name        room's name
         * @param desc        room's description
         * @param entrances   list of room's entrances
         * @param items       list of room's items
         * @param zombieCount number of zombies in room
         */
        RoomInfo(
            std::string name,
            std::string desc,
            std::vector<EntranceInfo> entrances,
            std::vector<std::string> items,
            unsigned int zombieCount);

        /**
         * room name
         * @return name
         */
        std::string getName() const {
           return name_;
        }

        /**
         * room description
         * @return description
         */
        std::string getDescription() const {
           return description_;
        }

       /**
        * room entrances
        * @return list of entrances
        */
       std::vector<EntranceInfo> getEntrances() {
           return entrances_;
       }

       /**
        * rooms items
        * @return list of items
        */
       std::vector<std::string> getItems() {
           return items_;
       }

       /**
        * rooms zombie count
        * @return number of zombies in room
        */
       unsigned int getZombieCount() {
           return zombieCount_;
       }
    };
} // namespace uwe
