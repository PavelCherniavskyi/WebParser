cmake_minimum_required(VERSION 2.8.12)

project(WebParser)
find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set (CMAKE_CXX_STANDARD 11)
add_compile_options(-pedantic -Werror -std=c++11)

add_executable(${PROJECT_NAME} "main.cpp" "types.h")

add_subdirectory(Provisioning)
add_subdirectory(Http)

target_link_libraries(${PROJECT_NAME} Qt5::Core Provisioning Http)
