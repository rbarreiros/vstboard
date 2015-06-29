/**************************************************************************
#    Copyright 2010-2011 Rapha�l Fran�ois
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
#include "pa_asio.h"
#include "views/mmeconfigdialog.h"
#include "views/wasapiconfigdialog.h"
#include "connectables/audiodevicein.h"
#include "connectables/audiodeviceout.h"

FakeTimer::FakeTimer(MainHostHost *myHost) :
    QThread(myHost),
    myHost(myHost),
    stop(false)
{
    setObjectName("FakeTimer");
    myHost->SetBufferSize((myHost->GetSampleRate()*FAKE_RENDER_TIMER_MS)/1000);
    start(QThread::TimeCriticalPriority);
}

FakeTimer::~FakeTimer()
{
    //LOG("stop thread"<<objectName()<<(int)currentThreadId());
    LOG("stop thread"<<objectName());
    stop=true;
    wait(1000);
}

void FakeTimer::run()
{

    //LOG("start thread"<<objectName()<<(int)currentThreadId());
    LOG("start thread"<<objectName());
    while(!stop) {
        msleep(FAKE_RENDER_TIMER_MS);
        myHost->Render();
    }
    stop=false;
}

/*!
  \class AudioDevices
  \brief manage the list of audio devices provided by PortAudio
  */

/*!
  \param myHost pointer to the MainHost
  */
AudioDevices::AudioDevices(MainHostHost *myHost) :
    QObject(myHost),
    closing(false),
    model(0),
    countActiveDevices(0),
    myHost(myHost),
    fakeRenderTimer(0)
{
    fakeRenderTimer = new FakeTimer(myHost);
}

/*!
  Destructor, close all the opened devices
  */
AudioDevices::~AudioDevices()
{
    CloseDevices(true);

    mutexDevices.lock();
    foreach(Connectables::AudioDevice *dev, listAudioDevices)
        delete dev; //dev->DeleteIfUnused();
    listAudioDevices.clear();
    mutexDevices.unlock();

    if(fakeRenderTimer)
        delete fakeRenderTimer;
}

void AudioDevices::CloseDevices(bool close)
{
    mutexClosing.lock();
    closing=true;
    mutexClosing.unlock();

    mutexDevices.lock();
    foreach(Connectables::AudioDevice *ad, listAudioDevices) {
        ad->SetSleep(true);
    }
    mutexDevices.unlock();

    if(!close) {
        mutexClosing.lock();
        closing=false;
        mutexClosing.unlock();
    }

    if(model) {
        PaError err=Pa_Terminate();
        if(err!=paNoError) {
            LOG("Pa_Terminate"<<Pa_GetErrorText( err ));
        }
        if(!close)
            model->invisibleRootItem()->removeRows(0, model->invisibleRootItem()->rowCount());
    }
}

void AudioDevices::OpenDevices()
{
    model=0;
    PaError paRet =Pa_Initialize();
    if(paRet!=paNoError) {
        QMessageBox msgBox;
        msgBox.setText(tr("Unable to initialize audio engine : %1").arg( Pa_GetErrorText(paRet) ));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }
    BuildModel();

    mutexClosing.lock();
    closing=true;
    mutexClosing.unlock();

    mutexDevices.lock();
    foreach(Connectables::AudioDevice *ad, listAudioDevices) {
        ad->SetSleep(false);
    }
    mutexDevices.unlock();

    mutexClosing.lock();
    closing=false;
    mutexClosing.unlock();


    //rebuild all audio in&out objects
    foreach(QSharedPointer<Connectables::Object>obj, myHost->objFactory->GetListObjects()) {
        if(obj.isNull())
            continue;

        ObjectInfo info( obj->info() );
        if(info.objType == ObjType::AudioInterfaceIn || info.objType == ObjType::AudioInterfaceOut) {
            QString errMsg;
            Connectables::AudioDevice *newDevice = AddDevice( info, &errMsg );
            if(info.objType == ObjType::AudioInterfaceIn)
                static_cast<Connectables::AudioDeviceIn*>(obj.data())->SetParentDevice(newDevice);
            if(info.objType == ObjType::AudioInterfaceOut)
                static_cast<Connectables::AudioDeviceOut*>(obj.data())->SetParentDevice(newDevice);
            if(obj->Open()) {
                obj->UpdateModelNode();
            } else {
                static_cast<Connectables::Container*>(myHost->objFactory->GetObjectFromId( obj->GetContainerId() ).data())->UserParkObject( obj );
            }
            obj->SetErrorMessage(errMsg);
        }
    }
}

