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

using namespace Connectables;

/*!
  \class Connectables::AudioPinOut
  \brief audio pin output
  */

/*!
  Constructor, used by PinsList with the help of Object::CreatePin
  \param parent pointer to the parent Object
  \param number pin number in the list
  \param bufferSize size of the buffer
  \param doublePrecision true if the buffer should be a double precision buffer
  \param externalAllocation true if the audio buffer is not owned by the pin and shouldn't be deleted
  */
AudioPinOut::AudioPinOut(Object *parent, int number, unsigned long bufferSize, bool doublePrecision, bool externalAllocation) :
    AudioPin(parent,PinDirection::Output,number,bufferSize,doublePrecision,externalAllocation)
{
    setObjectName(QString("AudioOut%1").arg(number));
}

/*!
  Send the current buffer to all connected pins
  */
void AudioPinOut::SendAudioBuffer()
{
    if(doublePrecision)
        SendMsg(PinMessage::AudioBufferD,(void*)bufferD);
    else
        SendMsg(PinMessage::AudioBuffer,(void*)buffer);
}


