/*
 * File: EntranceInfo.cpp
 * Author: Benedict R. Gaster
 * Date: 26-06-2017
 * Desc:
 * Copyright: University of West of England 2017
 */
#include "EntranceInfo.h"

namespace uwe {

EntranceInfo::EntranceInfo(std::string dir, std::string to, bool locked) :
    direction_(dir),
    to_(to),
    locked_(locked) {
}

} // namespace uwe
