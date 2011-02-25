DEFINES += APP_NAME=\\\"VstBoardPlugin\\\"

srcdir      = vstdll
include(../config.pri)

QT += core gui script

TARGET = "VstBoardPlugin"
TEMPLATE = lib
#CONFIG += staticlib

include($${_PRO_FILE_PWD_}/../../libs/qtwinmigrate/src/qtwinmigrate.pri)

LIBS += -L$$top_destdir -lcommon

win32 {
    LIBS += -lwinmm
    LIBS += -ladvapi32
    LIBS += -lws2_32
}

win32-g++ {
    DEFINES += WINVER=0x0501
    DEFINES += _WIN32_WINNT=0x0501
    LIBS += -L$$quote($$MINGW_PATH/lib)
    INCLUDEPATH += $$quote($$MINGW_PATH/include)
    QMAKE_LFLAGS += -static-libgcc
}

win32-msvc* {
    DEFINES += _CRT_SECURE_NO_WARNINGS
    INCLUDEPATH += $$quote($$(INCLUDE))
    LIBS += -L$$quote($$(LIB))
    QMAKE_CFLAGS += -Fd$$top_destdir/$$TARGET
    RC_FILE = vstdll.rc

#to add symbols :
#    QMAKE_CFLAGS_RELEASE +=  -Zi
#    QMAKE_LFLAGS_RELEASE += /DEBUG
}


vstsdk {
    DEFINES += VSTSDK
    INCLUDEPATH += $$top_srcdir/$$VSTSDK_PATH \
        $$top_srcdir/$$VSTSDK_PATH/public.sdk/source/vst2.x
}


INCLUDEPATH += ../common

SOURCES += \
    main.cpp \
    gui.cpp \
    connectables/vstaudiodevicein.cpp \
    connectables/vstaudiodeviceout.cpp \
    vst.cpp \
    connectables/vstautomation.cpp \
    connectables/vstmididevice.cpp \
    mainwindowvst.cpp \
    mainhostvst.cpp \
    connectables/objectfactoryvst.cpp \
    views/configdialogvst.cpp


HEADERS  += \
    gui.h \
    connectables/vstaudiodevicein.h \
    connectables/vstaudiodeviceout.h \
    vst.h \
    connectables/vstautomation.h \
    connectables/vstmididevice.h \
    mainwindowvst.h \
    mainhostvst.h \
    connectables/objectfactoryvst.h \
    views/configdialogvst.h


FORMS +=


PRECOMPILED_HEADER = ../common/precomp.h
OTHER_FILES += \
    vstdll.rc

RESOURCES += ../resources/resources.qrc
