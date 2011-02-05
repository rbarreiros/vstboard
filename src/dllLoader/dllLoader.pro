#-------------------------------------------------
#
# Project created by QtCreator 2011-02-03T20:52:43
#
#-------------------------------------------------

top_srcdir  = ..
srcdir      = dllloader

DEFINES += APP_NAME=\\\"VstBoardLoader\\\"
TARGET = VstBoard
TEMPLATE = lib

DEFINES += DLLLOADER_LIBRARY

VSTSDK_PATH	= ../libs/vstsdk2.4
INCLUDEPATH += $$top_srcdir/$$VSTSDK_PATH

CONFIG(debug, debug|release) {
    POST =
    build_postfix=debug
    DEFINES += DEBUG
} else {
    POST =
    build_postfix=release
}

top_builddir =$$top_srcdir/../build/$$build_postfix
top_destdir  =$$top_srcdir/../bin/$$build_postfix
builddir     =$$top_builddir/$$srcdir

OBJECTS_DIR =$$builddir
DESTDIR     =$$top_destdir
TARGET      =$$TARGET

QT       -= core gui
LIBS += -ladvapi32
LIBS += -luser32

SOURCES += \
    main.cpp

HEADERS +=

OTHER_FILES += \
    dllLoader.rc

RC_FILE = dllLoader.rc
