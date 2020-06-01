# include jsoncpp library
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/lib)

file(GLOB SOURCES *.cpp *.h)
add_library(json_util ${SOURCES})
target_link_libraries(json_util jsoncpp)
target_include_directories(json_util PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
