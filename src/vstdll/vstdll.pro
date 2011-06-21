DEFINES += APP_NAME=\\\"VstBoardPlugin\\\"

include(../config.pri)

QT += core gui script

TARGET = "VstBoardPlugin"
TEMPLATE = lib
#CONFIG += staticlib

CONFIG += qtwinmigrate-uselib
include($${_PRO_FILE_PWD_}/../../libs/qtwinmigrate/src/qtwinmigrate.pri)

win32 {
    LIBS += -lwinmm
    LIBS += -ladvapi32
    LIBS += -lws2_32
}

win32-g++ {
    DEFINES += WINVER=0x0501
    DEFINES += _WIN32_WINNT=0x0501
   # LIBS += -L$$quote($$MINGW_PATH/lib)
    # INCLUDEPATH += $$quote($$MINGW_PATH/include)
    QMAKE_LFLAGS += -static-libgcc
}

win32-msvc* {
    DEFINES += _CRT_SECURE_NO_WARNINGS
   # INCLUDEPATH += $$quote($$(INCLUDE))
   # LIBS += -L$$quote($$(LIB))
   # QMAKE_CFLAGS += -Fd$$top_destdir/$$TARGET
    RC_FILE = vstdll.rc

#to add symbols :
#    QMAKE_CFLAGS_RELEASE +=  -Zi
#    QMAKE_LFLAGS_RELEASE += /DEBUG
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


PRECOMPILED_HEADER = ../common/precomp.h

RESOURCES += ../resources/resources.qrc

TRANSLATIONS = ../resources/translations/vstdll_fr.ts

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix:!symbian: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD/../common
DEPENDPATH += $$PWD/../common

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/release/common.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/debug/common.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../common/libcommon.a
