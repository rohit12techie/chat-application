# server/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(chatserver)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED system)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/chat-server)

add_executable(chatserver chat_server_main.cc chat_server.cc)
target_link_libraries(chatserver ${Boost_LIBRARIES} pthread)
