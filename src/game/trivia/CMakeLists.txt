file(GLOB SOURCES *.cpp)
add_library(game_trivia ${SOURCES})

# classes inside trivia/ use methods from chatbot_util and chatbot_http
target_link_libraries(game_trivia PRIVATE chatbot_util chatbot_http)

target_include_directories(game_trivia PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
