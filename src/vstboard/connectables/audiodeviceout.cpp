/******************************************************************************
#    Copyright 2010 Rapha�l Fran�ois
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

#include "audiodeviceout.h"
#include "audiodevice.h"
#include "../globals.h"
#include "../audiobuffer.h"
#include "../mainhost.h"
#include "../audiodevices.h"

using namespace Connectables;

AudioDeviceOut::AudioDeviceOut(MainHost *myHost,int index, const ObjectInfo &info) :
    Object(myHost,index, info),
    parentDevice(0)
{
    listParameterPinOut->AddPin(0);
}

AudioDeviceOut::~AudioDeviceOut()
{
    Close();
}

bool AudioDeviceOut::Close()
{
    if(!Object::Close())
        return false;

    if(parentDevice) {
        parentDevice->SetObjectOutput(0);
        parentDevice.clear();
    }
    return true;
}

void AudioDeviceOut::Render()
{
    if(parentDevice)
        static_cast<ParameterPinOut*>(listParameterPinOut->listPins.value(0))->ChangeValue(parentDevice->GetCpuUsage());
}

void AudioDeviceOut::SetBufferSize(unsigned long size)
{
    foreach(Pin *pin, listAudioPinIn->listPins) {
        static_cast<AudioPinIn*>(pin)->buffer->SetSize(size);
    }
}

bool AudioDeviceOut::Open()
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

    //if no output channels
    if(parentDevice->devInfo.maxOutputChannels==0) {
        parentDevice.clear();
        //should be deleted : return false
        return false;
    }

    listAudioPinIn->ChangeNumberOfPins( parentDevice->devInfo.maxOutputChannels );

    //device already has a child
    if(!parentDevice->SetObjectOutput(this)) {
        parentDevice.clear();
        return false;
    }

    Object::Open();
    return true;
}

Pin* AudioDeviceOut::CreatePin(const ConnectionInfo &info)
{
    Pin *newPin = Object::CreatePin(info);
    if(newPin) {
        if(info.type==PinType::Audio)
            newPin->setObjectName(QString("Output %1").arg(info.pinNumber));
        return newPin;
    }

    switch(info.direction) {
        case PinDirection::Output :
            if(info.pinNumber==0)
                return new ParameterPinOut(this,0,0,true,"cpu%",false);
            break;

        default :
            debug("AudioDeviceIn::CreatePin PinDirection")
            return 0;
            break;
    }

    return 0;
}
