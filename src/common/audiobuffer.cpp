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

#include "audiobuffer.h"
#include "audiobufferd.h"

/*!
  \class AudioBuffer
  \brief a single precision resizeable audio buffer
  */

float const AudioBuffer::blankBuffer[] = {.0f};

/*!
  \param externalAllocation true if the buffer pointer is handle by something else : don't create or delete it
  \todo check if the externalAllocation is still in use
  */
AudioBuffer::AudioBuffer(bool externalAllocation) :
    stackSize(0),
    pBuffer(0),
    nSize(0),
    nAllocatedSize(0),
    _maxVal(.0f),
    currentVu(.0f),
    bExternalAllocation(externalAllocation)
{
}

/*!
  Delete the buffer if it's not an external
  */
AudioBuffer::~AudioBuffer()
{
    if(pBuffer && !bExternalAllocation)
        delete[] pBuffer;
}

/*!
  Set the size of the buffer
  Keep the memory allocated if smaller, reallocate only if the new size is bigger
  Don't allocate anything if it's externally allocated
  \param size the new size
  */
bool AudioBuffer::SetSize(unsigned long size)
{
    if(size==nSize)
        return true;

    if(!bExternalAllocation) {
        if( size <= nAllocatedSize) {
            nSize = size;
            return true;
        }

        if(pBuffer)
            delete[] pBuffer;

        pBuffer = new float[size];
        Q_ASSERT(pBuffer);
        if(!pBuffer) {
            pBuffer=0;
            return false;
        }
    }

    nSize = nAllocatedSize = size;
    if(!bExternalAllocation) Clear();
    return true;
}

/*!
  Erase the buffer by copying a blank buffer
  */
void AudioBuffer::Clear()
{
     memcpy(pBuffer,blankBuffer,sizeof(float)*nSize);
}

/*!
  Add a buffer to the stack. Can resize the buffer if needed if the current stack is empty
  \param buff buffer to be added to the mix
  */
void AudioBuffer::AddToStack(AudioBuffer * buff)
{
    if(buff->GetSize()!=nSize) {
        if(stackSize==0) {
            SetSize(buff->GetSize());
        } else {
            debug("AudioBuffer::AddToStack : not the same size, abort %ld != %ld",buff->GetSize(),nSize)
            return;
        }
    }

    if(stackSize==0) {

        //1st in the stack, copying is enough
        memcpy(pBuffer,buff->GetPointer(),sizeof(float)*nSize);

    } else {

        //add the next buffers to the stack
        float *buffToAdd = buff->GetPointer();
        float *myBuff = pBuffer;

        for(unsigned long i=0;i<nSize;i++) {
            *myBuff += *buffToAdd;
            ++myBuff;
            ++buffToAdd;
        }
    }
    stackSize++;
}

/*!
  Convert a double precision buffer and add it to the stack
  \param buff double precision buffer to be added to the mix
  */
void AudioBuffer::AddToStack(AudioBufferD * buff)
{
    if(buff->GetSize()!=nSize) {
        if(stackSize==0) {
            SetSize(buff->GetSize());
        } else {
            debug("AudioBuffer::AddToStack : not the same size, abort %ld != %ld",buff->GetSize(),nSize)
            return;
        }
    }

    if(stackSize==0) {

        //1st in the stack, copying is enough
        float *dest=pBuffer;
        double *ori=buff->GetPointer();
        for(uint i=0; i<nSize; ++i) {
            *dest=(float)*ori;
            ++dest;
            ++ori;
        }

    } else {

        //add the next buffers to the stack
        double *buffToAdd = buff->GetPointer();
        float *myBuff = pBuffer;

        for(unsigned long i=0;i<nSize;i++) {
            *myBuff += (float)*buffToAdd;
            ++myBuff;
            ++buffToAdd;
        }
    }
    stackSize++;
}

/*!
  Set the pointer to the externally allocated buffer
  \param buff the pointer
  \param tmpBufferToBeFilled true if the buffer is currently blank, false if it contains the first stack of the mix
  */
void AudioBuffer::SetPointer(float * buff, bool tmpBufferToBeFilled)
{
    pBuffer=buff;

    if(tmpBufferToBeFilled)
        stackSize=0;
    else
        stackSize=1;
}

/*!
  Get the pointer of the audio buffer
  \param willBeFilled true if we get this pointer to replace the content of the buffer with a new sound
  */
float * AudioBuffer::GetPointer(bool willBeFilled)
{
    if(willBeFilled)
        stackSize=1;

    return pBuffer;
}

/*!
  Flatten the stack and return the resulting pointer.
  All the stacks are mixed together in one resulting buffer
  Update the vu-meter value
  \return pointer to the resulting buffer
  */
float *AudioBuffer::ConsumeStack()
{
    float ma = .0f;
    float mi = .0f;
    float *buf;

    switch(stackSize) {
        case 0:
            //empty stack : clear the stack
            Clear();
            break;

        case 1:
            //only 1 in stack, no mix needed
            //find max value
            buf = pBuffer;
            for(unsigned long i=0;i<nSize;i++) {
                if(*buf > ma)
                    ma = *buf;
                if(*buf < mi)
                    mi = *buf;

                ++buf;
            }
            break;

        default:
            //mixdown the stack and find max value
            buf = pBuffer;
            for(unsigned long i=0;i<nSize;i++) {
                *buf/=stackSize; //divide by the number of buffers, maybe not a good mix

                if(*buf > ma)
                    ma = *buf;
                if(*buf < mi)
                    mi = *buf;

                ++buf;
           }
    }

    if( std::max(ma,-mi) > _maxVal)
        _maxVal = std::max(ma,-mi);

    //if we're off-limits : here is a limiter
    if(_maxVal > 1.0f) {
        buf = pBuffer;
        for(unsigned long i=0;i<nSize;i++) {
            if(*buf > 1.0f)
                *buf = .8f;
            if(*buf < -1.0f)
                *buf = -.8f;
           ++buf;
        }
        _maxVal = 1.0f;
    }
    return pBuffer;
}

/*!
  Get the vu-meter value and reset the peak
  */
float AudioBuffer::GetVu()
{
    currentVu=_maxVal;
    _maxVal=.0f;
    return currentVu;
}
