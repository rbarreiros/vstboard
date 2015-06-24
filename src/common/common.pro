include(../config.pri)

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += staticlib

#Disable vst sdk unused warnings
QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter

vstsdk {
    HEADERS += vst/cvsthost.h \
        connectables/vstplugin.h \
        vst/ceffect.h \
        vst/const.h \
        vst/vstbank.h \
        vst/vstbankbase.h \
        vst/vstprogram.h \
        views/vstpluginwindow.h \
        views/vstshellselect.h

    SOURCES += vst/cvsthost.cpp \
        connectables/vstplugin.cpp \
        vst/ceffect.cpp \
        vst/vstbank.cpp \
        vst/vstbankbase.cpp \
        vst/vstprogram.cpp \
        views/vstpluginwindow.cpp \
        views/vstshellselect.cpp \
        $$VSTSDK_PATH/public.sdk/source/vst2.x/audioeffectx.cpp \
        $$VSTSDK_PATH/public.sdk/source/vst2.x/audioeffect.cpp

    FORMS += views/vstpluginwindow.ui \
        views/vstshellselect.ui
}

scriptengine {
    QT += script

    SOURCES += connectables/script.cpp \
        views/scripteditor.cpp \

    HEADERS += connectables/script.h \
        views/scripteditor.h \

    FORMS += views/scripteditor.ui
}

PRECOMPILED_HEADER = precomp.h

SOURCES += \
    mainhost.cpp \
    audiobuffer.cpp \
    mainwindow.cpp \
    circularbuffer.cpp \
    connectables/midipinin.cpp \
    connectables/midipinout.cpp \
    connectables/objectprogram.cpp \
    connectables/containerprogram.cpp \
    connectables/bridgepinin.cpp \
    connectables/bridgepinout.cpp \
    connectables/cable.cpp \
    connectables/objectparameter.cpp \
    connectables/objectfactory.cpp \
    connectables/object.cpp \
    connectables/container.cpp \
    connectables/connectioninfo.cpp \
    connectables/objectinfo.cpp \
    connectables/pin.cpp \
    connectables/parameterpin.cpp \
    connectables/parameterpinin.cpp \
    connectables/parameterpinout.cpp \
    connectables/bridge.cpp \
    connectables/miditoautomation.cpp \
    connectables/midisender.cpp \
    connectables/pinslist.cpp \
    connectables/hostcontroller.cpp \
    connectables/audiopin.cpp \
    connectables/buffer.cpp \
    renderer/pathsolver.cpp \
    renderer/renderer.cpp \
    renderer/solvernode.cpp \
    renderer/renderthread.cpp \
    renderer/optimizerstep.cpp \
    renderer/optimizer.cpp \
    renderer/optimizestepthread.cpp \
    renderer/renderernode.cpp \
    renderer/node.cpp \
    projectfile/projectfile.cpp \
    models/listtoolsmodel.cpp \
    models/hostmodel.cpp \
    models/programsmodel.cpp \
    commands/comdisconnectpin.cpp \
    commands/comaddobject.cpp \
    commands/comaddcable.cpp \
    commands/comremoveobject.cpp \
    commands/comchangeprogram.cpp \
    commands/comaddprogram.cpp \
    commands/comremoveprogram.cpp \
    commands/comremovegroup.cpp \
    commands/comaddgroup.cpp \
    commands/comchangeprogramitem.cpp \
    commands/comdiscardchanges.cpp \
    commands/comchangeautosave.cpp \
    commands/comremovepin.cpp \
    commands/comaddpin.cpp \
    views/keybindingdialog.cpp \
    views/buttonswidget.cpp \
    views/modifierswidget.cpp \
    renderer/updatedelays.cpp

SOURCES += \
    views/programlist.cpp \
    views/filebrowser.cpp \
    views/grouplistview.cpp \
    views/proglistview.cpp \
    views/aboutdialog.cpp \
    views/configdialog.cpp \
    views/maingraphicsview.cpp \
    views/splash.cpp \
    views/viewconfigdialog.cpp \
    views/viewconfig.cpp \
    views/listaudiodevicesview.cpp \
    views/gradientwidget.cpp \
    views/gradientwidgethue.cpp \
    views/keybind.cpp
	