/*!
  Get the view model of the list
  \return pointer to the model
  */
ListAudioInterfacesModel * AudioDevices::GetModel()
{
    CloseDevices();
    OpenDevices();
    return model;
}

/*!
  Create or update the list of devices
  */
void AudioDevices::BuildModel()
{
    if(!model)
        model = new ListAudioInterfacesModel(this);

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
void AudioDevices::OnToggleDeviceInUse(PaHostApiIndex apiId, PaDeviceIndex devId, bool inUse, PaTime inLatency, PaTime outLatency, double sampleRate)
{

    //find API item
    QStandardItem *apiItem = 0;
    int nbApi = model->invisibleRootItem()->rowCount();
    int apiCount = 0;
    while(!apiItem && apiCount<nbApi) {
        if(model->item(apiCount,0)->data(UserRoles::value).toInt() == apiId )
            apiItem = model->item(apiCount,0);
        apiCount++;
    }

    if(!apiItem) {
        LOG("API not found"<<apiId);
        return;
    }

    //find device item
    QStandardItem *devItem = 0;
    int nbDev = apiItem->rowCount();
    int devCount = 0;
    while(!devItem && devCount<nbDev) {
        ObjectInfo info = apiItem->child(devCount,0)->data(UserRoles::objInfo).value<ObjectInfo>();
        if(info.id == devId)
            devItem = apiItem->child(devCount,0);
        devCount++;
    }

    if(!devItem) {
        LOG("device not found"<<apiId<<devId);
        return;
    }

    //change status
    QStandardItem *chk = apiItem->child( devItem->row(), 3);

    if(inUse) {
        if(chk->checkState()!=Qt::Checked) {
            apiItem->child( devItem->row(), 3)->setCheckState(Qt::Checked);
            countActiveDevices++;
        }

        int inL = ceil(inLatency*1000);
        int outL = ceil(outLatency*1000);
        devItem->setToolTip( QString("Input latency %1ms\nOutput latency %2ms\nSample rate %3Hz")
                             .arg(inL).arg(outL).arg(sampleRate) );
    } else {
        if(chk->checkState()==Qt::Checked) {
            chk->setCheckState(Qt::Unchecked);
            countActiveDevices--;
        }
        devItem->setToolTip("");
    }

    qDebug()<<"countActiveDevices"<<countActiveDevices<<fakeRenderTimer;

    //the renderer is normally launched when all the audio devices are ready,
    //if there is no audio device we have to run a timer
    if(countActiveDevices>0) {
        myHost->SetBufferSize(1);
        if(fakeRenderTimer) {
            delete fakeRenderTimer;
            fakeRenderTimer=0;
        }
    }
    if(countActiveDevices==0) {
        myHost->SetBufferSizeMs(FAKE_RENDER_TIMER_MS);
        if(!fakeRenderTimer && !closing)
            fakeRenderTimer = new FakeTimer(myHost);
    }
}

Connectables::AudioDevice * AudioDevices::AddDevice(ObjectInfo &objInfo, QString *errMsg)
{
    PaDeviceInfo PAinfo;

    if(!FindPortAudioDevice(objInfo, &PAinfo)) {
        errMsg->append(tr("Device not found"));
        return 0;
    }

    mutexDevices.lock();
    Connectables::AudioDevice *dev = listAudioDevices.value(objInfo.id,0);
    mutexDevices.unlock();

    if(!dev) {
        dev = new Connectables::AudioDevice(PAinfo, myHost,objInfo);
        if(!dev->Open()) {
            if(errMsg)
                errMsg->append(dev->errorMessage);
            delete dev;
            return 0;
        }
        mutexDevices.lock();
        listAudioDevices.insert(objInfo.id, dev);
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

#ifdef CIRCULAR_BUFFER
void AudioDevices::PutPinsBuffersInRingBuffers()
{
    mutexClosing.lock();
    if(closing) {
        mutexClosing.unlock();
        return;
    }

    mutexDevices.lock();
    foreach(Connectables::AudioDevice *dev, listAudioDevices) {
        dev->PinsToRingBuffers();
    }
    mutexDevices.unlock();

    mutexClosing.unlock();
}
#endif
/*!
  Try to find a device in the list returned by PortAudio
  \param[in] objInfo the ObjectInfo we're looking for
  \param[out] devInfo the PaDeviceInfo of the object found
  \return true if found
  */
bool AudioDevices::FindPortAudioDevice(ObjectInfo &objInfo, PaDeviceInfo *dInfo)
{
    int cptDuplicateNames=0;

    PaDeviceIndex foundSameName=-1;
    PaDeviceIndex foundSameNameId=-1;
    PaDeviceIndex foundSameNamePins=-1;
    PaDeviceIndex foundSameNamePinsId=-1;

    PaHostApiTypeId apiType = (PaHostApiTypeId)objInfo.api;
    PaHostApiIndex apiIndex = Pa_HostApiTypeIdToHostApiIndex( apiType );
    const PaHostApiInfo *apiInfo = Pa_GetHostApiInfo( apiIndex );

    for (int i=0; i<apiInfo->deviceCount; i++) {
        PaDeviceIndex devIndex = Pa_HostApiDeviceIndexToDeviceIndex( apiIndex, i);
        const PaDeviceInfo *info = Pa_GetDeviceInfo( devIndex );

        QString devName(info->name);
        //remove " x64" from device name so we can share files with 32bit version
        devName.remove(QRegExp("( )?x64"));

        if(devName == objInfo.name) {

            if(info->maxInputChannels == objInfo.inputs
            && info->maxOutputChannels == objInfo.outputs) {
                if(objInfo.duplicateNamesCounter == cptDuplicateNames) {
                    foundSameNamePinsId = devIndex;
                } else {
                    foundSameNamePins = devIndex;
                }
            } else {
                if(objInfo.duplicateNamesCounter == cptDuplicateNames) {
                    foundSameNameId = devIndex;
                } else {
                    foundSameName = devIndex;
                }
            }

            cptDuplicateNames++;
        }
    }


    PaDeviceIndex deviceNumber=-1;

    if(foundSameNamePinsId!=-1)
        deviceNumber = foundSameNamePinsId;
    else if(foundSameNameId!=-1)
        deviceNumber = foundSameNameId;
    else if(foundSameNamePins!=-1)
        deviceNumber = foundSameNamePins;
    else if(foundSameName!=-1)
        deviceNumber = foundSameName;
    else {
        LOG("device not found"<<objInfo.apiName<<objInfo.name);
        return false;
    }

    if(dInfo) {
        const PaDeviceInfo *i = Pa_GetDeviceInfo(deviceNumber);
        if(!i) {
            LOG("error in GetDeviceInfo");
            return false;
        }
        *dInfo = *i;
    }
    objInfo.id = deviceNumber;
    return true;
}

void AudioDevices::ConfigDevice(const QModelIndex &index)
{
    if(!index.data(UserRoles::objInfo).isValid())
        return;

    ObjectInfo info = index.data(UserRoles::objInfo).value<ObjectInfo>();
    PaDeviceIndex configDevId = (PaDeviceIndex)info.id;
    PaHostApiTypeId configApiIndex = (PaHostApiTypeId)info.api;

    mutexDevices.lock();
    foreach(Connectables::AudioDevice *ad, listAudioDevices) {
        ad->SetSleep(true);
    }
    mutexDevices.unlock();

    switch(configApiIndex) {
        case paASIO: {
            PaError err;
#if WIN32
            err = PaAsio_ShowControlPanel( configDevId, (void*)myHost->mainWindow );
#endif
#ifdef __APPLE__
            // TODO
            //err = PaAsio_ShowControlPanel( configDevId, (void*)0 );
#endif

            if( err != paNoError ) {
                QMessageBox msg(QMessageBox::Warning,
                                tr("Error"),
                                Pa_GetErrorText( err ),
                                QMessageBox::Ok);
                msg.exec();
            }
            break;
        }
#ifdef WIN32
        case paMME: {
            MmeConfigDialog dlg( myHost );
            dlg.exec();
            break;
        }

        case paWASAPI: {
            WasapiConfigDialog dlg( myHost );
            dlg.exec();
            break;
        }
#endif
        default: {
            QMessageBox msg(QMessageBox::Information,
                tr("No config"),
                tr("No config dialog for this device"),
                QMessageBox::Ok);
            msg.exec();
            break;
        }
    }

    OpenDevices();
}
