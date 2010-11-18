/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "audiodevicein.h"
#include "audiodevice.h"
#include "../globals.h"
#include "../audiobuffer.h"
#include "../mainhost.h"

using namespace Connectables;

AudioDeviceIn::AudioDeviceIn(int index, const ObjectInfo &info) :
    Object(index, info),
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
    if(!AudioDevice::FindDeviceFromName(objInfo)) {
        debug("AudioDeviceIn::Open device not found")
        return false;
    }

    closed=false;

    //create the audiodevice if needed
    {
        QMutexLocker l(&AudioDevice::listDevMutex);

        if(!AudioDevice::listAudioDevices.contains(objInfo.id)) {
    //        parentDevice = new AudioDevice(objInfo);
            AudioDevice *dev = new AudioDevice(objInfo);
            parentDevice = QSharedPointer<AudioDevice>(dev);
            if(!parentDevice->Open()) {
    //            delete parentDevice;
    //            parentDevice=0;
                parentDevice.clear();
                return false;
            }
            AudioDevice::listAudioDevices.insert(objInfo.id, parentDevice);
        } else {
            parentDevice = AudioDevice::listAudioDevices.value(objInfo.id);
        }
    }

    if(!parentDevice)
        return false;

    //if no input channels
    if(parentDevice->devInfo.maxInputChannels==0) {
        parentDevice.clear();
        return false;
    }

    for(int i=0;i<parentDevice->devInfo.maxInputChannels;i++) {
//        AudioPinOut *pin = new AudioPinOut(this,i,true);
        AudioPinOut *pin = new AudioPinOut(this,i);
        pin->buffer->SetSize(MainHost::Get()->GetBufferSize());
        pin->setObjectName(QString("Input %1").arg(i));
        listAudioPinOut << pin;
    }

    //device already has a child
    if(!parentDevice->SetObjectInput(this)) {
//        parentDevice = 0;
        parentDevice.clear();
        return false;
    }

    Object::Open();
    return true;
}
