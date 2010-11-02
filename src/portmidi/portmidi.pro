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

top_srcdir  = ..
srcdir      = portmidi
include($$top_srcdir/config.pri)

CONFIG -= qt
QT -= core gui

DEFINES -= UNICODE

TEMPLATE = lib
CONFIG += staticlib

SOURCES += $$top_srcdir/$$PORTMIDI_PATH/pm_common/portmidi.c \
    $$top_srcdir/$$PORTMIDI_PATH/pm_common/pmutil.c

win32:SOURCES += $$top_srcdir/$$PORTMIDI_PATH/pm_win/pmwinmm.c \
    $$top_srcdir/$$PORTMIDI_PATH/pm_win/pmwin.c \
    $$top_srcdir/$$PORTMIDI_PATH/porttime/ptwinmm.c


unix:SOURCES += $$top_srcdir/$$PORTMIDI_PATH/pm_linux/pmlinux.c \
    $$top_srcdir/$$PORTMIDI_PATH/pm_linux/finddefault.c \
    #$$top_srcdir/$$PORTMIDI_PATH/pm_linux/pmlinuxalsa.c \
    $$top_srcdir/$$PORTMIDI_PATH/porttime/ptlinux.c


