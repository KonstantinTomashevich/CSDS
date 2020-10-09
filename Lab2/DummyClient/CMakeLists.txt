find_package(Boost COMPONENTS headers system log REQUIRED)
add_executable(DummyClient Main.cpp)
target_link_libraries(DummyClient Shared Boost::headers Boost::system Boost::log)

# Ad-hook to link windows socket library. Additional fixes for other compilers and systems required.
if(MINGW)
    target_link_libraries(DummyClient ws2_32 wsock32)
endif()
