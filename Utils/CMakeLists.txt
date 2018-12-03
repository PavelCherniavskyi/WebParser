cmake_minimum_required(VERSION 2.8.12)

project(Utils)
find_package(Qt5Core REQUIRED)
find_package(Qt5Test REQUIRED)

set(SOURCE
        src/JobExecutor.h
        src/Job.h
        src/ExpressServer.h
        src/private/Job.cpp
        src/private/JobExecutor.cpp
        src/private/ExpressServer.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_include_directories(${PROJECT_NAME} INTERFACE src/)

# Add TestHttpServer files to project
file(GLOB_RECURSE TestHttpServerApps NodeJsExpress/*)
add_custom_target(TestHttpServerMain SOURCES ${TestHttpServerApps} )

#Tests =================
set(TEST_SOURCE
        src/test/UtilsTest.cpp
        src/test/UtilsTest.h
        src/test/testMain.cpp
)

add_executable(utils_test ${TEST_SOURCE})
target_link_libraries(utils_test Qt5::Test Qt5::Core curl Utils SmartLogger Provisioning)
add_test(NAME utils_test COMMAND utils_test)
