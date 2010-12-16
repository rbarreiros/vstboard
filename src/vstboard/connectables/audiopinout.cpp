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

#include "audiopinout.h"
#include "pin.h"
#include "object.h"
#include "../globals.h"
#include "../audiobuffer.h"

using namespace Connectables;

AudioPinOut::AudioPinOut(Object *parent, int number, bool externalAllocation, bool bridge)
    :Pin(parent,PinType::Audio,PinDirection::Output,number,bridge)
{
    buffer = new AudioBuffer(externalAllocation);
    setObjectName(QString("Out%1").arg(number));
    visible=true;
}

AudioPinOut::~AudioPinOut()
{
    delete buffer;
}

void AudioPinOut::SetBuffer(AudioBuffer *buffer)
{
   this->buffer = buffer;
}

void AudioPinOut::SendAudioBuffer()
{
    SendMsg(PinMessage::AudioBufferToMix,(void*)buffer);
}


float AudioPinOut::GetValue()
{
    float newVu = buffer->GetVu();
    if(newVu != value) {
        valueChanged=true;
    }
    return newVu;
}
