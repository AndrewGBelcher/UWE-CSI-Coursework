/*
 * File: ZombieServer.h
 * Author: Benedict R. Gaster
 * Date: 26-06-2017
 *
 * Desc:
 *
 * Copyright: University of West of England 2017
 */

#pragma once

#include <iostream>
#include <string>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "ZombieBot.h"

typedef websocketpp::server<websocketpp::config::asio> server;

namespace uwe {
    /*! \class
     *  \brief class that communicates with the web front end,
     *  using websockets. An instance of the ZombieBot interface
     *  provided and used by the server to interact with the game engine.
     */
    class ZombieServer {
    private:
        server zserver_;
        uint16_t port_;
        ZombieBot& zbot_;

        // pull out the type of messages sent by our config
        typedef server::message_ptr message_ptr;

        /**
         * [onMessage description]
         * @param hdl [description]
         * @param msg [description]
         */
        void onMessage(
            websocketpp::connection_hdl hdl,
            message_ptr msg);

        /**
         * Generate JSON for output message
         * @param text to wrap in JSON
         * @return JSON
         */
        std::string outputMsg(std::string text) {
            return "{ \"type\" : \"output\", \"text\" : \"" + text + "\" }";
        }

        /**
         * Generate JSON for timer message
         * @param duration, if 0 disable timer, otherwise set it to positive int
         *                  for count down.
         * @return JSON
         */
        std::string timerMsg(int duration) {
            return "{ \"type\" : \"timer\", \"value\" : \"" +
                   std::to_string(duration) + "\" }";
        }

        /**
         * Generate JSON for updated score, which will be displayed in client.
         * @param score to be displayed
         * @return JSON
         */
        std::string scoreMsg(int score) {
            return "{ \"type\" : \"score\", \"value\" : " +
                   std::to_string(score) + " }";
        }
    public:
        /**
         * constructor
         * @param port to use for server
         */
        ZombieServer(uint16_t port, ZombieBot& zbot);

        /**
         * run server, loops until user quits or instance deleted
         */
        void run();
    };
}
