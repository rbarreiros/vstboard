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

#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#define BLANK_BUFFER_SIZE 4096

#include "precomp.h"

class AudioBuffer
{
public:
        AudioBuffer(bool externalAllocation = false);
        ~AudioBuffer(void);
        bool SetSize(unsigned long size);
        void Clear();
        void AddToStack(AudioBuffer * buff);
        void SetPointer(float * buff, bool tmpBufferToBeFilled=false);
        float *GetPointer(bool willBeFilled=false);
        float *ConsumeStack();
        inline unsigned long GetSize() {return nSize;}
        float GetVu();
        float GetCurrentVu() {return currentVu;}
        inline void ResetStackCounter() {stackSize=0;}
        inline bool IsEmpty() {return (stackSize==0);}
        static float const blankBuffer[BLANK_BUFFER_SIZE];

protected:
        unsigned int stackSize;
        float * pBuffer;
        unsigned long nSize;
        unsigned long nAllocatedSize;
        float _maxVal;
        float currentVu;

        bool bExternalAllocation;
};


#endif // AUDIOBUFFER_H
