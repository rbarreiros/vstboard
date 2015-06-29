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

TARGET = portaudio
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += $$PORTAUDIO_PATH/include \
	       $$PORTAUDIO_PATH/src/common

DEFINES -= UNICODE

# Shutup warnings about unused api functions
QMAKE_CFLAGS += -Wno-unused-function

# Common Source

SOURCES += $$PORTAUDIO_PATH/src/common/pa_stream.c \
           $$PORTAUDIO_PATH/src/common/pa_trace.c \
           $$PORTAUDIO_PATH/src/hostapi/skeleton/pa_hostapi_skeleton.c \
           $$PORTAUDIO_PATH/src/common/pa_ringbuffer.c \
           $$PORTAUDIO_PATH/src/common/pa_process.c \
           $$PORTAUDIO_PATH/src/common/pa_front.c \
           $$PORTAUDIO_PATH/src/common/pa_dither.c \
           $$PORTAUDIO_PATH/src/common/pa_debugprint.c \
           $$PORTAUDIO_PATH/src/common/pa_cpuload.c \
           $$PORTAUDIO_PATH/src/common/pa_converters.c \
           $$PORTAUDIO_PATH/src/common/pa_allocation.c

unix:!macx {
     CONFIG += link_pkgconfig
     PKGCONFIG += jack
     INCLUDEPATH += $$PORTAUDIO_PATH/src/os/unix

     LIBS += -lasound -lm -lpthread

     DEFINES += PA_USE_ALSA=1
     DEFINES += PA_USE_JACK=1
     DEFINES += PA_USE_OSS=0
     DEFINES += PA_USE_ASIHPI=0

     SOURCES += $$PORTAUDIO_PATH/src/hostapi/alsa/pa_linux_alsa.c \
     	     	$$PORTAUDIO_PATH/src/hostapi/jack/pa_jack.c \
		$$PORTAUDIO_PATH/src/common/pa_ringbuffer.c \
		$$PORTAUDIO_PATH/src/os/unix/pa_unix_hostapis.c \
		$$PORTAUDIO_PATH/src/os/unix/pa_unix_util.c

     HEADERS += $$PORTAUDIO_PATH/include/pa_linux_alsa.h \
     	     	$$PORTAUDIO_PATH/include/pa_jack.h
}

win32 {

    LIBS += -lwinmm -lm -lole32 -luuid -luser32 -ladvapi32 -lsetupapi -lmthreads

    DEFINES += PA_USE_WDMKS=0
    DEFINES += PA_USE_WASAPI=0
    DEFINES += PA_USE_ASIO=1
    DEFINES += PA_USE_WMME=1
    DEFINES += WINDOWS

    INCLUDEPATH += $$PORTAUDIO_PATH/src \
    		   $$PORTAUDIO_PATH/src/common \
    		   $$PORTAUDIO_PATH/src/os/win \
        	   $$PORTAUDIO_PATH/src/hostapi/asio \
    		   $$PORTAUDIO_PATH/src/hostapi/asio/common \
		   $$PORTAUDIO_PATH/src/hostapi/asio/host \
		   $$PORTAUDIO_PATH/src/hostapi/asio/host/pc

    SOURCES += $$PORTAUDIO_PATH/src/hostapi/asio/pa_asio.cpp \
    	       $$PORTAUDIO_PATH/src/common/pa_ringbuffer.c \
	       $$PORTAUDIO_PATH/src/os/win/pa_win_hostapis.c \
	       $$PORTAUDIO_PATH/src/os/win/pa_win_util.c \
	       $$PORTAUDIO_PATH/src/os/win/pa_win_coinitialize.c \
               $$PORTAUDIO_PATH/src/hostapi/asio/iasiothiscallresolver.cpp \
	       $$PORTAUDIO_PATH/src/hostapi/asio/common/asio.cpp \
	       $$PORTAUDIO_PATH/src/hostapi/asio/host/asiodrivers.cpp \
	       $$PORTAUDIO_PATH/src/hostapi/asio/host/pc/asiolist.cpp \
	       $$PORTAUDIO_PATH/src/hostapi/asio/host/ASIOConvertSamples.cpp \
    	       $$PORTAUDIO_PATH/src/hostapi/wmme/pa_win_wmme.c \
	       $$PORTAUDIO_PATH/src/os/win/pa_win_hostapis.c \
	       $$PORTAUDIO_PATH/src/os/win/pa_win_util.c \
	       $$PORTAUDIO_PATH/src/os/win/pa_win_waveformat.c

    HEADERS += $$PORTAUDIO_PATH/include/pa_win_wmme.h \
    	       $$PORTAUDIO_PATH/include/pa_win_waveformat.h \
    	       $$PORTAUDIO_PATH/include/pa_win_wasapi.h \
    	       $$PORTAUDIO_PATH/include/pa_asio.h \
	       $$PORTAUDIO_PATH/include/portaudio.h

    win32-g++ {
    	DEFINES += PA_USE_DS=0

	# ?!?? Needed rly ???
        #DEFINES += KSAUDIO_SPEAKER_DIRECTOUT=0
        #DEFINES += METHOD_NEITHER=3
        #DEFINES += FILE_ANY_ACCESS=0

	QMAKE_CFLAGS += -ffast-math -fomit-frame-pointer	

        SOURCES += $$PORTAUDIO_PATH/src/hostapi/asio/iasiothiscallresolver.cpp
    }

    win32-msvc* {
    	DEFINES += PA_USE_DS=1
        DEFINES += PAWIN_USE_DIRECTSOUNDFULLDUPLEXCREATE

	LIBS += -ldsound

	SOURCES += $$PORTAUDIO_PATH/src/hostapi/dsound/pa_win_ds.c \
		   $$PORTAUDIO_PATH/src/hostapi/dsound/pa_win_ds_dynlink.c \
		   $$PORTAUDIO_PATH/src/os/win/pa_win_hostapis.c \
		   $$PORTAUDIO_PATH/src/os/win/pa_win_util.c \
		   $$PORTAUDIO_PATH/src/os/win/pa_win_coinitialize.c \
		   $$PORTAUDIO_PATH/src/os/win/pa_win_waveformat.c \
		   $$PORTAUDIO_PATH/src/os/win/pa_x86_plain_converters.c 


        HEADERS += $$PORTAUDIO_PATH/include/pa_win_ds.h
    }
}

macx: {
    INCLUDEPATH += $$PORTAUDIO_PATH/src/hostapi/coreaudio

    SOURCES += $$PORTAUDIO_PATH/src/hostapi/coreaudio/pa_mac_core.c \
               $$PORTAUDIO_PATH/src/hostapi/coreaudio/pa_mac_core_blocking.c \
               $$PORTAUDIO_PATH/src/hostapi/coreaudio/pa_mac_core_utilities.c \
               $$PORTAUDIO_PATH/src/os/unix/pa_unix_util.c \
               $$PORTAUDIO_PATH/src/os/unix/pa_unix_hostapis.c

    HEADERS += $$PORTAUDIO_PATH/src/hostapi/coreaudio/pa_mac_core_blocking.h \
               $$PORTAUDIO_PATH/src/hostapi/coreaudio/pa_mac_core_internal.h \
               $$PORTAUDIO_PATH/src/hostapi/coreaudio/pa_mac_core_utilities.h

}

