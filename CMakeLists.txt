cmake_minimum_required(VERSION 3.11.3)

#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread -ljsoncpp")
set(CMAKE_VERBOSE_MAKEFILE on)

project(Chatbot)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# include jsoncpp library
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/lib)

#add_subdirectory(src/util)
#add_subdirectory(src/http)

file(GLOB project_SRCS
        src/http/*.cpp
        src/http/restapi/*.cpp
        src/*.cpp
        src/graph/*.cpp
        src/util/json/*.cpp
        src/game/*.cpp
        src/game/trivia/*.cpp
        src/chat/*.cpp)
# src/util/json/parser.cpp src/util/json/parser.h src/graph/util/constants.h src/game/game.cpp src/game/game.h src/game/trivia/trivia.cpp src/game/trivia/trivia.h src/game/rest_api_game.cpp src/game/rest_api_game.h src/game/trivia/util/constants.h
add_executable(chatbot ${project_SRCS} src/http/util/http_constants.h src/chat/util/chat_constants.h src/chat/chat_metadata.cpp src/chat/chat_metadata.h src/manage/chatbot_manager.cpp src/manage/chatbot_manager.h src/manage/util/chatbot_manager_constants.h)
target_link_libraries(chatbot ${wxWidgets_LIBRARIES} curl jsoncpp)
target_include_directories(chatbot PRIVATE ${wxWidgets_INCLUDE_DIRS} )
