# server/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(ChatServerBroker)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED system)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/chat-broker-server)

add_executable(ChatServerBroker chat_broker_server_main.cc chat_broker_server.cc)
target_link_libraries(ChatServerBroker ${Boost_LIBRARIES} pthread)
