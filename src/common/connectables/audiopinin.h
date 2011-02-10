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

#ifndef CONNECTABLEAUDIOPININ_H
#define CONNECTABLEAUDIOPININ_H

#include "pin.h"

class AudioBuffer;
class AudioBufferD;

namespace Connectables {

    class AudioPinIn : public Pin
    {
    public:
        AudioPinIn(Object *parent, int number, bool externalAllocation = false, bool bridge=false);
        ~AudioPinIn();

        void SetBuffer(AudioBuffer *buffer);
        void SetBuffer(AudioBufferD *buffer);
        void ReceiveMsg(const PinMessage::Enum msgType,void *data=0);
        float GetValue();
        void NewRenderLoop();
        AudioBuffer *buffer;
        AudioBufferD *bufferD;

        bool doublePrecision;
    };
}
#endif // CONNECTABLEAUDIOPININ_H
