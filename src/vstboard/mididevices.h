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

#ifndef MIDIDEVICES_H
#define MIDIDEVICES_H

#include "precomp.h"
#include "porttime.h"
#include "portmidi.h"
#include "models/listmidiinterfacesmodel.h"
#include "connectables/mididevice.h"

class MidiDevices : public QObject
{
Q_OBJECT
public:
    explicit MidiDevices(MainHost *myHost);
    ~MidiDevices();

    ListMidiInterfacesModel* GetModel();

    void OpenDevice(QSharedPointer<Connectables::Object> objPtr);
    void CloseDevice(QSharedPointer<Connectables::Object> objPtr);

private:
    void BuildModel();
    static void MidiReceive_poll(PtTimestamp timestamp, void *userData);
    QList< QSharedPointer<Connectables::MidiDevice> >listOpenedMidiDevices;

    ListMidiInterfacesModel *model;

    QMutex mutexListMidi;
    MainHost *myHost;
};

#endif // MIDIDEVICES_H
