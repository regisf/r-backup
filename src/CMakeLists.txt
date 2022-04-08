set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

list(REMOVE_ITEM ${SOURCES} main.cpp)

#target_include_directories(${BINARY} PUBLIC ${YAMLCPP_INCLUDE_DIRECTORIES})

add_library(${BINARY}_lib STATIC ${SOURCES})

add_executable(${BINARY} main.cpp)
# target_link_libraries(${BINARY}  ${BINARY}_lib  PRIVATE bfg::lyra spdlog::spdlog spdlog::spdlog_header_only yaml-cpp)

target_link_libraries(${BINARY}  ${BINARY}_lib ${YAMLCPP_LINK_LIBRARIES})
