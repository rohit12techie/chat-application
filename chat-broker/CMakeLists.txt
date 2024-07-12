# server/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(chatbroker)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED system)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/chat-server)

add_executable(chatbroker chat_broker_main.cc chat_broker.cc)
target_link_libraries(chatbroker ${Boost_LIBRARIES} pthread)
