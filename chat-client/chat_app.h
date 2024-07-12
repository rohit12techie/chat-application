// chat_app.h

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
