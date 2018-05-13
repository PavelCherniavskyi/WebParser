cmake_minimum_required(VERSION 2.8.12)

project(SmartLogger)
find_package(Qt5Core REQUIRED)

set(SOURCE
        src/SmartLogger.h
        src/private/SmartLogger.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_include_directories(${PROJECT_NAME} INTERFACE .)

#Tests =================
set(TEST_SOURCE
        src/test/SmartLoggerTest.h
        src/test/SmartLoggerTest.cpp
)

add_executable(logger_test ${TEST_SOURCE})
target_link_libraries(logger_test ${PROJECT_NAME})
add_test(NAME logger_test COMMAND logger_test)
