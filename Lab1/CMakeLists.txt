cmake_minimum_required(VERSION 3.15)
project(Lab1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(Utility)
add_subdirectory(VigenereEncoder)
add_subdirectory(VigenereDecoder)
add_subdirectory(VigenereCracker)
