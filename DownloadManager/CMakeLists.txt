cmake_minimum_required(VERSION 2.8.12)

project(DownloadManager)
find_package(Qt5Core REQUIRED)

set(SOURCE
        src/private/DownloadManager.cpp
        src/private/Parser.cpp
        src/private/Parser.h
        src/DownloadManager.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_include_directories(${PROJECT_NAME} INTERFACE src/)