SOURCES += \
    sceneview/connectablepinview.cpp \
    sceneview/objectview.cpp \
    sceneview/pinview.cpp \
    sceneview/sceneview.cpp \
    sceneview/cableview.cpp \
    sceneview/maincontainerview.cpp \
    sceneview/connectableobjectview.cpp \
    sceneview/bridgepinview.cpp \
    sceneview/containercontent.cpp \
    sceneview/bridgeview.cpp \
    sceneview/listpinsview.cpp \
    sceneview/minmaxpinview.cpp \
    sceneview/cursorview.cpp \
    sceneview/vstpluginview.cpp \
    sceneview/objectdropzone.cpp

HEADERS += \
    _version.h \
    globals.h \
    mainhost.h \
    audiobuffer.h \
    mainwindow.h \
    circularbuffer.h \
    connectables/midisender.h \
    connectables/miditoautomation.h \
    connectables/hostcontroller.h \
    connectables/midipinout.h \
    connectables/objectprogram.h \
    connectables/containerprogram.h \
    connectables/bridgepinin.h \
    connectables/bridgepinout.h \
    connectables/cable.h \
    connectables/objectparameter.h \
    connectables/objectfactory.h \
    connectables/object.h \
    connectables/container.h \
    connectables/connectioninfo.h \
    connectables/objectinfo.h \
    connectables/pin.h \
    connectables/parameterpin.h \
    connectables/parameterpinin.h \
    connectables/parameterpinout.h \
    connectables/bridge.h \
    connectables/midipinin.h \
    connectables/pinslist.h \
    connectables/audiopin.h \
    connectables/buffer.h \
    renderer/solvernode.h \
    renderer/pathsolver.h \
    renderer/renderer.h \
    renderer/renderthread.h \
    renderer/optimizerstep.h \
    renderer/optimizer.h \
    renderer/optimizestepthread.h \
    renderer/renderernode.h \
    renderer/node.h \
    projectfile/projectfile.h \
    projectfile/fileversion.h \
    models/listtoolsmodel.h \
    models/hostmodel.h \
    models/programsmodel.h \
    commands/comdisconnectpin.h \
    commands/comaddobject.h \
    commands/comaddcable.h \
    commands/comremoveobject.h \
    commands/comchangeprogram.h \
    commands/comaddprogram.h \
    commands/comremoveprogram.h \
    commands/comremovegroup.h \
    commands/comaddgroup.h \
    commands/comchangeprogramitem.h \
    commands/comdiscardchanges.h \
    commands/comchangeautosave.h \
    commands/comremovepin.h \
    commands/comaddpin.h \
    views/keybindingdialog.h \
    views/buttonswidget.h \
    views/modifierswidget.h \
    renderer/updatedelays.h

HEADERS += \
    views/configdialog.h \
    views/programlist.h \
    views/filebrowser.h \
    views/grouplistview.h \
    views/proglistview.h \
    views/aboutdialog.h \
    views/maingraphicsview.h \
    views/splash.h \
    views/viewconfigdialog.h \
    views/viewconfig.h \
    views/listaudiodevicesview.h \
    views/gradientwidget.h \
    views/gradientwidgethue.h \
    views/keybind.h
	
HEADERS += \
    sceneview/objectview.h \
    sceneview/pinview.h \
    sceneview/connectablepinview.h \
    sceneview/sceneview.h \
    sceneview/cableview.h \
    sceneview/maincontainerview.h \
    sceneview/connectableobjectview.h \
    sceneview/bridgepinview.h \
    sceneview/containercontent.h \
    sceneview/bridgeview.h \
    sceneview/listpinsview.h \
    sceneview/minmaxpinview.h \
    sceneview/cursorview.h \
    sceneview/vstpluginview.h \
    sceneview/objectdropzone.h
    
FORMS += \
    mainwindow.ui \
    views/configdialog.ui \
    views/filebrowser.ui \
    views/programlist.ui \
    views/aboutdialog.ui \
    views/splash.ui \
    views/viewconfigdialog.ui \
    views/keybindingdialog.ui \
    views/buttonswidget.ui \
    views/modifierswidget.ui


PRECOMPILED_HEADER = precomp.h

TRANSLATIONS = ../resources/translations/common_fr.ts

RESOURCES += \
    ../resources/resources.qrc





















