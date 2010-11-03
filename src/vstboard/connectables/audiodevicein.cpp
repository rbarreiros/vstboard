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
#include "../mainhost.h"

using namespace Connectables;

AudioDeviceIn::AudioDeviceIn(int index, const ObjectInfo &info) :
    Object(index, info),
    bufferReady(false),
    parentDevice(0)
{

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
    }
    return true;
}

void AudioDeviceIn::Render()
{
    foreach(AudioPinOut* pin,listAudioPinOut) {
        pin->SendAudioBuffer();
    }
}

bool AudioDeviceIn::Open()
{
    closed=false;

    //create the audiodevice if needed
    if(!AudioDevice::listAudioDevices.contains(objInfo.id)) {
        parentDevice = new AudioDevice(objInfo, MainHost::Get());
        if(!parentDevice->Open()) {
            delete parentDevice;
            parentDevice=0;
            return false;
        }
    } else {
        parentDevice = AudioDevice::listAudioDevices.value(objInfo.id);
    }

    if(!parentDevice)
        return false;

    if(!parentDevice->devInfo)
        return false;

    //if no input channels
    if(parentDevice->devInfo->maxInputChannels==0)
        return false;

    for(int i=0;i<parentDevice->devInfo->maxInputChannels;i++) {
        AudioPinOut *pin = new AudioPinOut(this,i,true);
        pin->setObjectName(QString("Input %1").arg(i));
        listAudioPinOut << pin;
    }

    //device already has a child
    if(!parentDevice->SetObjectInput(this)) {
        parentDevice = 0;
        return false;
    }

    Object::Open();
    return true;
}

