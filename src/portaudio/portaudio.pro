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

include(../config.pri)

CONFIG -= qt
QT -= core gui
TEMPLATE = lib
CONFIG += staticlib

DEFINES -= UNICODE

INCLUDEPATH += $$PORTAUDIO_PATH/include

linux-g++ {
    DEFINES += KSAUDIO_SPEAKER_DIRECTOUT=0
    DEFINES += METHOD_NEITHER=3
    DEFINES += FILE_ANY_ACCESS=0

    INCLUDEPATH += $$PORTAUDIO_PATH/src/os/unix

    SOURCES += $$PORTAUDIO_PATH/src/hostapi/alsa/pa_linux_alsa.c
    SOURCES += $$PORTAUDIO_PATH/src/hostapi/jack/pa_jack.c
    #SOURCES += $$PORTAUDIO_PATH/src/hostapi/oss/pa_unix_oss.c
    SOURCES += $$PORTAUDIO_PATH/src/os/unix/pa_unix_hostapis.c
    SOURCES += $$PORTAUDIO_PATH/src/os/unix/pa_unix_util.c
}

win32 {

    LIBS += -lwinmm
    LIBS += -luser32
    LIBS += -ladvapi32
    LIBS += -lole32
    LIBS += -lsetupapi
    LIBS += -ldsound

    INCLUDEPATH += $$PORTAUDIO_PATH/src/os/win
    INCLUDEPATH += $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/common
    INCLUDEPATH += $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host
    INCLUDEPATH += $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/pc

    HEADERS += $$PORTAUDIO_PATH/include/pa_win_wmme.h \
    $$PORTAUDIO_PATH/include/pa_win_waveformat.h \
    $$PORTAUDIO_PATH/include/pa_win_wasapi.h \
    $$PORTAUDIO_PATH/include/pa_asio.h

    SOURCES += $$PORTAUDIO_PATH/src/hostapi/wasapi/pa_win_wasapi.c \
    $$PORTAUDIO_PATH/src/hostapi/wmme/pa_win_wmme.c \
    $$PORTAUDIO_PATH/src/os/win/pa_win_waveformat.c \
    $$PORTAUDIO_PATH/src/os/win/pa_win_util.c \
    $$PORTAUDIO_PATH/src/os/win/pa_win_hostapis.c \
    $$PORTAUDIO_PATH/src/hostapi/asio/pa_asio.cpp \
    $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/common/asio.cpp \
    $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/ASIOConvertSamples.cpp \
    $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/asiodrivers.cpp \
    $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/pc/asiolist.cpp

    win32-g++ {
        DEFINES += KSAUDIO_SPEAKER_DIRECTOUT=0
        DEFINES += METHOD_NEITHER=3
        DEFINES += FILE_ANY_ACCESS=0

        DEFINES += PA_NO_WDMKS
        DEFINES += PA_NO_DS
        DEFINES += PA_NO_WASAPI
    #    DEFINES += PA_NO_WMME
    #    DEFINES += PA_NO_ASIO

        INCLUDEPATH += $$PORTAUDIO_PATH/src/hostapi/wasapi/mingw-include

        SOURCES += $$PORTAUDIO_PATH/src/hostapi/asio/iasiothiscallresolver.cpp

    }

    win32-msvc* {
        DEFINES += PAWIN_USE_DIRECTSOUNDFULLDUPLEXCREATE

        INCLUDEPATH += $$PORTAUDIO_PATH/src/os/win
        HEADERS += $$PORTAUDIO_PATH/src/os/win/pa_win_wdmks_utils.h \
                $$PORTAUDIO_PATH/src/os/win/pa_x86_plain_converters.h \
                $$PORTAUDIO_PATH/include/pa_win_ds.h
        SOURCES += $$PORTAUDIO_PATH/src/hostapi/wdmks/pa_win_wdmks.c \
                $$PORTAUDIO_PATH/src/os/win/pa_win_wdmks_utils.c \
                $$PORTAUDIO_PATH/src/os/win/pa_x86_plain_converters.c \
                $$PORTAUDIO_PATH/src/hostapi/dsound/pa_win_ds.c \
                $$PORTAUDIO_PATH/src/hostapi/dsound/pa_win_ds_dynlink.c
    }



    INCLUDEPATH += $$PORTAUDIO_PATH/src/common
    INCLUDEPATH += $$PORTAUDIO_PATH/src/

    HEADERS += $$PORTAUDIO_PATH/include/portaudio.h

    SOURCES += $$PORTAUDIO_PATH/src/common/pa_stream.c \
        $$PORTAUDIO_PATH/src/common/pa_skeleton.c \
        $$PORTAUDIO_PATH/src/common/pa_ringbuffer.c \
        $$PORTAUDIO_PATH/src/common/pa_process.c \
        $$PORTAUDIO_PATH/src/common/pa_front.c \
        $$PORTAUDIO_PATH/src/common/pa_dither.c \
        $$PORTAUDIO_PATH/src/common/pa_debugprint.c \
        $$PORTAUDIO_PATH/src/common/pa_cpuload.c \
        $$PORTAUDIO_PATH/src/common/pa_converters.c \
        $$PORTAUDIO_PATH/src/common/pa_allocation.c
}

mac {
    INCLUDEPATH += $PORTAUDIO_PATH/src/os/mac_osx
    INCLUDEPATH += $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/common
    INCLUDEPATH += $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host
    INCLUDEPATH += $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/mac

    SOURCES += $$PORTAUDIO_PATH/src/hostapi/asio/pa_asio.cpp \
    $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/common/asio.cpp \
    $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/ASIOConvertSamples.cpp \
    $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/asiodrivers.cpp \
    $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/mac/asioshlib.cpp \
    $$PORTAUDIO_PATH/src/hostapi/asio/ASIOSDK/host/mac/codefragements.cpp
}

OTHER_FILES +=
