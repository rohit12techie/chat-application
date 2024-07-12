/**
 * chat_broker.cpp
 * Created by [Rohit Kumar] on [12-Jul-2024].
 * Copyright © [2024] [Rohit Kumar]. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * */

#include "chat_broker.h"

ChatBroker::ChatBroker() {
    server_.init_asio();
    server_.set_open_handler(bind(&ChatBroker::on_open, this, std::placeholders::_1));
    server_.set_close_handler(bind(&ChatBroker::on_close, this, std::placeholders::_1));
    server_.set_message_handler(bind(&ChatBroker::on_message, this, std::placeholders::_1, std::placeholders::_2));
}

void ChatBroker::run(uint16_t port) {
    server_.listen(port);
    server_.start_accept();
    server_.run();
}

void ChatBroker::on_open(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(client_mutex_);
    clients_.insert(hdl);
    std::cout << "Client connected" << std::endl;
}

void ChatBroker::on_close(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(client_mutex_);
    clients_.erase(hdl);
    std::cout << "Client disconnected" << std::endl;
}

void ChatBroker::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::lock_guard<std::mutex> lock(client_mutex_);
    std::cout << "Received message: " << msg->get_payload() << std::endl;
    std::string response = "Received : " + msg->get_payload();
    
    msg->set_payload(response);
    for (auto it : clients_) {
        server_.send(it, msg);
    }

    try {
        auto jsondata = nlohmann::json::parse(msg->get_payload());
        if (jsondata.contains("register") && jsondata["register"].contains("client_id")) {
            std::string client_id = jsondata["register"]["client_id"];
            handle_registration(hdl, client_id);
            std::cout << "Registered client: " << client_id << std::endl;
        }
        else if (jsondata.contains("message") && 
                jsondata["message"].contains("from") && 
                jsondata["message"].contains("to") && 
                jsondata["message"].contains("text")) {
            std::string from = jsondata["message"]["from"];
            std::string to = jsondata["message"]["to"];
            std::string text = jsondata["message"]["text"];
            
            handle_messages(from, to, text);
        } else {
            std::cerr << "Invalid message structure or missing fields" << std::endl;
        }

    } catch (nlohmann::json::parse_error& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
    } catch (nlohmann::json::type_error& e) {
        std::cerr << "Type error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General Exception: " << e.what() << std::endl;
    }
}


void ChatBroker::handle_registration(websocketpp::connection_hdl hdl, std::string client_id) {

}

void ChatBroker::handle_messages(std::string from, std::string to, std::string text) {

}