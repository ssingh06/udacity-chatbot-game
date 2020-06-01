# curl related cmake changes - begin
add_definitions( -DCURL_STATICLIB )

set( CURL_LIBRARY ${CMAKE_SOURCE_DIR}/lib )
set( CURL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include )
find_package( CURL )

include_directories( ${CURL_INCLUDE_DIRS} )
link_directories( ${CURL_LIBRARIES} )
# curl related cmake changes - end

file(GLOB SOURCES
        *.cpp
        restapi/*.cpp)
add_library(chatbot_http ${SOURCES})

target_include_directories(chatbot_http PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(chatbot_http PRIVATE curl)
