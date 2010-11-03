#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.

PORTAUDIO_PATH 	= ../libs/portaudio
PORTMIDI_PATH 	= ../libs/portmidi

win32|macx {
    VSTSDK_PATH	= ../libs/vstsdk2.4
    CONFIG += vstsdk
}

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
#MOC_DIR     =$$srcdir/..
#UI_DIR      =$$srcdir/..
#RCC_DIR     =$$srcdir/..
DESTDIR     =$$top_destdir
TARGET      =$$TARGET

INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/include
INCLUDEPATH += $$top_srcdir/$$PORTMIDI_PATH/porttime
INCLUDEPATH += $$top_srcdir/$$PORTMIDI_PATH/pm_common

LIBS += -L$$top_destdir -lportaudio
LIBS += -L$$top_destdir -lportmidi

win32 {
    LIBS += -lwinmm
    LIBS += -luser32
    LIBS += -ladvapi32
    LIBS += -lole32
    LIBS += -lsetupapi
    LIBS += -ldsound
}

linux-g++ {
    message("linux-g++")

} else:win32-g++ {
    message("win32-g++")
    DEFINES += WINVER=0x0501
    DEFINES += _WIN32_WINNT=0x0501
    LIBS += -L$$quote($$MINGW_PATH/lib)
    INCLUDEPATH += $$quote($$MINGW_PATH/include)


} else:win32-msvc2008 {
    message("win32-msvc2008")
    DEFINES += _CRT_SECURE_NO_WARNINGS
    INCLUDEPATH += $$quote($$(INCLUDE))
    LIBS += -L$$quote($$(LIB))

#to add symbols :
#    QMAKE_CFLAGS_RELEASE +=  -Zi
#    QMAKE_LFLAGS_RELEASE += /DEBUG

    QMAKE_CFLAGS += -Fd$$top_destdir/$$TARGET
} else {
    error("compiler not configured in config.pri")
}

#modified in qmake.conf for msvc2008 :
#QMAKE_CFLAGS_RELEASE    = -O2 -MT -GL -MP -Ob2 -Oi -Ot -Oy- -EHsc -GS -Gy -fp:fast -GA -arch:SSE2
#QMAKE_LFLAGS_RELEASE    = /INCREMENTAL:NO /OPT:REF /OPT:ICF /LTCG  /NODEFAULTLIB:MSVCRT
#QMAKE_LIB               = lib /NOLOGO /LTCG

HEADERS +=

SOURCES +=
