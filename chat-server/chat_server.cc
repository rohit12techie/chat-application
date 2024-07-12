// ChatServer.cpp
#include "chat_server.h"

ChatServer::ChatServer() {
    m_server.init_asio();
    m_server.set_open_handler(bind(&ChatServer::on_open, this, std::placeholders::_1));
    m_server.set_close_handler(bind(&ChatServer::on_close, this, std::placeholders::_1));
    m_server.set_message_handler(bind(&ChatServer::on_message, this, std::placeholders::_1, std::placeholders::_2));
}

void ChatServer::run(uint16_t port) {
    m_server.listen(port);
    m_server.start_accept();
    m_server.run();
}

void ChatServer::on_open(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(m_client_mutex);
    m_clients.insert(hdl);
    std::cout << "Client connected" << std::endl;
}

void ChatServer::on_close(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(m_client_mutex);
    m_clients.erase(hdl);
    std::cout << "Client disconnected" << std::endl;
}

void ChatServer::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::lock_guard<std::mutex> lock(m_client_mutex);
    std::cout << "Received message: " << msg->get_payload() << std::endl;
    std::string response = "Received : " + msg->get_payload();
    
    msg->set_payload(response);
    for (auto it : m_clients) {
        m_server.send(it, msg);
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


void ChatServer::handle_registration(websocketpp::connection_hdl hdl, std::string client_id) {

}

void ChatServer::handle_messages(std::string from, std::string to, std::string text) {

}