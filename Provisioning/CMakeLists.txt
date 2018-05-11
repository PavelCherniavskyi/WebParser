cmake_minimum_required(VERSION 2.8.12)

project(Provisioning)
find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5XmlPatterns REQUIRED)

set(SOURCE
	Provisioning.cpp
	Provisioning.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Xml Qt5::XmlPatterns)
target_include_directories(${PROJECT_NAME} INTERFACE .)
