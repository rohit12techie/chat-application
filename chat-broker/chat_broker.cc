/**
 * chat_broker.cpp
 * Created by [Rohit Kumar] on [12-Jul-2024].
 * Copyright © [2024] [Rohit Kumar]. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * */

#include "chat_broker.h"

ChatServerBroker::ChatServerBroker() {
    server_.init_asio();
    server_.set_reuse_addr(true);
    server_.set_open_handler(bind(&ChatServerBroker::on_open, this, std::placeholders::_1));
    server_.set_close_handler(bind(&ChatServerBroker::on_close, this, std::placeholders::_1));
    server_.set_message_handler(bind(&ChatServerBroker::on_message, this, std::placeholders::_1, std::placeholders::_2));
}

void ChatServerBroker::run(uint16_t port) {
    server_.listen(port);
    server_.start_accept();
    server_.run();
}

void ChatServerBroker::on_open(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(client_mutex_);
    std::cout << "Client connected" << std::endl;
}

void ChatServerBroker::on_close(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(client_mutex_);
    clients_.erase(hdl);
    std::cout << "Client disconnected" << std::endl;
}

void ChatServerBroker::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::lock_guard<std::mutex> lock(client_mutex_);
    std::cout << "Received message: " << msg->get_payload() << std::endl;
    std::string message_data = msg->get_payload();
    try {
        nlohmann::json jsondata = nlohmann::json::parse(message_data);
        if (jsondata.contains("ping") && jsondata["ping"].contains("client_id")) {
            std::cout << "Got the ping message from client" << std::endl;
            std::string client_id = jsondata["ping"]["client_id"];
            std::cout << "Got the ping message from client == " << client_id << std::endl;
            handle_ping_connect(hdl, client_id);
        }
        else if (jsondata.contains("register") && jsondata["register"].contains("client_id")) {
            std::string client_id = jsondata["register"]["client_id"];
            handle_registration(hdl, client_id);

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

void ChatServerBroker::handle_registration(websocketpp::connection_hdl hdl, std::string client_id) {
    std::cout << "Registering client : " << client_id << std::endl;
    registered_clients_[client_id] = hdl;
}

void ChatServerBroker::handle_ping_connect(websocketpp::connection_hdl hdl, std::string client_id) {
    auto form_ping_response_payload = []() {
        nlohmann::json ping_response_payload;
        ping_response_payload["ping"]["response"]= "pong";
        return ping_response_payload.dump();
    };
    std::cout << "Client has come online : " << client_id << std::endl;
    registered_clients_[client_id] = hdl;
    server_.send(hdl, form_ping_response_payload(), websocketpp::frame::opcode::text);
}

void ChatServerBroker::handle_messages(std::string from, std::string to, std::string text) {
    auto form_message_payload = [&]() {
        nlohmann::json message_payload;
        message_payload["message"]["from"] = from;
        message_payload["message"]["to"] = to;
        message_payload["message"]["text"] = text;
        return message_payload.dump();
    };

    auto registerd_client = registered_clients_.find(to);
    if(registerd_client != registered_clients_.end()) {
        server_.send(registerd_client->second, form_message_payload(), websocketpp::frame::opcode::text);
    }
    else{
        std::cout << "Message to invalid client" << std::endl;
    }
}
