/**
 * chat_client.cc
 * Created by [Rohit Kumar] on [12-Jul-2024].
 * Copyright © [2024] [Rohit Kumar]. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * */

#include "chat_client.h"
#include <nlohmann/json.hpp>

ChatBrokerClient::ChatBrokerClient():connected_(false) {

}

ChatBrokerClient::ChatBrokerClient(std::string app_client, func_on_message_received on_received_msg)
                                    :app_client_(app_client),
                                    on_message_received_(on_received_msg),
                                    connected_(false) {
}

ChatBrokerClient::~ChatBrokerClient(){
    // if (client_thread_.joinable()) {
    //     ws_client_.stop();
    //     client_thread_.join();
    // }
}

void ChatBrokerClient::run(std::promise<void>& connection_promise) {

    ws_client_.set_access_channels(websocketpp::log::alevel::all);
    ws_client_.clear_access_channels(websocketpp::log::alevel::frame_header | websocketpp::log::alevel::frame_payload);

    try{
        ws_client_.init_asio();
        ws_client_.set_open_handler(bind(&ChatBrokerClient::on_open, this, std::placeholders::_1));
        ws_client_.set_message_handler(bind(&ChatBrokerClient::on_message, this, std::placeholders::_1, std::placeholders::_2));
        websocketpp::lib::error_code ec;
        ws_client::connection_ptr con = ws_client_.get_connection(uri_, ec);
        if(ec) {
            throw std::runtime_error(ec.message());
        }
        ws_client_.connect(con);
        ws_client_.run();
    } catch (...) {
        connection_promise.set_exception(std::current_exception());
    }
}

void ChatBrokerClient::connect() {
    std::promise<void> connection_promise;
    std::future<void> connection_future = connection_promise.get_future();
    std::async(std::launch::async, &ChatBrokerClient::run, this, std::ref(connection_promise));
    connection_future.get();
}

void ChatBrokerClient::on_open(websocketpp::connection_hdl hdl) {
    std::cout << "Connected to server" << std::endl;
    //std::unique_lock<std::mutex> lock(mutex_);
    hdl_ = hdl;
    connected_ = true;
    // cv_.notify_all();

    auto form_ping_payload = [&]() {
        nlohmann::json ping_payload;
        ping_payload["ping"]["client_id"]= app_client_;
        std::cout << "payload to send : " << ping_payload.dump() << std::endl;
        return ping_payload.dump();
    };
    send_message(form_ping_payload());
}

void ChatBrokerClient::send_message(const std::string& message) {
    //std::unique_lock<std::mutex> lock(mutex_);
    websocketpp::lib::error_code ec;
    ws_client_.send(hdl_, message, websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cout << "Send failed because: " << ec.message() << std::endl;
    }
}


void ChatBrokerClient::on_message(websocketpp::connection_hdl hdl, ws_client::message_ptr msg) {
    //std::unique_lock<std::mutex> lock(mutex_);
    try {
        nlohmann::json jsonData = nlohmann::json::parse(msg->get_payload());
        on_message_received_("server", msg->get_payload());

        // if (jsonData.contains("ping") && jsonData["ping"].contains("response")) {
        //     std::cout << "Got the ping message from client" << std::endl;
        //     std::string response = jsonData["ping"]["response"];
        //     std::cout << "Got the ping message from server" << response << std::endl;
        // }
        // else if (jsonData.contains("message")) {
        //     std::string from = jsonData["message"]["from"];
        //     std::string message = jsonData["message"]["message"];
        //     on_message_received_(from, message);
        // }
        // else {
        //     std::cout << "**** debug got some message : " << msg->get_payload() << std::endl;

        // }
    } catch (const std::exception& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
    }
}