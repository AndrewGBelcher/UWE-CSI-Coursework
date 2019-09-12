/*
 * File: RoomInfo.cpp
 * Author: Benedict R. Gaster
 * Date: 26-06-2017
 * Desc:
 * Copyright: University of West of England 2017
 */
#include "RoomInfo.h"

namespace uwe {

RoomInfo::RoomInfo(
    std::string name,
    std::string desc,
    std::vector<EntranceInfo> entrances,
    std::vector<std::string> items,
    unsigned int zombieCount) :
        name_(name),
        description_(desc),
        entrances_(entrances),
        items_(items),
        zombieCount_(zombieCount) {
}

} // namespace uwe
