#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    under the terms of the GNU Lesser General Public License for more details.
#
#    You should have received a copy of the under the terms of the GNU Lesser General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.

DEFINES += APP_NAME=\\\"VstBoard\\\"

top_srcdir = ..
srcdir = vstboard

include($$top_srcdir/config.pri)
TEMPLATE = app
TARGET = $${APP_NAME}
!CONFIG(debug, debug|release) {

    targetdir = $$OUT_PWD/../../bin/$$build_postfix
    builddir = $$OUT_PWD/../../build/$$build_postfix

#other files included in the release
    EXTRA_FILES = \
        $${_PRO_FILE_PWD_}/../../*.txt \
        $${_PRO_FILE_PWD_}/../../tools/*.nsi

    linux-g++{
        for(FILE,EXTRA_FILES){
            QMAKE_POST_LINK += $$quote(cp $${FILE} $${targetdir} $$escape_expand(\n\t))
        }
    }

win32-msvc* {
    RC_FILE = vstboard.rc
}

    win32 {
        $${INSTALLER_UPDATEQTDLL} {
            EXTRA_FILES += $$[QT_INSTALL_BINS]/QtCore4.dll \
                    $$[QT_INSTALL_BINS]/QtGui4.dll
        }

        EXTRA_FILES_WIN = $${EXTRA_FILES}
        EXTRA_FILES_WIN ~= s,/,\\,g
        DESTDIR_WIN = $${targetdir}
        DESTDIR_WIN ~= s,/,\\,g
        for(FILE,EXTRA_FILES_WIN){
            QMAKE_POST_LINK += copy /y \"$${FILE}\" \"$${DESTDIR_WIN}\" $$escape_expand(\n\t)
        }

    #run mpress
#x64 version is buggy, pack the 32bit only
contains(QMAKE_HOST.arch, x86_64):{
        exists($$INSTALLER_MPRESS_PATH) {
            exists($$DESTDIR_WIN/VstBoard.exe) {
                QMAKE_POST_LINK += \"$$INSTALLER_MPRESS_PATH\" -q \"$$DESTDIR_WIN\\VstBoard.exe\" $$escape_expand(\n\t)
            }
            exists($$DESTDIR_WIN/VstBoardPlugin.dll) {
                QMAKE_POST_LINK += \"$$INSTALLER_MPRESS_PATH\" -q \"$$DESTDIR_WIN\\VstBoardPlugin.dll\" $$escape_expand(\n\t)
            }

            $${INSTALLER_UPDATEQTDLL} {
                QMAKE_POST_LINK += \"$$INSTALLER_MPRESS_PATH\" -q \"$$DESTDIR_WIN\\QtGui4.dll\" $$escape_expand(\n\t)
                QMAKE_POST_LINK += \"$$INSTALLER_MPRESS_PATH\" -q \"$$DESTDIR_WIN\\QtCode4.dll\" $$escape_expand(\n\t)
            }
        }
}

    #build installer
        exists($$INSTALLER_NSIS_PATH) {
            arc=win32
            contains(QMAKE_HOST.arch, x86_64):{
                arc=win64
            }
            QMAKE_POST_LINK += \"$$INSTALLER_NSIS_PATH\" /V1 /DARCH=$$arc \"$$DESTDIR_WIN\\nsis.nsi\" $$escape_expand(\n\t)
        }
    }
}

vstsdk { 
    DEFINES += VSTSDK
    INCLUDEPATH += $$top_srcdir/$$VSTSDK_PATH \
        $$top_srcdir/$$VSTSDK_PATH/public.sdk/source/vst2.x
    HEADERS += vst/cvsthost.h \
        connectables/vstplugin.h \
        vst/ceffect.h \
        vst/const.h \
        vst/vstbank.h \
        views/vstpluginwindow.h \
        views/vstshellselect.h
    SOURCES += vst/cvsthost.cpp \
        connectables/vstplugin.cpp \
        vst/ceffect.cpp \
        vst/vstbank.cpp \
        views/vstpluginwindow.cpp \
        views/vstshellselect.cpp
    FORMS += views/vstpluginwindow.ui \
        views/vstshellselect.ui
}
SOURCES += main.cpp \
    mainwindow.cpp \
    models/listaudiointerfacesmodel.cpp \
    connectables/pin.cpp \
    connectables/object.cpp \
    connectables/audiopinout.cpp \
    connectables/audiopinin.cpp \
    connectables/audiodevice.cpp \
    audiobuffer.cpp \
    audiobuffergroup.cpp \
    mainhost.cpp \
    projectfile/projectfile.cpp \
    connectables/audiodevicein.cpp \
    connectables/audiodeviceout.cpp \
    pathsolver.cpp \
    views/objectview.cpp \
    views/pinview.cpp \
    views/cableview.cpp \
    views/filebrowser.cpp \
    renderer.cpp \
    solvernode.cpp \
    views/solverscene.cpp \
    models/listmidiinterfacesmodel.cpp \
    connectables/mididevice.cpp \
    connectables/midipinin.cpp \
    connectables/midipinout.cpp \
    connectables/parameterpinin.cpp \
    views/listpinsview.cpp \
    connectables/miditoautomation.cpp \
    connectables/parameterpinout.cpp \
    connectables/midisender.cpp \
    connectables/connectioninfo.cpp \
    connectables/objectfactory.cpp \
    models/listtoolsmodel.cpp \
    connectables/hostcontroller.cpp \
    connectables/objectprogram.cpp \
    connectables/parameterpin.cpp \
    connectables/objectparameter.cpp \
    views/maingraphicsview.cpp \
    views/configdialog.cpp \
    connectables/container.cpp \
    views/connectableobjectview.cpp \
    views/bridgepinview.cpp \
    views/connectablepinview.cpp \
    views/containercontent.cpp \
    views/maincontainerview.cpp \
    connectables/bridge.cpp \
    views/sceneview.cpp \
    views/bridgeview.cpp \
    models/hostmodel.cpp \
    connectables/cable.cpp \
    connectables/containerprogram.cpp \
    projectfile/setupfile.cpp \
    audiodevices.cpp \
    connectables/bridgepinin.cpp \
    connectables/bridgepinout.cpp \
    connectables/maincontainer.cpp \
    connectables/objectinfo.cpp \
    views/aboutdialog.cpp \
    views/splash.cpp \
    mididevices.cpp \
    circularbuffer.cpp \
    views/programlist.cpp \
    programs.cpp \
    views/grouplistview.cpp \
    views/proglistview.cpp \
    models/programsmodel.cpp \
    connectables/pinslist.cpp \
    views/minmaxpinview.cpp \
    views/cursorview.cpp \
    audiobufferd.cpp
HEADERS += mainwindow.h \
    precomp.h \
    models/listaudiointerfacesmodel.h \
    connectables/pin.h \
    connectables/object.h \
    connectables/audiopinout.h \
    connectables/audiopinin.h \
    connectables/audiodevice.h \
    audiobuffer.h \
    audiobuffergroup.h \
    mainhost.h \
    projectfile/projectfile.h \
    globals.h \
    connectables/audiodevicein.h \
    connectables/audiodeviceout.h \
    pathsolver.h \
    views/objectview.h \
    views/pinview.h \
    views/cableview.h \
    views/filebrowser.h \
    renderer.h \
    solvernode.h \
    views/solverscene.h \
    models/listmidiinterfacesmodel.h \
    connectables/mididevice.h \
    connectables/midipinin.h \
    connectables/midipinout.h \
    connectables/parameterpinin.h \
    views/listpinsview.h \
    connectables/miditoautomation.h \
    connectables/parameterpinout.h \
    connectables/midisender.h \
    connectables/connectioninfo.h \
    connectables/objectfactory.h \
    models/listtoolsmodel.h \
    connectables/hostcontroller.h \
    connectables/objectprogram.h \
    connectables/parameterpin.h \
    connectables/objectparameter.h \
    views/maingraphicsview.h \
    views/configdialog.h \
    connectables/container.h \
    views/connectableobjectview.h \
    views/bridgepinview.h \
    views/connectablepinview.h \
    views/containercontent.h \
    views/maincontainerview.h \
    connectables/bridge.h \
    views/sceneview.h \
    views/bridgeview.h \
    models/hostmodel.h \
    connectables/cable.h \
    connectables/containerprogram.h \
    projectfile/setupfile.h \
    audiodevices.h \
    connectables/bridgepinin.h \
    connectables/bridgepinout.h \
    connectables/maincontainer.h \
    connectables/objectinfo.h \
    views/aboutdialog.h \
    views/splash.h \
    mididevices.h \
    circularbuffer.h \
    views/programlist.h \
    programs.h \
    views/grouplistview.h \
    views/proglistview.h \
    models/programsmodel.h \
    _version.h \
    connectables/pinslist.h \
    views/minmaxpinview.h \
    views/cursorview.h \
    projectfile/fileversion.h \
    audiobufferd.h
FORMS += mainwindow.ui \
    views/filebrowser.ui \
    views/configdialog.ui \
    views/aboutdialog.ui \
    views/splash.ui \
    views/programlist.ui
PRECOMPILED_HEADER = precomp.h
RESOURCES += ../resources/resources.qrc
OTHER_FILES += \ 
    ../../README.txt \
    ../../license.txt \
    ../../LGPL.txt \
    ../../GPL.txt \
    ../../tools/nsis.nsi \
    vstboard.rc

TRANSLATIONS = vstboard_fr.ts
