cmake_minimum_required(VERSION 2.8.12)

project(SmartLogger)
find_package(Qt5Core REQUIRED)
find_package(Qt5Test REQUIRED)

file(STRINGS ../ProvData.xml provData)

set(DLT_ENABLE OFF)
foreach(data ${provData})
    string(FIND ${data} "dltenable" lineIndex)
    if(lineIndex GREATER 0)
        string(FIND ${data} "true" index)
        if(index GREATER 0)
            set(DLT_ENABLE ON)
            break()
        endif()
    endif()
endforeach()

message(STATUS "DLT Enabled: " ${DLT_ENABLE})

if(DLT_ENABLE)
    find_package(PkgConfig)
    pkg_check_modules(DLT REQUIRED automotive-dlt)
    add_definitions(-DDLT_ENABLE)
endif()

set(SOURCE
        src/SmartLogger.h
        src/private/SmartLogger.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME} Qt5::Core ${DLT_LIBRARIES})
include_directories(${DLT_INCLUDE_DIRS})
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
