cmake_minimum_required(VERSION 2.8.12)

project(SmartLogger)
find_package(Qt5Core REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(PkgConfig)

pkg_check_modules(DLT REQUIRED automotive-dlt)

set(SOURCE
        src/SmartLogger.h
        src/private/SmartLogger.cpp
        ${DLT_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME} Qt5::Core ${DLT_LIBRARIES})
target_include_directories(${PROJECT_NAME} INTERFACE src/)

#Tests =================
set(TEST_SOURCE
        src/test/SmartLoggerTest.h
        src/test/SmartLoggerTest.cpp
        src/test/testMain.cpp
)

add_executable(logger_test ${TEST_SOURCE})
target_link_libraries(logger_test Qt5::Test Qt5::Core SmartLogger Provisioning)
add_test(NAME logger_test COMMAND logger_test)
