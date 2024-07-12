// chat_broker_main.cpp
#include "chat_broker.h"

#include <thread>
#include <vector>

int main() {
    const uint16_t port = 9002;
    const std::string uri = "ws://localhost:9002";
    const int num_clients = 5;

    // Start the server
    ChatBroker broker;
    broker.run(port);
    
    return 0;
}
