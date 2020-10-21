find_package(Boost COMPONENTS headers REQUIRED)
add_library(Shared Idea.cpp Idea.hpp RSA.cpp RSA.hpp MessageType.hpp StateMachine.hpp StateMachine.cpp MD5.cpp MD5.hpp GM.hpp GM.cpp)
target_link_libraries(Shared Boost::headers)
