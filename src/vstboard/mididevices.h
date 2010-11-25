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
    inline static MidiDevices *Get() {return theMidiDevices;}
    static MidiDevices *MidiDevices::Create(QObject *parent=0);
    ~MidiDevices();

    ListMidiInterfacesModel* GetModel();

    static void OpenDevice(QSharedPointer<Connectables::Object> objPtr);
    static void CloseDevice(QSharedPointer<Connectables::Object> objPtr);

private:
    explicit MidiDevices(QObject *parent=0);
    void BuildModel();
    static void MidiReceive_poll(PtTimestamp timestamp, void *userData);
    static QList< QSharedPointer<Connectables::MidiDevice> >listOpenedMidiDevices;

    ListMidiInterfacesModel *model;

    static QMutex mutexListMidi;
    static MidiDevices *theMidiDevices;
};

#endif // MIDIDEVICES_H
