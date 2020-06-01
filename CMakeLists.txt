cmake_minimum_required(VERSION 3.11.3)

#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread -ljsoncpp")
set(CMAKE_VERBOSE_MAKEFILE on)

project(Chatbot)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_subdirectory(src/util)
add_subdirectory(src/http)
add_subdirectory(src/graph)
add_subdirectory(src/game)
add_subdirectory(src/manage)

file(GLOB project_SRCS
        src/*.cpp
        src/chat/*.cpp)

add_executable(chatbot ${project_SRCS} src/chat/util/chat_constants.h src/chat/chat_metadata.cpp src/chat/chat_metadata.h)
target_link_libraries(chatbot ${wxWidgets_LIBRARIES} chatbot_graph chatbot_game chatbot_manage)
target_include_directories(chatbot PRIVATE ${wxWidgets_INCLUDE_DIRS})
