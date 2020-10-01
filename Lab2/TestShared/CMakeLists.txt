find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(TestShared TestModule.cpp Idea.cpp RSA.cpp)
target_link_libraries(TestShared Shared Boost::unit_test_framework)
