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

#include "vstaudiodeviceout.h"
#include "vstaudiodevice.h"
#include "globals.h"
#include "audiobuffer.h"
#include "mainhost.h"
#include "testvst.h"

using namespace Connectables;

VstAudioDeviceOut::VstAudioDeviceOut(int index, const ObjectInfo &info) :
    Object(index, info)
{
}

VstAudioDeviceOut::~VstAudioDeviceOut()
{
    Close();
}

bool VstAudioDeviceOut::Close()
{
    TestVst::theVstPlugin->setDeviceOut(0);
    if(!Object::Close())
        return false;
    return true;
}

void VstAudioDeviceOut::Render()
{
    foreach(AudioPinIn* pin,listAudioPinIn) {
        pin->buffer->ConsumeStack();
    }
}

void VstAudioDeviceOut::SetBufferSize(long size)
{
    foreach(AudioPinIn *pin, listAudioPinIn) {
        pin->buffer->SetSize(size);
    }
}

bool VstAudioDeviceOut::Open()
{
    if(!TestVst::theVstPlugin->setDeviceOut(this))
        return false;

    AudioPinIn *pin=0;
    for(int i=0; i<2; i++) {
        pin = new AudioPinIn(this,i,true);
        listAudioPinIn << pin;
        pin->buffer->SetSize(512);
        pin->setObjectName(QString("Output %1").arg(i));
    }
    Object::Open();
    return true;
}

void VstAudioDeviceOut::SetBuffers(float **buf)
{
    int cpt=0;
    foreach(AudioPinIn* pin,listAudioPinIn) {
        pin->buffer->SetPointer(buf[cpt],true);
        cpt++;
    }
}

