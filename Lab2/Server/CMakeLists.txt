find_package(Boost COMPONENTS headers system log REQUIRED)
add_executable(Server Main.cpp Server.cpp Server.hpp Session.cpp Session.hpp AuthService.cpp AuthService.hpp FileService.cpp FileService.hpp)
target_link_libraries(Server Shared Boost::headers Boost::system Boost::log)

# Ad-hook to link windows socket library. Additional fixes for other compilers and systems required.
if(MINGW)
    target_link_libraries(Server ws2_32 wsock32)
endif()
