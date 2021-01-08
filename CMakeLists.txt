cmake_minimum_required(VERSION 3.10.2)
project(mathematical_pendulum C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_REQUIRED_LIBRARIES m)

add_executable(mathematical_pendulum src/main.c)

target_link_libraries(mathematical_pendulum m)
