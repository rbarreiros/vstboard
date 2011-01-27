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

#include "vstaudiodeviceout.h"
#include "globals.h"
#include "audiobuffer.h"
#include "audiobufferd.h"
#include "mainhost.h"
#include "../vst.h"

using namespace Connectables;

VstAudioDeviceOut::VstAudioDeviceOut(MainHost *myHost, int index, const ObjectInfo &info) :
    Object(myHost,index, info)
{
}

VstAudioDeviceOut::~VstAudioDeviceOut()
{
    Close();
}

bool VstAudioDeviceOut::Close()
{
    myHost->myVstPlugin->removeAudioOut(this);
    if(!Object::Close())
        return false;
    return true;
}

void VstAudioDeviceOut::SetBufferSize(unsigned long size)
{
    foreach(Pin *pin, listAudioPinIn->listPins) {
        if(doublePrecision) {
            static_cast<AudioPinIn*>(pin)->bufferD->SetSize(size);
        } else {
            static_cast<AudioPinIn*>(pin)->buffer->SetSize(size);
        }
    }
}

bool VstAudioDeviceOut::Open()
{
    if(!myHost->myVstPlugin->addAudioOut(this))
        return false;

    listAudioPinIn->ChangeNumberOfPins(2);
    SetBufferSize(myHost->GetBufferSize());
    Object::Open();
    return true;
}

void VstAudioDeviceOut::GetBuffers(float **buf, int &cpt, int sampleFrames)
{
    foreach(Pin *pin, listAudioPinIn->listPins) {
        AudioBuffer *abuf= static_cast<AudioPinIn*>(pin)->buffer;
        memcpy((float*)buf[cpt], (float*)abuf->ConsumeStack(), sampleFrames*sizeof(float));
        cpt++;
    }
}

void VstAudioDeviceOut::GetBuffers(double **buf, int &cpt, int sampleFrames)
{
    foreach(Pin *pin, listAudioPinIn->listPins) {
        AudioBufferD *abuf= static_cast<AudioPinIn*>(pin)->bufferD;
        memcpy((float*)buf[cpt], (float*)abuf->ConsumeStack(), sampleFrames*sizeof(double));
        cpt++;
    }
}

//Pin* VstAudioDeviceOut::CreatePin(const ConnectionInfo &info)
//{
//    AudioPinIn *newPin = new AudioPinIn(this,info.pinNumber);
//    return newPin;
//}
