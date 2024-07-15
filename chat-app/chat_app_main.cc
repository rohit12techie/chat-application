/**
 * chat_client_main.cc
 * Created by [Rohit Kumar] on [12-Jul-2024].
 * Copyright © [2024] [Rohit Kumar]. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * */

#include "chat_app.h"

#include <vector>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "Enter your ID and other's ID" << std::endl;
        std::exit(0);
    }
    std::string me = argv[1];
    std::string to = argv[2];
    std::cout << "me : " << me << std::endl;
    try{
        ChatApp::init(me, to);
    }
    catch (const std::exception& e) {
        std::cout << "ChatApp could not be started " << std::endl;
    }

    return 0;
}
