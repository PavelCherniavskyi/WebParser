cmake_minimum_required(VERSION 2.8.12)

project(Http)
find_package(Qt5Core REQUIRED)

set(SOURCE
        src/private/Http.cpp
        src/private/LibCurlConfig.cpp
        src/private/ProtocolSlave.cpp
        src/private/ProcessorSlave.cpp
        src/private/ProcessorExecutor.cpp
        src/private/ProcessorMaster.cpp
        src/private/ProtocolMaster.cpp
        src/private/HttpJob.cpp
        src/private/LibCurlConfig.h
        src/private/ProtocolSlave.h
        src/private/ProcessorSlave.h
        src/private/ProcessorExecutor.h
        src/private/ProcessorMaster.h
        src/private/ProtocolMaster.h
        src/private/HttpJob.h
        src/Http.h
        src/HttpEnums.h
        src/HttpTypes.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME} Qt5::Core curl)
target_include_directories(${PROJECT_NAME} INTERFACE src/)
