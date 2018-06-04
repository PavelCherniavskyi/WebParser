cmake_minimum_required(VERSION 2.8.12)

project(Utils)
find_package(Qt5Core REQUIRED)

set(SOURCE
        src/JobExecutor.h
        src/Job.h
        src/private/Job.cpp
        src/private/JobExecutor.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_include_directories(${PROJECT_NAME} INTERFACE src/)
