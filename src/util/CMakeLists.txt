file(GLOB SOURCES *.cpp *.h)
add_library(message_queue ${SOURCES})
target_include_directories(message_queue PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})