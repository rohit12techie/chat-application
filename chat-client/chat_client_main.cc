/**
 * chat_client_main.cc
 * Created by [Rohit Kumar] on [12-Jul-2024].
 * Copyright © [2024] [Rohit Kumar]. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * */

#include "chat_client.h"
#include <vector>
#include <nlohmann/json.hpp>

ChatBrokerClient* chatBrokerClient_ptr = nullptr;
std::string g_from;
std::string g_to;

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.clear();
}


void handle_ping(){
    std::cout << "On handle ping " << std::endl;
    auto form_message = [&](){
        nlohmann::json message_payload;
        std::string text;
        message_payload["message"]["from"] = g_from;
        message_payload["message"]["to"] = g_to;
        clearInputBuffer();
        std::cout << "Enter first message: ";
        std::getline(std::cin, text);
        message_payload["message"]["text"] = text;
        return message_payload.dump();
    };
    chatBrokerClient_ptr->send_message(form_message());
}

void handle_message(){
    std::cout << "On handle message " << std::endl;
    auto form_message = [&](){
        nlohmann::json message_payload;
        std::string text;
        message_payload["message"]["from"] = g_from;
        message_payload["message"]["to"] = g_to;
        clearInputBuffer();
        std::cout << "Enter next message: ";
        std::getline(std::cin, text);
        message_payload["message"]["text"] = text;
        return message_payload.dump();
    };
    chatBrokerClient_ptr->send_message(form_message());
}

void on_receive(std::string from, std::string message) {
    try{
        nlohmann::json jsonData = nlohmann::json::parse(message);
        if (jsonData.contains("ping") && jsonData["ping"].contains("response")) {
            std::string response = jsonData["ping"]["response"];
            std::cerr << "Got the ping message from server" << response << std::endl;
            handle_ping();
        }
        else if (jsonData.contains("message")) {
            std::string from = jsonData["message"]["from"];
            std::string message = jsonData["message"]["text"];
            std::cerr << "Message : " << message << std::endl;
            handle_message();
        }
        else {
            std::cerr << "**** debug got some message : " << message << std::endl;
        }
    }catch(std::exception& e) {
        std::cerr << "Exception while parsing the data : " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "Enter your ID and other's ID" << std::endl;
        std::exit(0);
    }
    g_from = argv[1];
    g_to = argv[2];
    std::cout << "from : " << g_from << " to : " << g_to << std::endl;
    try{
        chatBrokerClient_ptr = new ChatBrokerClient(g_from, on_receive);
        chatBrokerClient_ptr->connect();
    }
    catch (const std::exception& e) {
        std::cout << "ChatApp could not be started " << std::endl;
    }

    return 0;
}
