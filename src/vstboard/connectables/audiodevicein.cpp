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

#include "audiodevicein.h"
#include "audiodevice.h"
#include "../globals.h"
#include "../audiobuffer.h"
#include "../mainhost.h"
#include "../audiodevices.h"

using namespace Connectables;

AudioDeviceIn::AudioDeviceIn(MainHost *myHost,int index, const ObjectInfo &info) :
    Object(myHost,index, info),
    bufferReady(false),
    parentDevice(0)
{
    listParameterPinOut.insert(0, new ParameterPinOut(this,0,0,true,"cpu%"));
}

AudioDeviceIn::~AudioDeviceIn()
{
    Close();
}

bool AudioDeviceIn::Close()
{
    if(!Object::Close())
        return false;

    if(parentDevice) {
        parentDevice->SetObjectInput(0);
        parentDevice.clear();
    }
    return true;
}

void AudioDeviceIn::Render()
{
    foreach(AudioPinOut* pin,listAudioPinOut) {
        pin->buffer->ConsumeStack();
        pin->SendAudioBuffer();
    }

    if(parentDevice)
        listParameterPinOut.value(0)->ChangeValue(parentDevice->GetCpuUsage());
}

void AudioDeviceIn::SetBufferSize(long size)
{
    foreach(AudioPinIn *pin, listAudioPinIn) {
        pin->buffer->SetSize(size);
    }
    foreach(AudioPinOut *pin, listAudioPinOut) {
        pin->buffer->SetSize(size);
    }
}

bool AudioDeviceIn::Open()
{
    //can we find this device on this computer ?
    if(!AudioDevice::FindDeviceByName(objInfo)) {
    errorMessage = tr("Device not found");
        return true;
    }

    closed=false;

    //create the audiodevice if needed
    if(!parentDevice) {
        QMutexLocker l(&AudioDevice::listDevMutex);

        if(!myHost->audioDevices->listAudioDevices.contains(objInfo.id)) {
            AudioDevice *dev = new AudioDevice(myHost,objInfo);
            parentDevice = QSharedPointer<AudioDevice>(dev);
            if(!parentDevice->Open()) {
                parentDevice.clear();
                errorMessage=tr("Error while opening the interface");
                return true;
            }
            myHost->audioDevices->listAudioDevices.insert(objInfo.id, parentDevice);
        } else {
            parentDevice = myHost->audioDevices->listAudioDevices.value(objInfo.id);
        }
    }

    if(!parentDevice) {
        errorMessage=tr("Error : device was deleted");
        return true;
    }

    //if no input channels
    if(parentDevice->devInfo.maxInputChannels==0) {
        parentDevice.clear();
        //should be deleted : return false
        return false;
    }

    for(int i=0;i<parentDevice->devInfo.maxInputChannels;i++) {
        AudioPinOut *pin=0;
        if(listAudioPinOut.size()>i) {
            pin = listAudioPinOut.at(i);
        } else {
            pin = new AudioPinOut(this,i);
            listAudioPinOut << pin;
        }
        pin->buffer->SetSize(myHost->GetBufferSize());
        pin->setObjectName(QString("Input %1").arg(i));

    }

    //device already has a child
    if(!parentDevice->SetObjectInput(this)) {
        parentDevice.clear();
        return false;
    }

    Object::Open();
    return true;
}
