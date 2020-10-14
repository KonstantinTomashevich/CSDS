QT += quick

CONFIG += c++17

SOURCES += \
        src/authentication_result.cpp \
        src/controller.cpp \
        src/file_info.cpp \
        src/main.cpp \
        src/server_connection_result.cpp

RESOURCES += res/qml.qrc

TRANSLATIONS += \
    res/ts/Client_ru_BY.ts

# shared headers (cryptography)
INCLUDEPATH += ../

LIBS = -L"/home/vladislav/Qt/5.15.1/gcc_64/lib"
!android: LIBS += -L"/usr/lib/x86_64-linux-gnu" -lboost_system
!android: LIBS += -L"$$PWD/../cmake-build-debug/Shared" -lShared

HEADERS += \
    src/client/authentication_result.h \
    src/client/controller.h \
    src/client/file_info.h \
    src/client/server_connection_result.h

android {
    # boost for android
    INCLUDEPATH += /home/vladislav/Libraries/ndk_21_boost_1.72.0/include
    LIBS += -L"/home/vladislav/Libraries/ndk_21_boost_1.72.0/libs"
    # compile as project files
    SOURCES += $$files(../Shared/*.cpp)
    HEADERS += $$files(../Shared/*.h)
    # DEFINES += ANDROID_STL=c++_static
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ANDROID_ABIS = armeabi-v7a
