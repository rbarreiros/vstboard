/******************************************************************************
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
******************************************************************************/

#ifndef GLOBALS_H
#define GLOBALS_H

#include "precomp.h"

#define EMPTY_PROGRAM 0xffff
#define TEMP_PROGRAM 0xfffe

#define SETUP_FILE_EXTENSION "ctrls"
#define PROJECT_FILE_EXTENSION "ctrlp"
#define NB_RECENT_FILES 5

namespace UserRoles {
    enum Enum {
        programGroup = Qt::UserRole+1,
        programNumber,
        shortcut,
        connectionInfo,
        value,
        stepSize,
        falloff,
        position,
        size,
        parking,
        objInfo
    };
}
namespace NodeType {
    enum Enum {
        ND,
        object,
        container,
        bridge,
        listPin,
        pin,
        cable,
        editor
    };
}

namespace ObjType {
    enum Enum {
        ND,
        AudioInterface,
        AudioInterfaceIn,
        AudioInterfaceOut,
        MidiInterface,
        VstPlugin,
        MidiToAutomation,
        MidiSender,
        HostController,
        MainContainer,
        ParkingContainer,
        Container,
        BridgeIn,
        BridgeOut,
        BridgeSend,
        BridgeReturn,
        listAudioIn,
        listAudioOut,
        listMidiIn,
        listMidiOut,
        listParamIn,
        listParamOut,
        listBridgeIn,
        listBridgeOut,
        dummy
    };
}

namespace FixedObjId {
    enum Enum {
        mainContainer=1,
        hostContainer,
        hostContainerSend,
        hostContainerReturn,
        projectContainer,
        projectContainerIn,
        projectContainerOut,
        projectContainerSend,
        projectContainerReturn,
        programContainer,
        programContainerIn,
        programContainerOut,
        parkingContainer
    };
}

namespace PinType {
    enum Enum {
        ND,
        Audio,
        Midi,
        Parameter,
        Bridge
    };
}

namespace PinDirection {
    enum Enum {
        ND,
        Input,
        Output
    };
}

namespace PinMessage {
    enum Enum {
        ND,
        AudioBufferToMix,
        AudioBufferReplace,
        ClearAudioBuffer,
        MidiMsg,
        ParameterValue
    };
}

namespace MidiConst {
     enum Enum {
        codeMask = 0xf0,
        channelMask = 0x0f,

        noteOff =   0x80,
        noteOn =    0x90,
        ctrl =      0xb0,
        prog =      0xc0,
    };
}

namespace DefaultLayers {
    enum Enum {
        host,
        project,
        program,
    };
}

namespace Event {
    const QEvent::Type ProgChange = QEvent::User;
    const QEvent::Type UpdateSolver = QEvent::Type(QEvent::User+1);
    const QEvent::Type PinMessage = QEvent::Type(QEvent::User+2);
    const QEvent::Type Shortcut = QEvent::Type(QEvent::User+3);
}

#endif // GLOBALS_H
