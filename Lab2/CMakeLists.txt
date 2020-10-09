cmake_minimum_required(VERSION 3.15)
project(Lab2)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(Shared)
add_subdirectory(TestShared)
add_subdirectory(Server)
add_subdirectory(DummyClient)
