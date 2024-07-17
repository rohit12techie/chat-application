/**
 * chat_client.h
 * Created by [Rohit Kumar] on [12-Jul-2024].
 * Copyright © [2024] [Rohit Kumar]. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * */
#ifndef CHATBROKERCLIENT_H
#define CHATBROKERCLIENT_H

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>

typedef websocketpp::client<websocketpp::config::asio_client> ws_client;
typedef std::function<void(std::string, std::string)> func_on_message_received;

class ChatBrokerClient {
  public:
    ChatBrokerClient(std::string app_client, func_on_message_received on_received_message);
    ChatBrokerClient();
    ~ChatBrokerClient();
    void connect();
    void send_message(const std::string& message);

  private:
    void on_open(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, ws_client::message_ptr msg);
    void run();
    func_on_message_received on_message_received_;

    ws_client ws_client_;
    websocketpp::connection_hdl hdl_;
    const std::string app_client_;
    const std::string uri_ = "ws://localhost:9002";
    bool connected_;
};

#endif // CHATBROKERCLIENT_H
