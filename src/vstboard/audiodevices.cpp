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

#include "audiodevices.h"
#include "globals.h"
#include "connectables/objectinfo.h"
#include "mainhosthost.h"

/*!
  \class AudioDevices
  \brief manage the list of audio devices provided by PortAudio
  */

/*!
  \param myHost pointer to the MainHost
  */
AudioDevices::AudioDevices(MainHostHost *myHost) :
        QObject(myHost),
        model(0),
        countActiveDevices(0),
        myHost(myHost)
{
    fakeRenderTimer.start(FAKE_RENDER_TIMER_MS);
}

/*!
  Destructor, close all the opened devices
  */
AudioDevices::~AudioDevices()
{
    mutexDevices.lock();
    foreach(Connectables::AudioDevice *ad, listAudioDevices) {
        ad->SetSleep(true);
    }
    mutexDevices.unlock();

    if(model) {
        PaError err=Pa_Terminate();
        if(err!=paNoError) {
            debug("AudioDevices::~AudioDevices Pa_Terminate %s",Pa_GetErrorText( err ))
        }
        model->deleteLater();
    }

    mutexDevices.lock();
    foreach(Connectables::AudioDevice *dev, listAudioDevices)
        dev->DeleteIfUnused();
    listAudioDevices.clear();
    mutexDevices.unlock();
}

/*!
  Get the view model of the list
  \return pointer to the model
  */
ListAudioInterfacesModel * AudioDevices::GetModel()
{
    mutexDevices.lock();
    foreach(Connectables::AudioDevice *ad, listAudioDevices) {
        ad->SetSleep(true);
    }
    mutexDevices.unlock();

    if(model) {
        PaError err=Pa_Terminate();
        if(err!=paNoError) {
            debug("AudioDevices::GetModel Pa_Terminate %s",Pa_GetErrorText( err ))
        }
        model->invisibleRootItem()->removeRows(0, model->invisibleRootItem()->rowCount());
    }


    PaError paRet =Pa_Initialize();
    if(paRet!=paNoError) {
        QMessageBox msgBox;
        msgBox.setText(tr("Unable to initialize audio engine : %1").arg( Pa_GetErrorText(paRet) ));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return 0;
    }
    BuildModel();

    mutexDevices.lock();
    foreach(Connectables::AudioDevice *ad, listAudioDevices) {
        ad->SetSleep(false);
    }
    mutexDevices.unlock();

    foreach(QSharedPointer<Connectables::Object>obj, myHost->objFactory->GetListObjects()) {
        if(obj.isNull())
            continue;

        if(obj->info().objType == ObjType::AudioInterfaceIn || obj->info().objType == ObjType::AudioInterfaceOut) {
            obj->Open();
            obj->UpdateModelNode();
        }
    }

    return model;
}

/*!
  Create or update the list of devices
  */
void AudioDevices::BuildModel()
{
    if(!model)
        model = new ListAudioInterfacesModel();

    QStringList headerLabels;
    headerLabels << "Name";
    headerLabels << "In";
    headerLabels << "Out";
    headerLabels << "InUse";
    model->setHorizontalHeaderLabels(  headerLabels );

    QStandardItem *parentItem = model->invisibleRootItem();

    //APIs
    for (int i = 0; i < Pa_GetHostApiCount(); ++i) {
        QStandardItem *api = new QStandardItem(Pa_GetHostApiInfo(i)->name);
        api->setDragEnabled(false);
        api->setSelectable(false);
        parentItem->appendRow(api);
        if(api->text() == "ASIO")
            AsioIndex = api->index();
    }

    //devices
    QString lastName;
    int cptDuplicateNames=0;

    for (int i = 0; i < Pa_GetDeviceCount(); ++i) {
        QList<QStandardItem *>  items;
        const PaDeviceInfo *devInfo = Pa_GetDeviceInfo(i);

        QString devName(devInfo->name);

        //remove " x64" from device name so we can share files with 32bit version
        devName.remove(QRegExp("( )?x64"));

        if(lastName == devName) {
            cptDuplicateNames++;
        } else {
            cptDuplicateNames=0;
        }
        lastName = devName;

        ObjectInfo obj;
        obj.nodeType = NodeType::object;
        obj.objType = ObjType::AudioInterface;
        obj.id = i;
        obj.name = devName;
        obj.api = QString::fromStdString( Pa_GetHostApiInfo(devInfo->hostApi)->name );
        obj.duplicateNamesCounter = cptDuplicateNames;
        obj.inputs = devInfo->maxInputChannels;
        obj.outputs = devInfo->maxOutputChannels;

        QStandardItem *dev = new QStandardItem( devName);
        dev->setEditable(false);
        dev->setData(QVariant::fromValue(obj), UserRoles::objInfo);
        dev->setDragEnabled(true);
        items << dev;

        QStandardItem *ins = new QStandardItem( QString::number(devInfo->maxInputChannels));
        ins->setEditable(false);
        items << ins;

        QStandardItem *outs = new QStandardItem( QString::number(devInfo->maxOutputChannels));
        outs->setEditable(false);
        items << outs;

        QStandardItem *inUse = new QStandardItem();
        inUse->setCheckable(true);
        inUse->setCheckable(false);
        inUse->setEditable(false);
        items << inUse;

        QStandardItem *parent = model->item(devInfo->hostApi,0);
        parent->appendRow(items);
    }
}

