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
//#include "../audiodevices.h"
#include "../testvst.h"

using namespace Connectables;

AudioDeviceIn::AudioDeviceIn(int index, const ObjectInfo &info) :
    Object(index, info),
    bufferReady(false)
//    parentDevice(0)
{
//    listParameterPinOut.insert(0, new ParameterPinOut(this,0,0,true,"cpu%"));
}

AudioDeviceIn::~AudioDeviceIn()
{
    Close();
}

bool AudioDeviceIn::Close()
{
    TestVst::theVstPlugin->setDeviceIn(0);

    if(!Object::Close())
        return false;

//    if(parentDevice) {
//        parentDevice->SetObjectInput(0);
//        parentDevice.clear();
//    }
    return true;
}

void AudioDeviceIn::Render()
{
    foreach(AudioPinOut* pin,listAudioPinOut) {
        pin->buffer->ConsumeStack();
        pin->SendAudioBuffer();
    }

//    if(parentDevice)
//        listParameterPinOut.value(0)->ChangeValue(parentDevice->GetCpuUsage());
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
    if(!TestVst::theVstPlugin->setDeviceIn(this))
        return false;

    AudioPinOut *pin=0;
    for(int i=0; i<2; i++) {
        pin = new AudioPinOut(this,i,true);
        listAudioPinOut << pin;
        pin->buffer->SetSize(512);
        pin->setObjectName(QString("Input %1").arg(i));
    }

    //can we find this device on this computer ?
//    if(!AudioDevice::FindDeviceByName(objInfo)) {
//    errorMessage = tr("Device not found");
//        return true;
//    }

//    closed=false;

    //create the audiodevice if needed
//    if(!parentDevice) {
//        QMutexLocker l(&AudioDevice::listDevMutex);

//        if(!AudioDevices::listAudioDevices.contains(objInfo.id)) {
//            AudioDevice *dev = new AudioDevice(objInfo);
//            parentDevice = QSharedPointer<AudioDevice>(dev);
//            if(!parentDevice->Open()) {
//                parentDevice.clear();
//                errorMessage=tr("Error while opening the interface");
//                return true;
//            }
//            AudioDevices::listAudioDevices.insert(objInfo.id, parentDevice);
//        } else {
//            parentDevice = AudioDevices::listAudioDevices.value(objInfo.id);
//        }
//    }

//    if(!parentDevice) {
//        errorMessage=tr("Error : device was deleted");
//        return true;
//    }

//    //if no input channels
//    if(parentDevice->devInfo.maxInputChannels==0) {
//        parentDevice.clear();
//        //should be deleted : return false
//        return false;
//    }

//    for(int i=0;i<parentDevice->devInfo.maxInputChannels;i++) {
//        AudioPinOut *pin=0;
//        if(listAudioPinOut.size()>i) {
//            pin = listAudioPinOut.at(i);
//        } else {
//            pin = new AudioPinOut(this,i);
//            listAudioPinOut << pin;
//        }
//        pin->buffer->SetSize(MainHost::Get()->GetBufferSize());
//        pin->setObjectName(QString("Input %1").arg(i));

//    }

//    //device already has a child
//    if(!parentDevice->SetObjectInput(this)) {
//        parentDevice.clear();
//        return false;
//    }

    Object::Open();
    return true;
}

void AudioDeviceIn::SetBuffers(float **buf)
{
    int cpt=0;
    foreach(AudioPinOut* pin,listAudioPinOut) {
        pin->buffer->ResetStackCounter();
        pin->buffer->SetPointer(buf[cpt]);
        cpt++;
    }
}
