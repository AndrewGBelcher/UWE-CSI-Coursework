/*
 * File: WorldLoader.cpp
 * Author: Benedict R. Gaster
 * Date: 26-06-2017
 *
 * Desc: Load Zombie world from JSON string and provide an interface to
 * the static representation of the world, including its map and items,
 * start location, finish location, and so on.
 *
 * Copyright: University of West of England 2017
 */
#include <json.hpp>

#include "WorldLoader.h"

using json = nlohmann::json;

using namespace std;

namespace uwe {

    // constructor parses JSON world and fills structures
    // for getters
    WorldLoader::WorldLoader(std::string world) {
        auto jworld = json::parse(world);

        start_ = jworld["start"].get<string>();
        end_   = jworld["finish"].get<string>();
        info_  = jworld["info"].get<string>();

        inventoryHtml_ = jworld["inventoryHtml"].get<string>();
        startHtml_     = jworld["startHtml"].get<string>();

        //cout << jworld["items"] << endl;

        // create items
        for (auto& item: jworld["items"].get<vector<json>>()) {
            items_.push_back(
                ItemInfo{
                    item["name"].get<string>(),
                    item["html"].get<string>()});
        }

        //finally create rooms
        for (auto& room: jworld["rooms"].get<json>()) {
            std::vector<EntranceInfo> entrances;
            std::vector<string> items;

            // create list of entrances for room
            for (auto& entrance: room["entrances"].get<json>()) {
                entrances.push_back(
                    EntranceInfo{
                            entrance["direction"].get<string>(),
                            entrance["to"].get<string>(),
                            entrance["locked"].get<bool>()});
            }

            // create list of items
            for (auto& item: room["items"].get<json>()) {
                items.push_back(item["item"].get<string>());
            }

            rooms_.push_back(
                RoomInfo{
                    room["name"].get<string>(),
                    room["description"].get<string>(),
                    entrances,
                    items,
                    room["zombies"].get<unsigned int>()});
        }
    }

} // namespace uwe
