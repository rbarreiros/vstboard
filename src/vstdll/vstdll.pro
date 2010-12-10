#-------------------------------------------------
#
# Project created by QtCreator 2010-11-29T21:03:21
#
#-------------------------------------------------

top_srcdir  = ..
srcdir      = vstdll

DEFINES += APP_NAME=\\\"VstBoardPlugin\\\"
TARGET = "VstBoardPlugin"

TEMPLATE = lib
CONFIG += dll
DEFINES += QT_NODLL

DEFINES += VST_PLUGIN

PORTMIDI_PATH 	= ../libs/portmidi
INCLUDEPATH += $$top_srcdir/$$PORTMIDI_PATH/pm_common
INCLUDEPATH += $$top_srcdir/$$PORTMIDI_PATH/porttime

QTWINMIGRATE_PATH = ../libs/qtwinmigrate

DEFINES += APP_NAME=\\\"VstBoardPlugin\\\"

VSTSDK_PATH	= ../libs/vstsdk2.4
CONFIG += vstsdk

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

win32 {
    LIBS += -lwinmm
#    LIBS += -luser32
    LIBS += -ladvapi32
#    LIBS += -lole32
#    LIBS += -lsetupapi
#    LIBS += -ldsound
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


QT       += core gui

LIBS += -lqtmain
#LIBS += -L$$top_destdir -lportmidi

include($$top_srcdir/$$QTWINMIGRATE_PATH/src/qtwinmigrate.pri)

vstsdk {
    DEFINES += VSTSDK
    INCLUDEPATH += $$top_srcdir/$$VSTSDK_PATH \
        $$top_srcdir/$$VSTSDK_PATH/public.sdk/source/vst2.x
}


INCLUDEPATH += ../vstboard

SOURCES += \
main.cpp \
mainwindow.cpp \
gui.cpp \
views/maingraphicsview.cpp \
connectables/vstaudiodevicein.cpp \
connectables/vstaudiodeviceout.cpp \
connectables/mididevice.cpp \
connectables/hostcontroller.cpp \
views/configdialog.cpp \
../vstboard/views/connectablepinview.cpp \
../../libs/vstsdk2.4/public.sdk/source/vst2.x/audioeffectx.cpp \
../../libs/vstsdk2.4/public.sdk/source/vst2.x/audioeffect.cpp \
../vstboard/mainhost.cpp \
../vstboard/connectables/midipinin.cpp \
../vstboard/connectables/midipinout.cpp \
../vstboard/connectables/audiopinout.cpp \
../vstboard/connectables/audiopinin.cpp \
../vstboard/views/objectview.cpp \
../vstboard/views/pinview.cpp \
../vstboard/connectables/objectprogram.cpp \
../vstboard/connectables/containerprogram.cpp \
../vstboard/solvernode.cpp \
../vstboard/audiobuffer.cpp \
../vstboard/connectables/bridgepinin.cpp \
../vstboard/connectables/bridgepinout.cpp \
../vstboard/connectables/cable.cpp \
../vstboard/connectables/objectparameter.cpp \
../vstboard/views/sceneview.cpp \
../vstboard/projectfile/setupfile.cpp \
../vstboard/projectfile/projectfile.cpp \
../vstboard/models/listtoolsmodel.cpp \
../vstboard/views/cableview.cpp \
../vstboard/views/maincontainerview.cpp \
../vstboard/views/containerview.cpp \
../vstboard/views/connectableobjectview.cpp \
../vstboard/views/bridgepinview.cpp \
../vstboard/views/containercontent.cpp \
../vstboard/views/bridgeview.cpp \
../vstboard/views/listpinsview.cpp \
../vstboard/views/programlist.cpp \
../vstboard/views/filebrowser.cpp \
../vstboard/views/grouplistview.cpp \
../vstboard/views/proglistview.cpp \
../vstboard/connectables/objectfactory.cpp \
../vstboard/connectables/object.cpp \
../vstboard/connectables/container.cpp \
../vstboard/connectables/maincontainer.cpp \
../vstboard/connectables/parkingcontainer.cpp \
../vstboard/pathsolver.cpp \
../vstboard/renderer.cpp \
../vstboard/models/hostmodel.cpp \
../vstboard/programs.cpp \
../vstboard/vst/cvsthost.cpp \
../vstboard/connectables/vstplugin.cpp \
../vstboard/vst/ceffect.cpp \
../vstboard/vst/vstbank.cpp \
../vstboard/views/vstpluginwindow.cpp \
../vstboard/views/vstshellselect.cpp \
../vstboard/connectables/connectioninfo.cpp \
../vstboard/connectables/objectinfo.cpp \
../vstboard/connectables/pin.cpp \
../vstboard/connectables/parameterpin.cpp \
../vstboard/connectables/parameterpinin.cpp \
../vstboard/connectables/parameterpinout.cpp \
../vstboard/models/programsmodel.cpp \
../vstboard/connectables/bridge.cpp \
../vstboard/views/aboutdialog.cpp \
    vst.cpp \
    ../../libs/portmidi/pm_common/portmidi.c \
    ../../libs/portmidi/pm_common/pmutil.c \
    ../../libs/portmidi/pm_win/pmwin.c \
    ../../libs/portmidi/porttime/porttime.c \
    ../../libs/portmidi/porttime/ptwinmm.c \
    ../../libs/portmidi/pm_win/pmwinmm.c \
    ../vstboard/connectables/miditoautomation.cpp \
    ../vstboard/connectables/midisender.cpp \
    connectables/vstautomation.cpp


HEADERS  += \
mainwindow.h \
gui.h \
views/maingraphicsview.h \
connectables/vstaudiodevicein.h \
connectables/vstaudiodeviceout.h \
connectables/mididevice.h \
../vstboard/globals.h \
../vstboard/mainhost.h \
../vstboard/connectables/midisender.h \
../vstboard/connectables/miditoautomation.h \
../vstboard/connectables/hostcontroller.h \
../vstboard/connectables/midipinout.h \
../vstboard/connectables/audiopinout.h \
../vstboard/connectables/audiopinin.h \
../vstboard/views/configdialog.h \
../vstboard/views/objectview.h \
../vstboard/views/pinview.h \
../vstboard/views/connectablepinview.h \
../vstboard/connectables/objectprogram.h \
../vstboard/connectables/containerprogram.h \
../vstboard/solvernode.h \
../vstboard/audiobuffer.h \
../vstboard/connectables/bridgepinin.h \
../vstboard/connectables/bridgepinout.h \
../vstboard/connectables/cable.h \
../vstboard/connectables/objectparameter.h \
../vstboard/views/sceneview.h \
../vstboard/projectfile/setupfile.h \
../vstboard/projectfile/projectfile.h \
../vstboard/models/listtoolsmodel.h \
../vstboard/views/cableview.h \
../vstboard/views/maincontainerview.h \
../vstboard/views/containerview.h \
../vstboard/views/connectableobjectview.h \
../vstboard/views/bridgepinview.h \
../vstboard/views/containercontent.h \
../vstboard/views/bridgeview.h \
../vstboard/views/listpinsview.h \
../vstboard/views/programlist.h \
../vstboard/views/filebrowser.h \
../vstboard/views/grouplistview.h \
../vstboard/views/proglistview.h \
../vstboard/connectables/objectfactory.h \
../vstboard/connectables/object.h \
../vstboard/connectables/container.h \
../vstboard/connectables/maincontainer.h \
../vstboard/connectables/parkingcontainer.h \
../vstboard/pathsolver.h \
../vstboard/renderer.h \
../vstboard/models/hostmodel.h \
../vstboard/programs.h \
../vstboard/vst/cvsthost.h \
../vstboard/connectables/vstplugin.h \
../vstboard/vst/ceffect.h \
../vstboard/vst/const.h \
../vstboard/vst/vstbank.h \
../vstboard/views/vstpluginwindow.h \
../vstboard/views/vstshellselect.h \
../vstboard/connectables/connectioninfo.h \
../vstboard/connectables/objectinfo.h \
../vstboard/connectables/pin.h \
../vstboard/connectables/parameterpin.h \
../vstboard/connectables/parameterpinin.h \
../vstboard/connectables/parameterpinout.h \
../vstboard/models/programsmodel.h \
../vstboard/connectables/bridge.h \
../vstboard/views/aboutdialog.h \
    vst.h \
    ../vstboard/_version.h \
    ../vstboard/connectables/midipinin.h \
    connectables/vstautomation.h

FORMS += \
../vstboard/views/configdialog.ui \
../vstboard/mainwindow.ui \
../vstboard/views/filebrowser.ui \
../vstboard/views/programlist.ui \
../vstboard/views/vstpluginwindow.ui \
../vstboard/views/vstshellselect.ui \
../vstboard/views/aboutdialog.ui

PRECOMPILED_HEADER = ../vstboard/precomp.h
OTHER_FILES += \
    vstdll.rc

RESOURCES += ../resources/resources.qrc
