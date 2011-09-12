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

#include "keybind.h"

KeyBind::KeyBind(QObject *parent) :
    QObject(parent)
{
    mapMainShortcuts.insert(MainShortcuts::openProject,         "Ctrl+O");
    mapMainShortcuts.insert(MainShortcuts::saveProject,         "Ctrl+S");
    mapMainShortcuts.insert(MainShortcuts::saveProjectAs,       "Ctrl+Shift+S");
    mapMainShortcuts.insert(MainShortcuts::newProject,          "Ctrl+N");
    mapMainShortcuts.insert(MainShortcuts::openSetup,           "Alt+O");
    mapMainShortcuts.insert(MainShortcuts::saveSetup,           "Alt+S");
    mapMainShortcuts.insert(MainShortcuts::saveSetupAs,         "Alt+Shift+S");
    mapMainShortcuts.insert(MainShortcuts::newSetup,            "Alt+N");
    mapMainShortcuts.insert(MainShortcuts::hostPanel,           "Ctrl+H");
    mapMainShortcuts.insert(MainShortcuts::projectPanel,        "Ctrl+R");
    mapMainShortcuts.insert(MainShortcuts::programPanel,        "Ctrl+P");
    mapMainShortcuts.insert(MainShortcuts::groupPanel,          "Ctrl+G");
    mapMainShortcuts.insert(MainShortcuts::defaultLayout,       "");
    mapMainShortcuts.insert(MainShortcuts::toolBar,             "Ctrl+Alt+O");
    mapMainShortcuts.insert(MainShortcuts::tools,               "Ctrl+Alt+T");
    mapMainShortcuts.insert(MainShortcuts::vstPlugins,          "Ctrl+Alt+V");
    mapMainShortcuts.insert(MainShortcuts::browser,             "Ctrl+Alt+B");
    mapMainShortcuts.insert(MainShortcuts::programs,            "Ctrl+Alt+P");
    mapMainShortcuts.insert(MainShortcuts::midiDevices,         "Ctrl+Alt+M");
    mapMainShortcuts.insert(MainShortcuts::audioDevices,        "Ctrl+Alt+A");
    mapMainShortcuts.insert(MainShortcuts::solverModel,         "Ctrl+Alt+S");
    mapMainShortcuts.insert(MainShortcuts::hostModel,           "Ctrl+Alt+H");
    mapMainShortcuts.insert(MainShortcuts::undoHistory,         "Ctrl+Alt+U");
    mapMainShortcuts.insert(MainShortcuts::configuration,       "Ctrl+Alt+C");
    mapMainShortcuts.insert(MainShortcuts::appearence,          "Ctrl+Alt+E");
    mapMainShortcuts.insert(MainShortcuts::undo,                "Ctrl+Z");
    mapMainShortcuts.insert(MainShortcuts::redo,                "Ctrl+Y");
    mapMainShortcuts.insert(MainShortcuts::refreashAudioDevices,"Ctrl+F5");
    mapMainShortcuts.insert(MainShortcuts::refreashMidiDevices, "Alt+F5");

    {
        MoveBind b={MoveInputs::mouseWheel, Qt::NoButton, Qt::NoModifier};
        mapMoveShortcuts.insert( MovesBindings::zoom, b );
    }

    {
        MoveBind b={MoveInputs::ND, Qt::MiddleButton, Qt::NoModifier};
        mapMoveShortcuts.insert( MovesBindings::zoomReset, b );
    }

    {
        MoveBind b={MoveInputs::mouse, Qt::LeftButton, Qt::NoModifier};
        mapMoveShortcuts.insert( MovesBindings::moveView, b );
    }

    {
        MoveBind b={MoveInputs::mouse, Qt::LeftButton, Qt::NoModifier};
        mapMoveShortcuts.insert( MovesBindings::moveObject, b );
    }

    {
        MoveBind b={MoveInputs::mouse, Qt::LeftButton, Qt::NoModifier};
        mapMoveShortcuts.insert( MovesBindings::createCable, b );
    }

    {
        MoveBind b={MoveInputs::mouse, Qt::LeftButton, Qt::ControlModifier};
        mapMoveShortcuts.insert( MovesBindings::changeValue, b );
    }

    {
        MoveBind b={MoveInputs::mouse, Qt::LeftButton, Qt::NoModifier};
        mapMoveShortcuts.insert( MovesBindings::changeCursorValue, b );
    }
}

QString KeyBind::GetMainShortcut(MainShortcuts::Enum id)
{
    return mapMainShortcuts.value(id,"");
}

const MoveBind KeyBind::GetMoveSortcuts(const MovesBindings::Enum id) const
{
    if(!mapMoveShortcuts.contains(id)) {
        LOG("move id not found");
        return MoveBind();
    }
    return mapMoveShortcuts.value(id);
}
