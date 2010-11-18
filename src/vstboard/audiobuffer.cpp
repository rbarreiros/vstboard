/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "audiobuffer.h"

float const AudioBuffer::blankBuffer[] = {.0f};

AudioBuffer::AudioBuffer(bool externalAllocation) :
    stackSize(0),
    pBuffer(0),
    nSize(0),
    nAllocatedSize(0),
    _maxVal(0),
    bExternalAllocation(externalAllocation)
{
}

AudioBuffer::~AudioBuffer(void)
{
    if(pBuffer && !bExternalAllocation)
        delete[] pBuffer;
}

bool AudioBuffer::SetSize(unsigned long size)
{
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

void AudioBuffer::Clear()
{
    memcpy(pBuffer,blankBuffer,sizeof(float)*nSize);
}

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


//if tmpBufferToBeFilled : this buff is not a sound and a blank buffer will be returned if no other sounds are added to the stack
void AudioBuffer::SetPointer(float * buff, bool tmpBufferToBeFilled)
{
    pBuffer=buff;

    if(tmpBufferToBeFilled)
        stackSize=0;
    else
        stackSize=1;
}

//if willBeFilled : the stack now contains 1 buffer
float * AudioBuffer::GetPointer(bool willBeFilled)
{
    if(willBeFilled)
        stackSize=1;

    return pBuffer;
}

float *AudioBuffer::ConsumeStack()
{
    float ma = .0f;
    float mi = .0f;
    float *buf;

    switch(stackSize) {
        case 0:
            //empty stack : return a blank buffer
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

float AudioBuffer::GetVu()
{
    float vu=_maxVal;
    _maxVal=.0f;
    return vu;
}
