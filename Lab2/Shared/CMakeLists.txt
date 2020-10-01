find_package(Boost COMPONENTS headers REQUIRED)
add_library(Shared Idea.cpp Idea.hpp RSA.cpp RSA.hpp)
target_link_libraries(Shared Boost::headers)
