#ifndef MIDIDEVICES_H
#define MIDIDEVICES_H

#include "precomp.h"
#include "porttime.h"
#include "portmidi.h"
#include "models/listmidiinterfacesmodel.h"
#include "connectables/mididevice.h"

class MidiDevices
{
public:
    MidiDevices();
    ~MidiDevices();

    ListMidiInterfacesModel* GetModel();

    static void OpenDevice(QSharedPointer<Connectables::Object> objPtr);
    static void CloseDevice(QSharedPointer<Connectables::Object> objPtr);

private:
    void BuildModel();
    static void MidiReceive_poll(PtTimestamp timestamp, void *userData);
    static QList< QSharedPointer<Connectables::MidiDevice> >listOpenedMidiDevices;

    ListMidiInterfacesModel *model;

    static QMutex mutexListMidi;
};

#endif // MIDIDEVICES_H
