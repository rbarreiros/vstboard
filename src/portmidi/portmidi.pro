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

TARGET = portmidi
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += $$PORTMIDI_PATH/porttime
INCLUDEPATH += $$PORTMIDI_PATH/pm_common

DEFINES -= UNICODE

# Common sources

SOURCES += $$PORTMIDI_PATH/pm_common/portmidi.c \
	   $$PORTMIDI_PATH/pm_common/pmutil.c \
	   $$PORTMIDI_PATH/porttime/porttime.c


HEADERS += $$PORTMIDI_PATH/pm_common/pmutil.h \
	   $$PORTMIDI_PATH/pm_common/pminternal.h \
	   $$PORTMIDI_PATH/pm_common/portmidi.h \
	   $$PORTMIDI_PATH/porttime/porttime.h

win32 {
	INCLUDEPATH += $$PORTMIDI_PATH/pm_win
	LIBS += -lwinmm
	SOURCES += $$PORTMIDI_PATH/pm_win/pmwinmm.c \
                   $$PORTMIDI_PATH/pm_win/pmwin.c \
                   $$PORTMIDI_PATH/porttime/ptwinmm.c
	HEADERS += $$PORTMIDI_PATH/pm_win/pmwinmm.h
}

unix:!macx {
	DEFINES += PMALSA
	INCLUDEPATH += $$PORTMIDI_PATH/pm_linux
	LIBS += -lasound
	SOURCES += $$PORTMIDI_PATH/pm_linux/finddefault.c \
		   $$PORTMIDI_PATH/pm_linux/pmlinux.c \
                   $$PORTMIDI_PATH/pm_linux/pmlinuxalsa.c \
                   $$PORTMIDI_PATH/porttime/ptlinux.c
	HEADERS += $$PORTMIDI_PATH/pm_linux/pmlinux.h \
		   $$PORTMIDI_PATH/pm_linux/pmlinuxalsa.h
}

macx: {
        INCLUDEPATH += $$PORTMIDI_PATH/pm_mac \
                       /System/Library/Frameworks/CoreFoundation.framework/Headers \
                       /System/Library/Frameworks/CoreServices.framework/Headers \
                       /Developer/Headers/FlatCarbon
        DEFINES +=
        LIBS += -framework CoreFoundation -framework CoreServices
        SOURCES += $$PORTMIDI_PATH/pm_mac/readbinaryplist.c \
                   $$PORTMIDI_PATH/pm_mac/pmmac.c \
                   $$PORTMIDI_PATH/pm_mac/finddefault.c \
                   $$PORTMIDI_PATH/porttime/ptmacosx_cf.c \
                   $$PORTMIDI_PATH/pm_mac/pmmacosxcm.c

        HEADERS += $$PORTMIDI_PATH/pm_mac/readbinaryplist.h \
                   $$PORTMIDI_PATH/pm_mac/pmmac.h
}
