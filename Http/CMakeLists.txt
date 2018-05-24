cmake_minimum_required(VERSION 2.8.12)

project(Http)
find_package(Qt5Core REQUIRED)

set(SOURCE
        src/private/Http.cpp
        src/Http.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_include_directories(${PROJECT_NAME} INTERFACE src/)
