#-------------------------------------------------
#
# Project created by QtCreator 2011-02-03T20:52:43
#
#-------------------------------------------------

top_srcdir  = ..
srcdir      = dllloader
include(../config.pri)

DEFINES += APP_NAME=\\\"VstBoardLoader\\\"
TARGET = VstBoard
TEMPLATE = lib

INCLUDEPATH += $$top_srcdir/$$VSTSDK_PATH

QT       -= core gui
LIBS += -ladvapi32
LIBS += -luser32

SOURCES += \
    main.cpp

HEADERS +=

win32-msvc* {
    OTHER_FILES += \
        dllLoader.rc
    RC_FILE = dllLoader.rc
}


