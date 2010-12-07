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
