/**************************************************************************
#    Copyright 2010-2011 Raphaël François
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
**************************************************************************/

#ifndef KEYBIND_H
#define KEYBIND_H

#include "precomp.h"

class KeyBind : public QObject
{
    Q_OBJECT
    Q_ENUMS(MainShortcuts)
    Q_ENUMS(MoveInputs)
    Q_ENUMS(MovesBindings)

public:

    enum MainShortcuts {
        openProject,
        saveProject,
        saveProjectAs,
        newProject,
        openSetup,
        saveSetup,
        saveSetupAs,
        newSetup,
        hostPanel,
        projectPanel,
        programPanel,
        groupPanel,
        defaultLayout,
        toolBar,
        tools,
        vstPlugins,
        browser,
        programs,
        midiDevices,
        audioDevices,
        solverModel,
        hostModel,
        undoHistory,
        configuration,
        appearence,
        undo,
        redo,
        refreashAudioDevices,
        refreashMidiDevices
        /*
        zoomIn,
        zoomOut,
        zoomDefault,
        deleteObject,
        deleteObjectWithCables,
        unplugPin,
        increaseValue,
        decreaseValue

        */
    };

    enum MoveInputs {
        none,
        mouse,
        mouseWheel
    };

    enum MovesBindings {
        zoom,
        zoomReset,
        moveView,
        moveObject,
        createCable,
        changeValue,
        changeCursorValue
    };

    struct MoveBind {
        KeyBind::MoveInputs input;
        Qt::MouseButtons buttons;
        Qt::KeyboardModifier modifier;
    };

    KeyBind(QObject *parent=0);
    const QString GetMainShortcut(const MainShortcuts id) const;
    const MoveBind GetMoveSortcuts(const MovesBindings id) const;
    QStandardItemModel * GetMainBindingModel();
    QStandardItemModel * GetModesModel();
private:
    QMap<MainShortcuts, QString>mapMainShortcuts;
    QMap<QString, QMap<MovesBindings, MoveBind> >mapModes;
    QStandardItemModel mainModel;
    QStandardItemModel modesModel;
    QString currentMode;
};

#endif // KEYBIND_H
