/**
 * chat_client_main.cc
 * Created by [Rohit Kumar] on [12-Jul-2024].
 * Copyright © [2024] [Rohit Kumar]. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * */

#include "chat_app.h"

#include <thread>
#include <vector>

void chat_options(ChatApp& chatapp, std::string to) {
    std::cout << "Chatting with " << to << std::endl;
    while(1) {
        std::cout << "Do you want to respond or wait " << std::endl;
        std::cout << "1. respond" << std::endl;
        std::cout << "2. wait" << std::endl;

        char ch;
        switch(ch) {
            case 1: chatapp.send_message(to);
                break;
            case 2: {
                while(1) {
                    if(chatapp.received_flag == true) {
                        chatapp.received_flag = false;
                        break;
                    }
                    std::cout << "Waiting message from : " << to << std::endl;
                    sleep(1);
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "Enter your client ID and to" << std::endl;
        std::exit(0);
    }
    std::string client = argv[0];
    std::string to = argv[1];
    std::cout << "client : " << client << std::endl;
    try{
        ChatApp chatapp(client);
        chatapp.register_me();
        chat_options(chatapp, to);
    }
    catch (const std::exception& e) {
        std::cout << "ChatApp could not be started " << std::endl;
    }

    return 0;
}
