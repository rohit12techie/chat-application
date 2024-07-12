# client/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(chatclient)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED system)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/chat-client)

add_executable(chatclient chat_client_main.cc chat_app.cc chat_client.cc)
target_link_libraries(chatclient ${Boost_LIBRARIES} pthread)
