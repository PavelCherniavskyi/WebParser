cmake_minimum_required(VERSION 2.8.12)

project(Http)
find_package(Qt5Core REQUIRED)

set(SOURCE
        src/private/Http.cpp
        src/private/LibCurlConfig.cpp
        src/private/ProtocolSlave.cpp
        src/private/ProcessorSlave.cpp
        src/private/LibCurlConfig.h
        src/private/ProtocolSlave.h
        src/private/ProcessorSlave.h
        src/Http.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME} Qt5::Core curl)
target_include_directories(${PROJECT_NAME} INTERFACE src/)
