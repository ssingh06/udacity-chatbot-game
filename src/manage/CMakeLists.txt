file(GLOB SOURCES *.cpp)
add_library(chatbot_manage ${SOURCES})
target_include_directories(chatbot_manage PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
