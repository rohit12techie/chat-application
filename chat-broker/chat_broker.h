// chat_broker.h
#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <set>
#include <mutex>
#include <iostream>
#include <nlohmann/json.hpp>

class ChatBroker {
public:
    ChatBroker();
    void run(uint16_t port);

private:
    typedef websocketpp::server<websocketpp::config::asio> server;

    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);

    void handle_registration(websocketpp::connection_hdl hdl, std::string client_id);
    void handle_messages(std::string from, std::string to, std::string text);

    server server_;
    std::map<std::string, websocketpp::connection_hdl> registered_clients_; 
    std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> clients_;
    std::mutex client_mutex_;
};

#endif // CHATSERVER_H