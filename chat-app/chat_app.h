/**
 * chat_app.h
 * Created by [Rohit Kumar] on [12-Jul-2024].
 * Copyright © [2024] [Rohit Kumar]. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * */

#ifndef CHATAPP_H
#define CHATAPP_H

#include <iostream>
#include "chat_client.h"

class ChatApp {
  public:
    static void init(std::string mine, std::string other);
  private:
    void run();
    ChatApp(std::string app_client_id);
    ChatApp() = delete;
    //~ChatApp() = delete;
    static bool is_registered();
    void register_me();
    void send_message(const std::string& to);
    void receive_message(const std::string& from, const std::string& message);
    bool received_flag = false;
    std::string app_client_id_;
    std::unique_ptr<ChatBrokerClient> chatbrokerclient_Ptr;
    static bool registered_;
    std::vector<std::string> contact_;
};

#endif // CHATAPP_H
