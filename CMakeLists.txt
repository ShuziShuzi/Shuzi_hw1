cmake_minimum_required(VERSION 4.0)

project(main)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/inc)

file(GLOB SRC_LIST "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(main ${SRC_LIST})
