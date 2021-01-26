/*
 * File: ZombieServer.cpp
 * Author: Benedict R. Gaster
 * Date: 26-06-2017
 *
 * Desc: Class for interacting with the frontend, using websockets
 *
 * Copyright: University of West of England 2017
 */

#include <iostream>

#include "ZombieServer.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::placeholders::_3;
using websocketpp::lib::bind;

using namespace std;

namespace uwe {

// private members

void ZombieServer::onMessage(
    websocketpp::connection_hdl hdl,
    message_ptr msg) {

    std::string msgReceived = msg->get_payload();

    std::vector<std::string> msgsSend = zbot_.processCmd(msgReceived);

    // now send responses
    try {
            // first send score
            zserver_.send(
                hdl,
                scoreMsg(zbot_.currentScore()),
                msg->get_opcode());

            // send any text to display
            for (auto& str : msgsSend) {
                zserver_.send(hdl, outputMsg(str), msg->get_opcode());
            }

            // finally do we need to enable/disable zombies
            if (zbot_.enableTimer()) {
                zserver_.send(hdl, timerMsg(5), msg->get_opcode());
            }
            else if (zbot_.disableTimer()) {
                zserver_.send(hdl, timerMsg(0), msg->get_opcode());
            }
    } catch (const websocketpp::lib::error_code& e) {
            std::cout << "Failed to respond because: " << e
                      << "(" << e.message() << ")" << std::endl;
    }

    // check if server should quit
    if (zbot_.shouldQuit()) {
        zserver_.stop_listening();
    }
}

// public members

ZombieServer::ZombieServer(uint16_t port, ZombieBot& zbot) :
    port_(port),
    zbot_(zbot) {
}

void ZombieServer::run() {
    try {
        // Set logging settings
        zserver_.set_access_channels(websocketpp::log::alevel::all);
        zserver_.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        zserver_.init_asio();

        // Register our message handler
        zserver_.set_message_handler(
            bind(&ZombieServer::onMessage, this,::_1,::_2));

        // Listen on port 9002
        cout << "Waiting on port " << port_ << endl;
        zserver_.listen(port_);

        // Start the server accept loop
        zserver_.start_accept();

        // Start the ASIO io_service run loop
        zserver_.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}

} // namespace uwe
