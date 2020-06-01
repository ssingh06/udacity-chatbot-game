add_subdirectory(json)

# This directory only contains header files for now, so we define the chatbot_util library as an INTERFACE library.
add_library(chatbot_util INTERFACE)

# Link the json_util library
target_link_libraries(chatbot_util INTERFACE json_util)
target_include_directories(chatbot_util INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
