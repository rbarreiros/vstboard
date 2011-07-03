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
#include "mididevices.h"
#include "connectables/objectinfo.h"
#include "mainhost.h"

//QList< QSharedPointer<Connectables::MidiDevice> >MidiDevices::listOpenedMidiDevices;
//QMutex MidiDevices::mutexListMidi;

MidiDevices::MidiDevices(MainHost *myHost) :
        QObject(myHost),
        model(0),
        myHost(myHost)
{
    //GetModel();
}

MidiDevices::~MidiDevices()
{
    if(Pt_Started())
        Pt_Stop();

//    if(model) {
//        model->deleteLater();
        Pm_Terminate();
//    }
}

ListMidiInterfacesModel* MidiDevices::GetModel()
{
//    mutexListMidi.lock();

    foreach(Connectables::MidiDevice* md, listOpenedMidiDevices) {
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
    if(pmRet!=pmNoError) {
        QMessageBox msgBox;
        msgBox.setText(tr("Unable to initialize midi engine : %1").arg( Pm_GetErrorText(pmRet) ));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return 0;
    }

    PtError ptRet = Pt_Start(1, MidiDevices::MidiReceive_poll,this);
    if(ptRet!=ptNoError) {
        QMessageBox msgBox;
        msgBox.setText(tr("Unable to start midi engine"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return 0;
    }

    BuildModel();

    foreach(QSharedPointer<Connectables::Object>obj, myHost->objFactory->GetListObjects()) {
        if(obj.isNull())
            continue;

        if(obj->info().objType == ObjType::MidiInterface) {
//            if(!obj->errorMessage.isEmpty())
                obj->Open();
            obj->UpdateModelNode();
        }
    }

    foreach(Connectables::MidiDevice* md, listOpenedMidiDevices) {
        if(md->OpenStream())
            md->SetSleep(false);
    }

//    mutexListMidi.unlock();

    return model;
}

void MidiDevices::OpenDevice(Connectables::MidiDevice* objPtr)
{
//    mutexListMidi.lock();
    listOpenedMidiDevices << objPtr;
//    mutexListMidi.unlock();
}

void MidiDevices::CloseDevice(Connectables::MidiDevice* objPtr)
{
//    mutexListMidi.lock();
    listOpenedMidiDevices.removeAll( objPtr );
//    mutexListMidi.unlock();
}

void MidiDevices::BuildModel()
{
    QStringList headerLabels;
    headerLabels << "Name";
    headerLabels << "In";
    headerLabels << "Out";

    if(model)
        model->deleteLater();
    model = new ListMidiInterfacesModel(this);
    model->setHorizontalHeaderLabels(  headerLabels );
    QStandardItem *parentItem = model->invisibleRootItem();

    QString lastName;
    int cptDuplicateNames=0;

    for(int i=0;i<Pm_CountDevices();i++) {
        QList<QStandardItem *>  items;
        const PmDeviceInfo *devInfo = Pm_GetDeviceInfo(i);

        QString devName= QString::fromLocal8Bit(devInfo->name);
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
        obj.apiName = QString::fromLocal8Bit(devInfo->interf );
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

    MidiDevices *devices = static_cast<MidiDevices*>(userData);

//    devices->mutexListMidi.lock();
    foreach(Connectables::MidiDevice* device, devices->listOpenedMidiDevices) {
        if(device->GetSleep())
            continue;

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
//    devices->mutexListMidi.unlock();
}
