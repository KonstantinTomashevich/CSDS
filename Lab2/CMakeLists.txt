cmake_minimum_required(VERSION 3.15)
project(Lab2)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(Shared)
add_subdirectory(TestShared)
add_subdirectory(Server)
