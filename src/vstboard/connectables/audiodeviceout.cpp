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

#include "audiodeviceout.h"
#include "audiodevice.h"
#include "../globals.h"
#include "../audiobuffer.h"
#include "../mainhost.h"

using namespace Connectables;

AudioDeviceOut::AudioDeviceOut(int index, const ObjectInfo &info) :
    Object(index, info),
    bufferReady(false),
    parentDevice(0)
{

}

AudioDeviceOut::~AudioDeviceOut()
{
    Close();
}

//void AudioDeviceOut::init()
//{
//        qRegisterMetaTypeStreamOperators<Connectables::AudioDeviceOut>("Connectables::AudioDeviceOut");
//        qMetaTypeId<Connectables::AudioDeviceOut>();
//}

bool AudioDeviceOut::Close()
{
    if(!Object::Close())
        return false;

    if(parentDevice) {
        parentDevice->SetObjectOutput(0);

//        if(!parentDevice->devIn && !parentDevice->devOut) {
//            parentDevice->Close();
//            parentDevice->deleteLater();
//            parentDevice=0;
//        }
    }
    return true;
}

void AudioDeviceOut::Render()
{
    bufferReady = true;
}

bool AudioDeviceOut::Open()
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

    //if no output channels
    if(parentDevice->devInfo->maxOutputChannels==0)
        return false;

    for(int i=0;i<parentDevice->devInfo->maxOutputChannels;i++) {
        AudioPinIn *pin = new AudioPinIn(this,i,true);
        pin->setObjectName(QString("Output %1").arg(i));
        listAudioPinIn << pin;
//        listPins << pin;
    }

    //device already has a child
    if(!parentDevice->SetObjectOutput(this)) {
        parentDevice = 0;
        return false;
    }

    //UpdateModelNode();
    Object::Open();
    return true;
}

