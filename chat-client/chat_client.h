// ChatClient.h
#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <thread>
#include <iostream>

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef std::function<void(std::string, std::string)> OnMessageReceived;

class ChatClient {
public:
    ChatClient();
    void run();
    void send_message(const std::string& message);

private:
    void on_open(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg);
    OnMessageReceived onMessageReceived_;
    client client_;
    websocketpp::connection_hdl hdl_;
    const uint16_t port = 9002;
    const std::string uri_ = "ws://localhost:9002";
};

#endif // CHATCLIENT_H
