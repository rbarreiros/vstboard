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

namespace MainShortcuts {
    enum Enum {
        ND,
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
}

namespace MovesBindings {
    enum Enum {
        ND,
        zoom,
        zoomReset,
        moveView,
        moveObject,
        createCable,
        changeValue,
        changeCursorValue
    };
}

namespace MoveInputs {
    enum Enum {
        ND,
        mouse,
        mouseWheel
    };
}

struct MoveBind {
    MoveInputs::Enum input;
    Qt::MouseButtons buttons;
    Qt::KeyboardModifier modifier;
};

class KeyBind : public QObject
{
    Q_OBJECT
public:
    KeyBind(QObject *parent=0);
    QString GetMainShortcut(MainShortcuts::Enum id);
    const MoveBind GetMoveSortcuts(const MovesBindings::Enum id) const;

private:
    QMap<MainShortcuts::Enum, QString>mapMainShortcuts;
    QMap<MovesBindings::Enum, MoveBind>mapMoveShortcuts;
};

#endif // KEYBIND_H
