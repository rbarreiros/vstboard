#-------------------------------------------------
#
# Project created by QtCreator 2011-02-03T20:52:43
#
#-------------------------------------------------

include(../config.pri)

TARGET = VstBoard
TEMPLATE = lib

QT       -= core gui
LIBS += -ladvapi32
LIBS += -luser32

scriptengine {
    DEFINES += SCRIPTENGINE
}

SOURCES += \
    main.cpp

HEADERS +=

win32-msvc* {
    RC_FILE = dllLoader.rc
}


