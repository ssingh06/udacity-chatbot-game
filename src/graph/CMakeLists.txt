file(GLOB SOURCES *.cpp)
add_library(chatbot_graph ${SOURCES})
target_include_directories(chatbot_graph PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
