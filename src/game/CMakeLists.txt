add_subdirectory(trivia)

file(GLOB SOURCES *.cpp)
add_library(chatbot_game ${SOURCES})

# Link the trivia_game library
target_link_libraries(chatbot_game PRIVATE game_trivia)

target_include_directories(chatbot_game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
