# client/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(ChatBrokerClient)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED system)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/chat-client)

add_executable(ChatBrokerClient chat_client_main.cc chat_client.cc)
target_link_libraries(ChatBrokerClient ${Boost_LIBRARIES} pthread)
