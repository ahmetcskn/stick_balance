cmake_minimum_required(VERSION 3.16)

project(stick_balance LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLUT REQUIRED)

add_library(
    stick_balance_core
    src/cart_pole.cpp
)

target_include_directories(
    stick_balance_core
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(
    stick_balance
    src/viewer.cpp
)

target_link_libraries(
    stick_balance
    PRIVATE
    stick_balance_core
    glfw
    OpenGL::GL
    GLUT::GLUT
)

