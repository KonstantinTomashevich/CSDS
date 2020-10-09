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

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ANDROID_ABIS = armeabi-v7a

HEADERS += \
    src/client/authentication_result.h \
    src/client/controller.h \
    src/client/file_info.h \
    src/client/server_connection_result.h
