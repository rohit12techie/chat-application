// chat_broker_main.cpp

/**
 * chat_broker_main.cpp
 * Created by [Rohit Kumar] on [12-Jul-2024].
 * Copyright © [2024] [Rohit Kumar]. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * */
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
