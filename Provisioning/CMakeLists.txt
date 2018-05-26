cmake_minimum_required(VERSION 2.8.12)

project(Provisioning)
find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5XmlPatterns REQUIRED)
find_package(Qt5Test REQUIRED)

set(SOURCE
        src/private/Provisioning.cpp
        src/Provisioning.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Xml Qt5::XmlPatterns)
target_include_directories(${PROJECT_NAME} INTERFACE src/)

#Tests =================
set(TEST_SOURCE
        src/test/ProvisioningTest.cpp
        src/test/ProvisioningTest.h
        src/test/testProvData.xml
        src/test/testMain.cpp
)

add_executable(prov_test ${TEST_SOURCE})
target_link_libraries(prov_test Qt5::Test Qt5::Core Provisioning Http SmartLogger)
add_test(NAME provisioning_test COMMAND prov_test)
