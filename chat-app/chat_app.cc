/**
 * chat_app.cc
 * Created by [Rohit Kumar] on [12-Jul-2024].
 * Copyright © [2024] [Rohit Kumar]. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * */

#include "chat_app.h"
#include <iostream>
#include <nlohmann/json.hpp>

ChatApp::ChatApp(std::string app_client_id) : app_client_id_(app_client_id) {
    try{
        chatbrokerclient_Ptr = std::make_unique<ChatBrokerClient>(app_client_id_);
        std::cout <<"It should not blocked after this" <<std::endl;
        std::thread([&](){
            //ChatBrokerClient ChatBrokerClient(app_client_id);
            chatbrokerclient_Ptr->connect();
        }).detach();
        std::cout <<"Is it coming here" <<std::endl;
    }
    catch (const std::exception& e){
        std::cout << "Exception occurred : " << e.what() << std::endl;
    }
}

void ChatApp::run() {
    auto form_message_payload = [this](std::string msg) {
        nlohmann::json message_payload;
        message_payload["from"] = app_client_id_;
        message_payload["to"] = contact_[0];
        message_payload["msg"] = msg;
        return message_payload.dump();
    };

    std::string message = "Hello Client2";
    std::cout << "Sending message to client2" << std::endl;
    while(true) {
        //std::cout << "Enter your message: ";
        //std::getline(std::cin, message);
        std::cout << "You entered: " << message << std::endl;
        chatbrokerclient_Ptr->send_message(message);
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        std::cout << "what is happening " << std::endl;
    }
}

void ChatApp::init(std::string mine, std::string other){
    if(is_registered()) {
        std::string app_client_id_ = mine;
        //std::unique_ptr<ChatApp> chatApp_ptr = std::make_unique<ChatApp>(app_client_id_);
        ChatApp* chatApp_ptr = new ChatApp(app_client_id_);
        chatApp_ptr->contact_.push_back(other);
        chatApp_ptr->run();
    }
    else{
        std::cout << "Register the client first." << std::endl;
    }
}

bool ChatApp::is_registered(){
    return true;
}

void ChatApp::register_me() {
    try{
        chatbrokerclient_Ptr = std::unique_ptr<ChatBrokerClient>(new ChatBrokerClient(app_client_id_));
        chatbrokerclient_Ptr->connect();
        ChatBrokerClient chatbrokerclient;
        if(! is_registered()) {
            register_me();
        }
    }
    catch (const std::exception& e){
        std::cout << "Exception occurred : " << e.what() << std::endl;
    }
    auto register_payload = [&]() {
        nlohmann::json register_payload;
        register_payload["register"]["client_id"] = app_client_id_;
        return register_payload.dump();
    };
    chatbrokerclient_Ptr->send_message(register_payload());
}

void ChatApp::send_message(const std::string& to) {
    std::string msg;
    std::cout << "Enter messsage for " << to << std::endl;
    std::cin >> msg;

    auto form_message = [&]() {
        nlohmann::json message_payload;
        message_payload["from"] = app_client_id_;
        message_payload["to"] = to;
        message_payload["msg"] = msg;
        return message_payload.dump();
    };
    chatbrokerclient_Ptr->send_message(form_message());
}

void ChatApp::receive_message(const std::string& from, const std::string& msg) {
    std::cerr << "Message came from - " << from << std::endl;
    std::cerr << msg << std::endl;
    std::cerr << "*************************************************" << std::endl;
}



