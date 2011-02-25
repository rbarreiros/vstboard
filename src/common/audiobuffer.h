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

#define BLANK_BUFFER_SIZE 44100

#include "precomp.h"

class AudioBufferD;
class AudioBuffer
{
public:
        AudioBuffer(bool externalAllocation = false);
        ~AudioBuffer();
        bool SetSize(unsigned long size);
        void Clear();
        void AddToStack(AudioBuffer * buff);
        void AddToStack(AudioBufferD * buff);

        void SetPointer(float * buff, bool tmpBufferToBeFilled=false);
        float *GetPointer(bool willBeFilled=false);
        float *ConsumeStack();

        float GetVu();

        /// \return the current buffer size
        inline unsigned long GetSize() {return nSize;}

        /*!
         Get the last vu-meter value. Don't reset the peak value (used by bridges)
         \return the vu-meter value
        */
        float GetCurrentVu() {return currentVu;}

        /*!
          Clear the stack. Empty the buffer
          */
        inline void ResetStackCounter() {stackSize=0;}

        /*!
          \return true if the buffer is empty, no sound
          */
        inline bool IsEmpty() {return (stackSize==0);}

        /// \return true if we're not the owner of the buffer
        inline bool IsExternallyAllocated() {return bExternalAllocation;}

        /// a blank buffer
        static float const blankBuffer[BLANK_BUFFER_SIZE];

protected:
        /// the stack size
        unsigned int stackSize;

        /// pointer to the audio buffer
        float * pBuffer;

        /// buffer size
        unsigned long nSize;

        /// allocated buffer size, can be bigger than the useable buffer size
        unsigned long nAllocatedSize;

        /// vu-meter peak
        float _maxVal;

        /// vu-meter value
        float currentVu;

        /// true if we're not the owned of the buffer
        bool bExternalAllocation;
};


#endif // AUDIOBUFFER_H
