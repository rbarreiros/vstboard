#include "mididevices.h"
#include "connectables/objectinfo.h"


QList< QSharedPointer<Connectables::MidiDevice> >MidiDevices::listOpenedMidiDevices;
QMutex MidiDevices::mutexListMidi;

MidiDevices::MidiDevices():
        model(0)
{
    GetModel();
}

MidiDevices::~MidiDevices()
{
    model->deleteLater();
}

ListMidiInterfacesModel* MidiDevices::GetModel()
{
    mutexListMidi.lock();

    foreach(QSharedPointer<Connectables::MidiDevice>md, listOpenedMidiDevices) {
        md->SetSleep(true);
        md->CloseStream();
    }

    if(Pt_Started())
        Pt_Stop();

    if(model) {
        Pm_Terminate();
        model->deleteLater();
    }

    PmError pmRet = Pm_Initialize();
    if(pmRet!=pmNoError)
        debug(Pm_GetErrorText(pmRet));

    PtError ptRet = Pt_Start(1, MidiDevices::MidiReceive_poll,0);
    if(ptRet!=ptNoError)
        debug(QString("pterror %1").arg(ptRet).toAscii());

    BuildModel();

    foreach(QSharedPointer<Connectables::MidiDevice>md, listOpenedMidiDevices) {
        if(md->OpenStream())
            md->SetSleep(false);
    }

    mutexListMidi.unlock();

    return model;
}

void MidiDevices::OpenDevice(QSharedPointer<Connectables::Object> objPtr)
{
    mutexListMidi.lock();
    listOpenedMidiDevices << objPtr.staticCast<Connectables::MidiDevice>();
    mutexListMidi.unlock();
}

void MidiDevices::CloseDevice(QSharedPointer<Connectables::Object> objPtr)
{
    mutexListMidi.lock();
    listOpenedMidiDevices.removeAll( objPtr.staticCast<Connectables::MidiDevice>() );
    mutexListMidi.unlock();
}

void MidiDevices::BuildModel()
{
    QStringList headerLabels;
    headerLabels << "Name";
    headerLabels << "In";
    headerLabels << "Out";

    model = new ListMidiInterfacesModel();
    model->setHorizontalHeaderLabels(  headerLabels );
    QStandardItem *parentItem = model->invisibleRootItem();

    QString lastName;
    int cptDuplicateNames=0;

    for(int i=0;i<Pm_CountDevices();i++) {
        QList<QStandardItem *>  items;
        const PmDeviceInfo *devInfo = Pm_GetDeviceInfo(i);

        QString devName= QString::fromStdString(devInfo->name);
        if(lastName == devName) {
            cptDuplicateNames++;
        } else {
            cptDuplicateNames=0;
        }
        lastName = devName;

        ObjectInfo obj;
        obj.nodeType = NodeType::object;
        obj.objType = ObjType::MidiInterface;
        obj.id = i;
        obj.name = devName;
        obj.api = QString::fromStdString( devInfo->interf );
        obj.duplicateNamesCounter = cptDuplicateNames;
        obj.inputs = devInfo->input;
        obj.outputs = devInfo->output;

        items << new QStandardItem(devName);
        items << new QStandardItem(QString::number(devInfo->input));
        items << new QStandardItem(QString::number(devInfo->output));

        items[0]->setData(QVariant::fromValue(obj), UserRoles::objInfo);

        parentItem->appendRow(items);
    }

}

//midi interfaces entry point
//===============================
void MidiDevices::MidiReceive_poll(PtTimestamp timestamp, void *userData)
{
    PmEvent buffer;
    PmError result = pmNoError;

    MidiDevices::mutexListMidi.lock();
    foreach(QSharedPointer<Connectables::Object> objPtr, MidiDevices::listOpenedMidiDevices) {
        if(objPtr.isNull())
            continue;
        if(objPtr->GetSleep())
            continue;

        Connectables::MidiDevice *device = static_cast<Connectables::MidiDevice*>(objPtr.data());
        if(!device->stream)
           continue;

        //lock device while processing (no rendering, no delete)
        device->Lock();

        //it's a midi input
        if(device->devInfo->input) {
            do {
                result = Pm_Poll(device->stream);
                if (result) {
                    PmError rslt = (PmError)Pm_Read(device->stream, &buffer, 1);
                    if (rslt == pmBufferOverflow) {
                        debug(QString("midi buffer overflow on device %1 %2").arg(device->GetIndex()).arg(device->objectName()).toAscii())
                        continue;
                    }
                    if(rslt == 1 ) {
                        Pm_Enqueue(device->queue, &buffer);
                    } else {
                        debug(QString("midi in error on device %1 %2").arg(device->GetIndex()).arg(device->objectName()).toAscii())
                        continue;
                    }
                }
            } while (result);
        }

        //it's a midi output
        if(device->devInfo->output) {
            while (!Pm_QueueEmpty(device->queue)) {
                result = Pm_Dequeue(device->queue, &buffer);
                Pm_Write(device->stream, &buffer, 1);
            }
        }

        device->Unlock();
    }
    MidiDevices::mutexListMidi.unlock();
}
