find_package(Boost COMPONENTS headers system REQUIRED)
add_executable(Server Main.cpp Server.cpp Server.hpp Session.cpp Session.hpp)
target_link_libraries(Server Shared Boost::headers Boost::system)

# Ad-hook to link windows socket library. Additional fixes for other compilers and systems required.
if(MINGW)
    target_link_libraries(Server ws2_32 wsock32)
endif()