/*!
  Called when a device is added or removed from a panel
  \param objInfo object description
  \param opened true if opened, false if closed
  */
void AudioDevices::OnToggleDeviceInUse(const ObjectInfo &objInfo, bool opened)
{
    for(int i=0; i<model->invisibleRootItem()->rowCount();i++) {
        QStandardItem *itemApi = model->item(i,0);
        if(itemApi->data(Qt::DisplayRole).toString()==objInfo.api) {
            for(int j=0; j<itemApi->rowCount(); j++) {
                QStandardItem *itemDev = itemApi->child(j,0);
                ObjectInfo info = itemDev->data(UserRoles::objInfo).value<ObjectInfo>();
                if(info.id == objInfo.id) {
                    if(opened) {
                        itemApi->child(j,3)->setCheckState(Qt::Checked);
                        countActiveDevices++;
                    } else {
                        itemApi->child(j,3)->setCheckState(Qt::Unchecked);
                        countActiveDevices--;
                    }

                    //the renderer is normally launched when all the audio devices are ready,
                    //if there is no audio device we have to run a timer
                    if(countActiveDevices==1) {
                        fakeRenderTimer.stop();
                    }
                    if(countActiveDevices==0) {
                        fakeRenderTimer.start(FAKE_RENDER_TIMER_MS);
                    }
                    return;
                }
            }
        }
    }
}

Connectables::AudioDevice * AudioDevices::AddDevice(const ObjectInfo &objInfo, QString *errMsg)
{
    PaDeviceInfo PAinfo;
    int devId = FindPortAudioDevice(objInfo, PAinfo);
    if(devId==-1) {
        errMsg->append(tr("Device not found"));
        return 0;
    }



    mutexDevices.lock();
    Connectables::AudioDevice *dev = listAudioDevices.value(devId,0);
    mutexDevices.unlock();

    if(!dev) {
        dev = new Connectables::AudioDevice(devId, PAinfo, myHost,objInfo);
        if(!dev->Open()) {
            if(errMsg)
                errMsg->append(dev->errorMessage);
            delete dev;
            return 0;
        }
        mutexDevices.lock();
        listAudioDevices.insert(devId, dev);
        mutexDevices.unlock();
    }
    return dev;
}

void AudioDevices::RemoveDevice(PaDeviceIndex devId)
{
    mutexDevices.lock();
    Connectables::AudioDevice *dev = listAudioDevices.take(devId);
    mutexDevices.unlock();
    if(dev) {
        delete dev;
    }
}

void AudioDevices::PutPinsBuffersInRingBuffers()
{
    mutexDevices.lock();
    foreach(Connectables::AudioDevice *dev, listAudioDevices) {
        dev->PinsToRingBuffers();
    }
    mutexDevices.unlock();
}

/*!
  Try to find a device in the list returned by PortAudio
  \param[in] objInfo the ObjectInfo we're looking for
  \param[out] devInfo the PaDeviceInfo of the object found
  \return true if found
  */
PaDeviceIndex AudioDevices::FindPortAudioDevice(const ObjectInfo &objInfo, PaDeviceInfo &devInfo)
{
    int cptDuplicateNames=0;
    int canBe=-1;
    PaDeviceIndex deviceNumber=-1;

    for(int i=0;i<Pa_GetDeviceCount();i++) {
        const PaDeviceInfo *info = Pa_GetDeviceInfo(i);
        QString devName = QString::fromStdString(info->name);
        //remove " x64" from device name so we can share files with 32bit version
        devName.remove(QRegExp("( )?x64"));

        if(devName == objInfo.name
           && info->maxInputChannels == objInfo.inputs
           && info->maxOutputChannels == objInfo.outputs) {
            //can be this one, but the interface number can change form a comp to another
            if(cptDuplicateNames==0)
                canBe=i;

            //we found the same number and the same name
            if(objInfo.duplicateNamesCounter == cptDuplicateNames) {
                devInfo = *info;
                deviceNumber = i;
                break;
            }
            cptDuplicateNames++;
        }
    }

    //didn't found an exact match
    if(deviceNumber==-1) {
        //but we found a device with the same name
        if(canBe!=-1) {
            deviceNumber=canBe;
            devInfo = *Pa_GetDeviceInfo(deviceNumber);
        } else {
            debug("AudioDevice::FindDeviceByName device not found")
            return -1;
        }
    }

    return deviceNumber;
}
