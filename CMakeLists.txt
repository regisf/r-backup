cmake_minimum_required(VERSION 3.10)

project(r-backup VERSION 0.1.0)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FindPkgConfig)
find_package(PkgConfig)

pkg_check_modules(YAMLCPP REQUIRED yaml-cpp)

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()

add_subdirectory(src)
add_subdirectory(tests)

