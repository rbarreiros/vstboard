#    Copyright 2015 Rui Barreiros
#    Contact : rbarreiros@gmail.com
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

QT -= core gui

TARGET = rtaudio
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += $$RTAUDIO_PATH/ \
               $$RTAUDIO_PATH/include

CONFIG(debug, debug|release): DEFINES += __RTAUDIO_DEBUG__
DEFINES -= UNICODE

# Common sources

SOURCES += $$RTAUDIO_PATH/RtAudio.cpp
HEADERS += $$RTAUDIO_PATH/RtAudio.h

win32: {
        # Directsound (Requires DX SDK)
        DEFINES += __WINDOWS_DS__
        LIBS += -ldsound

        # Windows WASAPI
        DEFINES += __WINDOWS_WASAPI__
        LIBS += -luuid -lksuser

        # Windows ASIO
        DEFINES += __WINDOWS_ASIO__
        SOURCES += $$RTAUDIO_PATH/include/asio.cpp \
                   $$RTAUDIO_PATH/include/asiodrivers.cpp \
                   $$RTAUDIO_PATH/include/asiolist.cpp \
                   $$RTAUDIO_PATH/include/iasiothiscallresolver.cpp
}

unix:!macx {
        CONFIG += link_pkgconfig
        PKGCONFIG += jack
        DEFINES += __LINUX_ALSA__
        DEFINES += __UNIX_JACK__
        LIBS += -lasound -lm -lpthread
}

macx: {
        DEFINES += __MACOSX_CORE__
        LIBS += -framework CoreAudio
}
