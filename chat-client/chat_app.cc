// chat_app.cc

#include "chat_app.h"
#include <iostream>
#include <nlohmann/json.hpp>

ChatApp::ChatApp(std::string client_id) : client_id_(client_id) {
    std::thread([&]() {
        try{
            ChatClient *chatclient = new ChatClient();
            smartptr_chatclient_ = std::unique_ptr<ChatClient>(chatclient);
        }
        catch (const std::exception& e){
            std::cout << "Exception occurred : " << e.what() << std::endl;
        }
    }).detach();
}

void ChatApp::register_me() {
    auto register_payload = [&]() {
        nlohmann::json register_payload;
        register_payload["register"]["client_id"] = client_id_;
        return register_payload.dump();
    };
    smartptr_chatclient_->send_message(register_payload());
}

void ChatApp::send_message(const std::string& to) {
    std::string msg;
    std::cout << "Enter messsage for " << to << std::endl;
    std::cin >> msg;

    auto form_message = [&]() {
        nlohmann::json message_payload;
        message_payload["from"] = client_id_;
        message_payload["to"] = to;
        message_payload["msg"] = msg;
        return message_payload.dump();
    };
    smartptr_chatclient_->send_message(form_message()); 
}

void ChatApp::receive_message(const std::string& from, const std::string& msg) {
    std::cout << "Message came from - " << from << std::endl;
    std::cout << msg << std::endl;
    std::cout << "*************************************************" << std::endl;
    received_flag = true;
}



