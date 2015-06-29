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

TARGET = rtmidi
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += $$RTMIDI_PATH

#DEFINES -= UNICODE

# Common sources

SOURCES += $$RTMIDI_PATH/RtMidi.cpp
HEADERS += $$RTMIDI_PATH/RtMidi.h

win32: {
    DEFINES += __WINDOWS_MM__
    LIBS += -lwinmm -lpthread
}

unix:!macx {
    DEFINES += __LINUX_ALSA__
    DEFINES += __UNIX_JACK__
    LIBS += -lasound -lpthread -ljack
}

macx: {
    LIBS += -framework CoreMIDI -framework CoreAudio -framework CoreFoundation
    DEFINES += __MACOSX_CORE__
}

