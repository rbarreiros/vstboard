#-------------------------------------------------
#
# Project created by QtCreator 2010-11-29T21:03:21
#
#-------------------------------------------------

top_srcdir  = ..
srcdir      = vstdll
#include($$top_srcdir/config.pri)



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

win32-g++ {
    DEFINES += WINVER=0x0501
    DEFINES += _WIN32_WINNT=0x0501
    LIBS += -L$$quote($$MINGW_PATH/lib)
    INCLUDEPATH += $$quote($$MINGW_PATH/include)
}

win32-msvc2008 {
    DEFINES += _CRT_SECURE_NO_WARNINGS
    INCLUDEPATH += $$quote($$(INCLUDE))
    LIBS += -L$$quote($$(LIB))
    QMAKE_CFLAGS += -Fd$$top_destdir/$$TARGET

#to add symbols :
#    QMAKE_CFLAGS_RELEASE +=  -Zi
#    QMAKE_LFLAGS_RELEASE += /DEBUG
}

QT       += core gui

TARGET = vstdll
TEMPLATE = lib

include($$top_srcdir/$$QTWINMIGRATE_PATH/src/qtwinmigrate.pri)
#INCLUDEPATH += $$quote($$top_srcdir/$$QTWINMIGRATE_PATH/src)
#LIBS += -L$$top_destdir -lQtSolutions_MFCMigrationFramework-head

vstsdk {
    DEFINES += VSTSDK
    INCLUDEPATH += $$top_srcdir/$$VSTSDK_PATH \
        $$top_srcdir/$$VSTSDK_PATH/public.sdk/source/vst2.x
}


SOURCES += main.cpp\
        mainwindow.cpp \
    testvst.cpp \
    gui.cpp \
mainhost.cpp \
    ../../libs/vstsdk2.4/public.sdk/source/vst2.x/audioeffectx.cpp \
    ../../libs/vstsdk2.4/public.sdk/source/vst2.x/audioeffect.cpp \
    views/maingraphicsview.cpp \
views/programlist.cpp \
views/filebrowser.cpp \
views/grouplistview.cpp \
views/proglistview.cpp \
   connectables/objectfactory.cpp \
connectables/object.cpp \
connectables/container.cpp \
connectables/maincontainer.cpp \
connectables/parkingcontainer.cpp \
pathsolver.cpp \
renderer.cpp \
models/hostmodel.cpp \
programs.cpp \
vst/cvsthost.cpp \
        connectables/vstplugin.cpp \
        vst/ceffect.cpp \
        vst/vstbank.cpp \
        views/vstpluginwindow.cpp \
        views/vstshellselect.cpp \
 connectables/connectioninfo.cpp \
connectables/objectinfo.cpp \
connectables/pin.cpp \
 connectables/parameterpin.cpp \
 connectables/parameterpinin.cpp \
 connectables/parameterpinout.cpp \
models/programsmodel.cpp \
connectables/bridge.cpp \
    connectables/audiodevicein.cpp \
    connectables/audiodeviceout.cpp \
connectables/mididevice.cpp \
connectables/midisender.cpp \
connectables/miditoautomation.cpp \
connectables/hostcontroller.cpp \
    connectables/midipinin.cpp \
    connectables/midipinout.cpp \
connectables/objectprogram.cpp \
    connectables/audiopinout.cpp \
    connectables/audiopinin.cpp \
connectables/containerprogram.cpp \
solvernode.cpp \
audiobuffer.cpp \
    connectables/bridgepinin.cpp \
    connectables/bridgepinout.cpp \
connectables/cable.cpp \
connectables/objectparameter.cpp \
views/configdialog.cpp \
views/sceneview.cpp \
projectfile/setupfile.cpp \
projectfile/projectfile.cpp \
models/listtoolsmodel.cpp \
views/objectview.cpp \
    views/pinview.cpp \
    views/cableview.cpp \
views/maincontainerview.cpp \
views/containerview.cpp \
views/connectableobjectview.cpp \
views/bridgepinview.cpp \
views/containercontent.cpp \
views/bridgeview.cpp \
views/listpinsview.cpp \
views/connectablepinview.cpp \
connectables/audiodevice.cpp


HEADERS  += mainwindow.h \
    testvst.h \
    gui.h \
mainhost.h \
    views/maingraphicsview.h \
views/programlist.h \
views/filebrowser.h \
views/grouplistview.h \
views/proglistview.h \
   connectables/objectfactory.h \
connectables/object.h \
connectables/container.h \
connectables/maincontainer.h \
connectables/parkingcontainer.h \
pathsolver.h \
renderer.h \
models/hostmodel.h \
programs.h \
vst/cvsthost.h \
        connectables/vstplugin.h \
        vst/ceffect.h \
        vst/const.h \
        vst/vstbank.h \
        views/vstpluginwindow.h \
        views/vstshellselect.h \
 connectables/connectioninfo.h \
connectables/objectinfo.h \
connectables/pin.h \
 connectables/parameterpin.h \
 connectables/parameterpinin.h \
connectables/parameterpinout.h \
models/programsmodel.h \
connectables/bridge.h \
    connectables/audiodevicein.h \
    connectables/audiodeviceout.h \
connectables/mididevice.h \
connectables/midisender.h \
connectables/miditoautomation.h \
connectables/hostcontroller.h \
    connectables/midipinin.h \
    connectables/midipinout.h \
connectables/objectprogram.h \
    connectables/audiopinout.h \
    connectables/audiopinin.h \
connectables/containerprogram.h \
solvernode.h \
audiobuffer.h \
    connectables/bridgepinin.h \
    connectables/bridgepinout.h \
connectables/cable.h \
connectables/objectparameter.h \
views/configdialog.h \
views/sceneview.h \
projectfile/setupfile.h \
projectfile/projectfile.h \
models/listtoolsmodel.h \
views/objectview.h \
    views/pinview.h \
    views/cableview.h \
views/maincontainerview.h \
views/containerview.h \
views/connectableobjectview.h \
views/bridgepinview.h \
views/containercontent.h \
views/bridgeview.h \
views/listpinsview.h \
views/connectablepinview.h \
connectables/audiodevice.h

FORMS    += mainwindow.ui \
views/filebrowser.ui \
views/programlist.ui \
views/vstpluginwindow.ui \
views/configdialog.ui \
        views/vstshellselect.ui
PRECOMPILED_HEADER = precomp.h
OTHER_FILES +=

RESOURCES += \
    ../resources/resources.qrc
