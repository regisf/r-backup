cmake_minimum_required(VERSION 3.10)

set(PROJ_NAME r-backup)

project(${PROJ_NAME} VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)
find_package(PkgConfig REQUIRED) 

pkg_check_modules(YAML REQUIRED yaml-cpp)

include_directories(${YAML_INCLUDE_DIRS})

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
