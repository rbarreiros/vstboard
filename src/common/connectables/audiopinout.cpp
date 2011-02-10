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

#include "audiopinout.h"
#include "pin.h"
#include "object.h"
#include "../globals.h"
#include "../audiobuffer.h"
#include "../audiobufferd.h"

using namespace Connectables;

AudioPinOut::AudioPinOut(Object *parent, int number, bool externalAllocation, bool bridge)
    :Pin(parent,PinType::Audio,PinDirection::Output,number,bridge)
{
    doublePrecision=false;
    buffer = new AudioBuffer(externalAllocation);
    bufferD = new AudioBufferD(externalAllocation);
    setObjectName(QString("Out%1").arg(number));
    visible=true;
}

AudioPinOut::~AudioPinOut()
{
    delete buffer;
    delete bufferD;
}

void AudioPinOut::SetBuffer(AudioBuffer *buffer)
{
   this->buffer = buffer;
}
void AudioPinOut::SetBuffer(AudioBufferD *buffer)
{
   this->bufferD = buffer;
}

void AudioPinOut::SendAudioBuffer()
{
    if(doublePrecision)
        SendMsg(PinMessage::AudioBufferD,(void*)bufferD);
    else
        SendMsg(PinMessage::AudioBuffer,(void*)buffer);
}


float AudioPinOut::GetValue()
{
    float newVu=.0f;
    if(doublePrecision)
        newVu = bufferD->GetVu();
    else
        newVu = buffer->GetVu();

    if(newVu != value) {
        valueChanged=true;
    }
    return newVu;
}
