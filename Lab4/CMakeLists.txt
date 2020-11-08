cmake_minimum_required(VERSION 3.17)
project(Lab4)

set(CMAKE_CXX_STANDARD 17)
find_package(Boost COMPONENTS headers REQUIRED)

add_executable(Lab4 Main.cpp EM.cpp EM.hpp MathUtils.cpp MathUtils.hpp)
target_link_libraries(Lab4 Boost::headers)