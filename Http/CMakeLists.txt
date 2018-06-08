cmake_minimum_required(VERSION 2.8.12)

project(Http)
find_package(Qt5Core REQUIRED)
find_package(Qt5Test REQUIRED)

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

#Tests =================
set(TEST_SOURCE
        src/test/HttpTest.cpp
        src/test/HttpTest.h
        src/test/testMain.cpp
)

add_executable(http_test ${TEST_SOURCE})
target_link_libraries(http_test Qt5::Test Qt5::Core Utils SmartLogger Provisioning Utils Http DownloadManager)
add_test(NAME http_test COMMAND http_test)
