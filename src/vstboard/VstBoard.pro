DEFINES += APP_NAME=\\\"VstBoard\\\"

srcdir = vstboard
include(../config.pri)

QT += core gui

TEMPLATE = app
TARGET = $${APP_NAME}

DEFINES += VSTBOARD

LIBS += -L$$top_destdir -lportaudio
LIBS += -L$$top_destdir -lportmidi
LIBS += -L$$top_destdir -lcommon

win32 {
    LIBS += -lwinmm
    LIBS += -luser32
    LIBS += -ladvapi32
    LIBS += -lole32
    LIBS += -lsetupapi
    LIBS += -ldsound
}

INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/include
INCLUDEPATH += $$top_srcdir/$$PORTMIDI_PATH/porttime
INCLUDEPATH += $$top_srcdir/$$PORTMIDI_PATH/pm_common

!CONFIG(debug, debug|release) {

    targetdir = $$OUT_PWD/../../bin/$$build_postfix
    builddir = $$OUT_PWD/../../build/$$build_postfix

#other files included in the release
    EXTRA_FILES = \
        $${_PRO_FILE_PWD_}/../../*.txt \
        $${_PRO_FILE_PWD_}/../../tools/*.nsi \

    linux-g++{
        for(FILE,EXTRA_FILES){
            QMAKE_POST_LINK += $$quote(cp $${FILE} $${targetdir} $$escape_expand(\n\t))
        }
    }

    win32-msvc* {
        RC_FILE = vstboard.rc
    }

    win32 {
        EXTRA_FILES_WIN = $${EXTRA_FILES}
        EXTRA_FILES_WIN ~= s,/,\\,g
        DESTDIR_WIN = $${targetdir}
        DESTDIR_WIN ~= s,/,\\,g
        for(FILE,EXTRA_FILES_WIN){
            QMAKE_POST_LINK += copy /y \"$${FILE}\" \"$${DESTDIR_WIN}\" $$escape_expand(\n\t)
        }
    }
}

INCLUDEPATH += ../common

vstsdk {
    DEFINES += VSTSDK
    INCLUDEPATH += $$top_srcdir/$$VSTSDK_PATH \
        $$top_srcdir/$$VSTSDK_PATH/public.sdk/source/vst2.x

    HEADERS += ../common/vst/cvsthost.h \
        ../common/connectables/vstplugin.h \
        ../common/vst/ceffect.h \
        ../common/vst/const.h \
        ../common/vst/vstbank.h \
        ../common/views/vstpluginwindow.h \
        ../common/views/vstshellselect.h
}

SOURCES += \
    main.cpp \
    ../common/mainhost.cpp \
    ../common/connectables/objectfactory.cpp \
    ../common/connectables/container.cpp \
    ../common/mainwindow.cpp \
    connectables/audiodevice.cpp \
    connectables/audiodevicein.cpp \
    connectables/audiodeviceout.cpp \
    models/listmidiinterfacesmodel.cpp \
    models/listaudiointerfacesmodel.cpp \
    connectables/mididevice.cpp \
    audiodevices.cpp \
    mididevices.cpp \
    circularbuffer.cpp \

HEADERS += \
    models/listaudiointerfacesmodel.h \
    connectables/audiodevice.h \
    connectables/audiodevicein.h \
    connectables/audiodeviceout.h \
    models/listmidiinterfacesmodel.h \
    connectables/mididevice.h \
    audiodevices.h \
    mididevices.h \
    circularbuffer.h \


FORMS += ../common/mainwindow.ui \
    ../common/views/configdialog.ui


PRECOMPILED_HEADER = ../common/precomp.h

#RESOURCES += ../resources/resources.qrc

OTHER_FILES += \ 
    ../../README.txt \
    ../../license.txt \
    ../../LGPL.txt \
    ../../GPL.txt \
    ../../tools/nsis.nsi \
    vstboard.rc

TRANSLATIONS = vstboard_fr.ts

RESOURCES += ../resources/resources.qrc
