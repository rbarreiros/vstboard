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
#include "mainhost.h"
#include "pa_asio.h"
#include "views/mmeconfigdialog.h"

/*!
  \class AudioDevices
  \brief manage the list of audio devices provided by PortAudio
  */

/*!
  \param myHost pointer to the MainHost
  */
AudioDevices::AudioDevices(MainHost *myHost) :
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
    foreach(QSharedPointer<Connectables::AudioDevice>ad, listAudioDevices) {
        ad->SetSleep(true);
    }

    if(model) {
//        debug("AudioDevices::~AudioDevices pa_terminate")
        PaError err=Pa_Terminate();
        if(err!=paNoError) {
            debug("AudioDevices::~AudioDevices Pa_Terminate %s",Pa_GetErrorText( err ))
        }
        model->deleteLater();
    }

    foreach(QSharedPointer<Connectables::AudioDevice>dev, listAudioDevices)
        dev->DeleteIfUnused();

    listAudioDevices.clear();
}

/*!
  Get the view model of the list
  \return pointer to the model
  */
ListAudioInterfacesModel * AudioDevices::GetModel()
{
    foreach(QSharedPointer<Connectables::AudioDevice>ad, listAudioDevices) {
        ad->SetSleep(true);
    }

    if(model) {
//        debug("AudioDevices::GetModel pa_terminate")
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

//    Connectables::AudioDevice::listDevMutex.lock();
    foreach(QSharedPointer<Connectables::AudioDevice>ad, listAudioDevices) {
        ad->SetSleep(false);
    }
//    Connectables::AudioDevice::listDevMutex.unlock();

    foreach(QSharedPointer<Connectables::Object>obj, myHost->objFactory->GetListObjects()) {
        if(obj.isNull())
            continue;

        if(obj->info().objType == ObjType::AudioInterfaceIn || obj->info().objType == ObjType::AudioInterfaceOut) {
//            if(!obj->errorMessage.isEmpty())
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
        const PaHostApiInfo *apiInfo = Pa_GetHostApiInfo(i);
        QStandardItem *apiItem = new QStandardItem(apiInfo->name);
        apiItem->setData( (quint8)apiInfo->type, UserRoles::value );
        apiItem->setDragEnabled(false);
        apiItem->setSelectable(false);
        parentItem->appendRow(apiItem);
        if(apiInfo->type == paASIO)
            AsioIndex = apiItem->index();

        //Devices

        //an api can contain multiple devices with the same name
        QString lastName;
        int cptDuplicateNames=0;

        for (int j=0; j<apiInfo->deviceCount; j++) {
            PaDeviceIndex devIndex = Pa_HostApiDeviceIndexToDeviceIndex(i, j);
            const PaDeviceInfo *devInfo = Pa_GetDeviceInfo( devIndex );

            QString devName(devInfo->name);
            //remove " x64" from device name so we can share files with 32bit version
            devName.remove(QRegExp("( )?x64"));

            //count duplicate names
            if(lastName == devName) {
                cptDuplicateNames++;
            } else {
                cptDuplicateNames=0;
                lastName = devName;
            }

            ObjectInfo obj;
            obj.nodeType = NodeType::object;
            obj.objType = ObjType::AudioInterface;
            obj.id = devIndex;
            obj.name = devName;
            obj.api = apiInfo->type;
            obj.duplicateNamesCounter = cptDuplicateNames;
            obj.inputs = devInfo->maxInputChannels;
            obj.outputs = devInfo->maxOutputChannels;

            QList<QStandardItem *> listItems;

            QStandardItem *devItem = new QStandardItem( devName);
            devItem->setEditable(false);
            devItem->setData(QVariant::fromValue(obj), UserRoles::objInfo);
            devItem->setDragEnabled(true);
            listItems << devItem;

            QStandardItem *inputItem = new QStandardItem( QString::number(devInfo->maxInputChannels));
            inputItem->setEditable(false);
            listItems << inputItem;

            QStandardItem *outputItem = new QStandardItem( QString::number(devInfo->maxOutputChannels));
            outputItem->setEditable(false);
            listItems << outputItem;

            QStandardItem *inUseItem = new QStandardItem();
            inUseItem->setCheckable(true);
            inUseItem->setCheckable(false);
            inUseItem->setEditable(false);
            listItems << inUseItem;

            apiItem->appendRow( listItems );
        }
    }
}

/*!
  Called when a device is added or removed from a panel
  \param objInfo object description
  \param opened true if opened, false if closed
  */
void AudioDevices::OnToggleDeviceInUse(const ObjectInfo &objInfo, bool opened)
{

    //find API item
    QStandardItem *apiItem = 0;
    int nbApi = model->invisibleRootItem()->rowCount();
    int apiCount = 0;
    while(!apiItem && apiCount<nbApi) {
        if(model->item(apiCount,0)->data(UserRoles::value).toInt() == objInfo.api )
            apiItem = model->item(apiCount,0);
        apiCount++;
    }

    if(!apiItem) {
        debug2(<< "AudioDevices::OnToggleDeviceInUse API not found " << objInfo.api << objInfo.name)
        return;
    }

    //find device item
    QStandardItem *devItem = 0;
    int nbDev = apiItem->rowCount();
    int devCount = 0;
    while(!devItem && devCount<nbDev) {
        ObjectInfo info = apiItem->child(devCount,0)->data(UserRoles::objInfo).value<ObjectInfo>();
        if(info.id == objInfo.id)
            devItem = apiItem->child(devCount,0);
        devCount++;
    }

    if(!devItem) {
        debug2(<< "AudioDevices::OnToggleDeviceInUse device not found " << objInfo.api << objInfo.name)
        return;
    }

    //change status
    if(opened) {
        apiItem->child( devItem->row(), 3)->setCheckState(Qt::Checked);
        countActiveDevices++;
    } else {
        apiItem->child( devItem->row(), 3)->setCheckState(Qt::Unchecked);
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
}

void AudioDevices::ConfigDevice(const QModelIndex &dev)
{
    if(!dev.data(UserRoles::objInfo).isValid())
        return;

    ObjectInfo info = dev.data(UserRoles::objInfo).value<ObjectInfo>();
    if(info.api == paASIO) {
        PaError err;
#if WIN32
        err = PaAsio_ShowControlPanel( info.id, (void*)myHost->mainWindow );
#endif
#ifdef __APPLE__
        err = PaAsio_ShowControlPanel( info.id, (void*)0 );
#endif

        if( err != paNoError ) {
            QMessageBox msg(QMessageBox::Warning,
                            tr("Error"),
                            Pa_GetErrorText( err ),
                            QMessageBox::Ok);
            msg.exec();
        }
        return;
    }

    if(info.api == paMME) {
        MmeConfigDialog dlg( info.name, myHost );
        dlg.exec();
        return;
    }

    QMessageBox msg(QMessageBox::Information,
                    tr("No config"),
                    tr("No config dialog for this device"),
                    QMessageBox::Ok);
    msg.exec();
}
