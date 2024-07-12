// chat_client.cc
#include "chat_client.h"
#include <nlohmann/json.hpp>

ChatClient::ChatClient() {
    client_.init_asio();
    client_.set_open_handler(bind(&ChatClient::on_open, this, std::placeholders::_1));
    client_.set_message_handler(bind(&ChatClient::on_message, this, std::placeholders::_1, std::placeholders::_2));
}

void ChatClient::run() {
    websocketpp::lib::error_code ec;
    client::connection_ptr con = client_.get_connection(uri_, ec);
    if (ec) {
        std::cout << "Could not create connection because: " << ec.message() << std::endl;
        return;
    }

    hdl_ = con->get_handle();
    client_.connect(con);

    client_.run();
}

void ChatClient::send_message(const std::string& message) {
    websocketpp::lib::error_code ec;
    client_.send(hdl_, message, websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cout << "Send failed because: " << ec.message() << std::endl;
    }
}

void ChatClient::on_open(websocketpp::connection_hdl hdl) {
    std::cout << "Connected to server" << std::endl;
    //("Ping");
}

void ChatClient::on_message(websocketpp::connection_hdl hdl, client::message_ptr msg) {
    if(msg->get_payload().compare("pong")) {
        std::cout << "Connectio established" << std::endl;
    }
    else {
        try {
            nlohmann::json jsonData = nlohmann::json::parse(msg->get_payload());
            // Access fields
            if (jsonData.contains("receive_message")) {
                std::string from = jsonData["receive_message"]["from"];
                std::string message = jsonData["receive_message"]["message"];
                onMessageReceived_(from, message);
            }
            else {
                std::cout << "**** debug got some message : " << msg->get_payload() << std::endl;
            } 
        } catch (const std::exception& e) {
            std::cerr << "Parse error: " << e.what() << std::endl;
        }
    }    
}

