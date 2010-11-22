/******************************************************************************
#    Copyright 2010 Raphaël François
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
******************************************************************************/

#include "audiodevices.h"
#include "globals.h"
#include "connectables/objectinfo.h"
#include "connectables/audiodevice.h"
#include "mainhost.h"

AudioDevices * AudioDevices::theAudioDevices=0;
QHash<int,QSharedPointer<Connectables::AudioDevice> >AudioDevices::listAudioDevices;

AudioDevices *AudioDevices::Create(QObject *parent)
{
    if(!theAudioDevices)
        theAudioDevices = new AudioDevices(parent);

    return theAudioDevices;
}

AudioDevices::AudioDevices(QObject *parent) :
        QObject(parent),
        model(0),
        countActiveDevices(0)
{
    GetModel();

    fakeRenderTimer.start(FAKE_RENDER_TIMER_MS);

}

AudioDevices::~AudioDevices()
{
    foreach(QSharedPointer<Connectables::AudioDevice>ad, listAudioDevices) {
        ad->SetSleep(true);
    }

    if(model) {
        debug("AudioDevices::~AudioDevices pa_terminate")
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

ListAudioInterfacesModel * AudioDevices::GetModel()
{
    foreach(QSharedPointer<Connectables::AudioDevice>ad, listAudioDevices) {
        ad->SetSleep(true);
    }

    if(model) {
        debug("AudioDevices::GetModel pa_terminate")
        PaError err=Pa_Terminate();
        if(err!=paNoError) {
            debug("AudioDevices::GetModel Pa_Terminate %s",Pa_GetErrorText( err ))
        }
        model->invisibleRootItem()->removeRows(0, model->invisibleRootItem()->rowCount());
    }


    PaError paRet =Pa_Initialize();
    if(paRet!=paNoError) {
        debug("AudioDevices::GetModel Pa_Initialize %s",Pa_GetErrorText(paRet))
    }
    BuildModel();

//    Connectables::AudioDevice::listDevMutex.lock();
    foreach(QSharedPointer<Connectables::AudioDevice>ad, listAudioDevices) {
        ad->SetSleep(false);
    }
//    Connectables::AudioDevice::listDevMutex.unlock();

    return model;
}

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
       // api->setColumnCount(1);
        parentItem->appendRow(api);
    }

    //devices
    QString lastName;
    int cptDuplicateNames=0;

    for (int i = 0; i < Pa_GetDeviceCount(); ++i) {
        QList<QStandardItem *>  items;
        const PaDeviceInfo *devInfo = Pa_GetDeviceInfo(i);

        QString devName(devInfo->name);
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
                    //if there is no audio device we have to run it a "fake engine"
                    if(countActiveDevices==1) {
//                        disconnect(&fakeRenderTimer,SIGNAL(timeout()),
//                                MainHost::Get(), SLOT(Render()));
                        debug("AudioDevices::OnToggleDeviceInUse fakeRender off")
                        fakeRenderTimer.stop();
                    }
                    if(countActiveDevices==0) {
                        debug("AudioDevices::OnToggleDeviceInUse fakeRender on")
                        fakeRenderTimer.start(FAKE_RENDER_TIMER_MS);
//                        connect(&fakeRenderTimer,SIGNAL(timeout()),
//                                MainHost::Get(), SLOT(Render()));
                    }
                    return;
                }
            }
        }
    }
}


