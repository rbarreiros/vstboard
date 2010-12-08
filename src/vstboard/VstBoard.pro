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

BUILDNO = $$system("git describe")

!CONFIG(debug, debug|release) {

    SRCDIR_WIN = $${_PRO_FILE_PWD_}
    SRCDIR_WIN ~= s,/,\\,g
    versionmk.target = version
    versionmk.commands = echo $${LITERAL_HASH}define APP_VERSION \"$${BUILDNO}\" > \"$${SRCDIR_WIN}\\_version.h\" $$escape_expand(\n\t)
    versionmk.depends =
    QMAKE_EXTRA_TARGETS += versionmk
    PRE_TARGETDEPS = version

    targetdir = $$OUT_PWD/../../bin/$$build_postfix
    builddir = $$OUT_PWD/../../build/$$build_postfix

#other files included in the release
    EXTRA_FILES = \
        $${_PRO_FILE_PWD_}/../../*.txt \
        $${_PRO_FILE_PWD_}/../../tools/*.nsi \
        $${_PRO_FILE_PWD_}/../../tools/*.qm \

    linux-g++{
        for(FILE,EXTRA_FILES){
            QMAKE_POST_LINK += $$quote(cp $${FILE} $${targetdir} $$escape_expand(\n\t))
        }
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

    #run upx
        exists($$INSTALLER_UPX_PATH) {
            exists($$DESTDIR_WIN/VstBoard.exe) {
                QMAKE_POST_LINK += \"$$INSTALLER_UPX_PATH\" -q --best \"$$DESTDIR_WIN\\VstBoard.exe\" $$escape_expand(\n\t)
            }
            exists($$DESTDIR_WIN/VstBoardPlugin.dll) {
                QMAKE_POST_LINK += \"$$INSTALLER_UPX_PATH\" -q --best \"$$DESTDIR_WIN\\VstBoardPlugin.dll\" $$escape_expand(\n\t)
            }

            $${INSTALLER_UPDATEQTDLL} {
                QMAKE_POST_LINK += \"$$INSTALLER_UPX_PATH\" -q --best \"$$DESTDIR_WIN\\*.dll\" $$escape_expand(\n\t)
            }
        }

    #build installer
        exists($$INSTALLER_NSIS_PATH) {
            QMAKE_POST_LINK += \"$$INSTALLER_NSIS_PATH\" /V1 \"$$DESTDIR_WIN\\nsis.nsi\" $$escape_expand(\n\t)
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
    views/outlinedtext.cpp \
    views/maingraphicsview.cpp \
    views/configdialog.cpp \
    views/textbutton.cpp \
    views/containerview.cpp \
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
    models/hostmodelproxy.cpp \
    audiodevices.cpp \
    connectables/bridgepinin.cpp \
    connectables/bridgepinout.cpp \
    connectables/parkingcontainer.cpp \
    views/combodelegate.cpp \
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
    imagecollection.cpp
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
    views/outlinedtext.h \
    views/maingraphicsview.h \
    views/configdialog.h \
    views/textbutton.h \
    views/containerview.h \
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
    models/hostmodelproxy.h \
    audiodevices.h \
    connectables/bridgepinin.h \
    connectables/bridgepinout.h \
    connectables/parkingcontainer.h \
    views/combodelegate.h \
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
    imagecollection.h \
    _version.h
FORMS += mainwindow.ui \
    views/filebrowser.ui \
    views/configdialog.ui \
    views/aboutdialog.ui \
    views/splash.ui \
    views/programlist.ui
PRECOMPILED_HEADER = precomp.h
RESOURCES += ../resources/resources.qrc
OTHER_FILES += \ 
    ../nsis.nsi \
    ../../README.txt \
    ../../license.txt \
    ../../LGPL.txt \
    ../../GPL.txt \
    ../../tools/nsis.nsi

TRANSLATIONS = vstboard_fr.ts
