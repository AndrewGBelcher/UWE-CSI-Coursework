/*
 * File: EntranceInfo.h
 * Author: Benedict R. Gaster
 * Date: 26-06-2017
 * Desc:
 * Copyright: University of West of England 2017
 */
#pragma once

#include <string>

namespace uwe {
    /*! \class
     *  \brief Information about a room's entrance
     */
    class EntranceInfo {
    private:
        const std::string direction_;
        const std::string to_;
        const bool locked_;
    public:
        /**
         * constructor to create info about an entrance
         * @param dir    direction entrance leads to
         * @param to     room extrance leads to
         * @param locked is the entrance locked, i.e. needs a key to open
         */
        EntranceInfo(std::string dir, std::string to, bool locked);

        /**
         * direction entrance is reachable by moving in this direction
         * @return direction
         */
        std::string getDirection() const {
            return direction_;
        }

        /**
         * room that is reached, when moving through entrance
         * @return room
         */
        std::string getTo() const {
            return to_;
        }

        /**
         * is room locked
         * @return true if tool is locked, otherwise entrance is open
         */
        bool isLocked() const {
            return locked_;
        }
    };

} // namespace uwe
