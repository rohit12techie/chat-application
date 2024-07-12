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
    ChatApp(std::string client_id);
    void register_me();
    void send_message(const std::string& to);
    void receive_message(const std::string& from, const std::string& message);
    bool received_flag = false;
  
  private:
    
    std::string client_id_;
    std::unique_ptr<ChatClient> smartptr_chatclient_;
};

#endif // CHATAPP_H
