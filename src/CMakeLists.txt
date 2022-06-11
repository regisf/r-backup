
set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

list(REMOVE_ITEM ${SOURCES} main.cpp)

add_library(${BINARY}_lib STATIC ${SOURCES})

add_executable(${BINARY} main.cpp)

target_link_libraries(${BINARY}  ${BINARY}_lib ${YAMLCPP_LINK_LIBRARIES})
