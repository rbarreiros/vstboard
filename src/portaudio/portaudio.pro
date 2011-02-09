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

srcdir      = portaudio
include(../config.pri)

CONFIG -= qt
QT -= core gui
TEMPLATE = lib
CONFIG += staticlib

DEFINES -= UNICODE

INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/include

linux-g++ {
    DEFINES += KSAUDIO_SPEAKER_DIRECTOUT=0
    DEFINES += METHOD_NEITHER=3
    DEFINES += FILE_ANY_ACCESS=0

    INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/os/unix

    SOURCES += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/alsa/pa_linux_alsa.c
    SOURCES += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/jack/pa_jack.c
    #SOURCES += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/oss/pa_unix_oss.c
    SOURCES += $$top_srcdir/$$PORTAUDIO_PATH/src/os/unix/pa_unix_hostapis.c
    SOURCES += $$top_srcdir/$$PORTAUDIO_PATH/src/os/unix/pa_unix_util.c
}

win32 {

    LIBS += -lwinmm
    LIBS += -luser32
    LIBS += -ladvapi32
    LIBS += -lole32
    LIBS += -lsetupapi
    LIBS += -ldsound

    INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/os/win
    INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/common
    INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host
    INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/pc

    HEADERS += $$top_srcdir/$$PORTAUDIO_PATH/include/pa_win_wmme.h \
    $$top_srcdir/$$PORTAUDIO_PATH/include/pa_win_waveformat.h \
    $$top_srcdir/$$PORTAUDIO_PATH/include/pa_win_wasapi.h \
    $$top_srcdir/$$PORTAUDIO_PATH/include/pa_asio.h

    SOURCES += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/wasapi/pa_win_wasapi.c \
    $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/wmme/pa_win_wmme.c \
    $$top_srcdir/$$PORTAUDIO_PATH/src/os/win/pa_win_waveformat.c \
    $$top_srcdir/$$PORTAUDIO_PATH/src/os/win/pa_win_util.c \
    $$top_srcdir/$$PORTAUDIO_PATH/src/os/win/pa_win_hostapis.c \
    $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/pa_asio.cpp \
    $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/common/asio.cpp \
    $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/ASIOConvertSamples.cpp \
    $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/asiodrivers.cpp \
    $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/pc/asiolist.cpp

    win32-g++ {
        DEFINES += KSAUDIO_SPEAKER_DIRECTOUT=0
        DEFINES += METHOD_NEITHER=3
        DEFINES += FILE_ANY_ACCESS=0

        DEFINES += PA_NO_WDMKS
        DEFINES += PA_NO_DS
        DEFINES += PA_NO_WASAPI
    #    DEFINES += PA_NO_WMME
    #    DEFINES += PA_NO_ASIO

        INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/wasapi/mingw-include

        SOURCES += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/iasiothiscallresolver.cpp

    }

    win32-msvc* {
        DEFINES += PAWIN_USE_DIRECTSOUNDFULLDUPLEXCREATE

        INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/os/win
        HEADERS += $$top_srcdir/$$PORTAUDIO_PATH/src/os/win/pa_win_wdmks_utils.h \
                $$top_srcdir/$$PORTAUDIO_PATH/src/os/win/pa_x86_plain_converters.h \
                $$top_srcdir/$$PORTAUDIO_PATH/include/pa_win_ds.h
        SOURCES += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/wdmks/pa_win_wdmks.c \
                $$top_srcdir/$$PORTAUDIO_PATH/src/os/win/pa_win_wdmks_utils.c \
                $$top_srcdir/$$PORTAUDIO_PATH/src/os/win/pa_x86_plain_converters.c \
                $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/dsound/pa_win_ds.c \
                $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/dsound/pa_win_ds_dynlink.c
    }



    INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/common
    INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/

    HEADERS += $$top_srcdir/$$PORTAUDIO_PATH/include/portaudio.h

    SOURCES += $$top_srcdir/$$PORTAUDIO_PATH/src/common/pa_stream.c \
        $$top_srcdir/$$PORTAUDIO_PATH/src/common/pa_skeleton.c \
        $$top_srcdir/$$PORTAUDIO_PATH/src/common/pa_ringbuffer.c \
        $$top_srcdir/$$PORTAUDIO_PATH/src/common/pa_process.c \
        $$top_srcdir/$$PORTAUDIO_PATH/src/common/pa_front.c \
        $$top_srcdir/$$PORTAUDIO_PATH/src/common/pa_dither.c \
        $$top_srcdir/$$PORTAUDIO_PATH/src/common/pa_debugprint.c \
        $$top_srcdir/$$PORTAUDIO_PATH/src/common/pa_cpuload.c \
        $$top_srcdir/$$PORTAUDIO_PATH/src/common/pa_converters.c \
        $$top_srcdir/$$PORTAUDIO_PATH/src/common/pa_allocation.c
}

mac {
    INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/os/mac_osx
    INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/common
    INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host
    INCLUDEPATH += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/mac

    SOURCES += $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/pa_asio.cpp \
    $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/common/asio.cpp \
    $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/ASIOConvertSamples.cpp \
    $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/asiodrivers.cpp \
    $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/mac/asioshlib.cpp \
    $$top_srcdir/$$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/mac/codefragements.cpp
}

OTHER_FILES +=
