/**
 * chat_broker.h
 * Created by [Rohit Kumar] on [12-Jul-2024].
 * Copyright © [2024] [Rohit Kumar]. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * */

#ifndef CHATBROKERSERVER_H
#define CHATBROKERSERVER_H

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <set>
#include <mutex>
#include <iostream>
#include <nlohmann/json.hpp>

class ChatServerBroker {
public:
    ChatServerBroker();
    void run(uint16_t port);

private:
    typedef websocketpp::server<websocketpp::config::asio> server;

    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);

    void handle_registration(websocketpp::connection_hdl hdl, std::string client_id);
    void handle_ping_connect(websocketpp::connection_hdl hdl, std::string client_id);
    void handle_messages(std::string from, std::string to, std::string text);

    server server_;
    std::map<std::string, websocketpp::connection_hdl> registered_clients_;
    std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> clients_;
    std::mutex client_mutex_;
};

#endif // CHATBROKERSERVER_H