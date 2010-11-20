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

#include "audiobuffergroup.h"

AudioBufferGroup::AudioBufferGroup(bool externalAllocation) :
    bExternalAllocation(externalAllocation),
    pBuffers(0),
    nCptBuffers(0),
    nSize(0),
    nAllocatedCptBuffers(0)
{
}

AudioBufferGroup::~AudioBufferGroup(void)
{
    if(pBuffers) {
        for(int i=0;i<nAllocatedCptBuffers;i++) {
            delete pBuffers[i];
        }
        delete[] pBuffers;
    }
}

bool AudioBufferGroup::SetSize(long size)
{
    for(int i=0;i<nCptBuffers;i++) {
        if(!pBuffers[i]->SetSize(size))
            return false;
    }

    nSize=size;
    return true;
}

bool AudioBufferGroup::SetNbBuffers(int nbBuff)
{
    Q_ASSERT(nbBuff>=0);
    if(nbBuff<0)
        return false;

    //if more than allocated, reallocate
    if(nbBuff>nAllocatedCptBuffers)
        return AllocBuffers(nbBuff,nSize);

    nCptBuffers=nbBuff;
    return true;
}

AudioBuffer * AudioBufferGroup::AddBuffer()
{
    if(!SetNbBuffers(nCptBuffers+1))
        return NULL;

    ClearBuffer(nCptBuffers-1);
    return pBuffers[nCptBuffers-1];
}

bool AudioBufferGroup::RemoveBuffer(int nb)
{
    return SetNbBuffers(nCptBuffers-nb);
}

bool AudioBufferGroup::AllocBuffers(int nbBuffers, long bufSize)
{
    //more buffers than allocated, allocate new ones
    if( nbBuffers > nAllocatedCptBuffers ) {

        AudioBuffer ** tmpBuf = new AudioBuffer*[nbBuffers];
        if(bExternalAllocation) {

        }

        //keep existing buffers
        for(int i=0;i<nAllocatedCptBuffers;i++) {
            tmpBuf[i]=pBuffers[i];
        }

        //add new ones
        for(int i=nAllocatedCptBuffers; i<nbBuffers; i++) {
            tmpBuf[i] = new AudioBuffer();

            Q_ASSERT(tmpBuf[i]);
            if(!tmpBuf[i])
                return false;

            nAllocatedCptBuffers++;
        }

        //delete old array
        if(pBuffers)
            delete pBuffers;

        //get new one
        pBuffers=tmpBuf;
    }

    nCptBuffers=nbBuffers;
    SetSize(bufSize);
    return true;
}


bool AudioBufferGroup::ClearBuffer(int cpt)
{
    Q_ASSERT(cpt<nCptBuffers);
    if(cpt>=nCptBuffers)
        return false;

    pBuffers[cpt]->Clear();
    return true;
}

void AudioBufferGroup::ClearAllBuffer()
{
    for(int j=0;j<nCptBuffers;j++) {
        pBuffers[j]->Clear();
    }
}

AudioBuffer * AudioBufferGroup::GetBuffer (int cpt) const
{
    if(cpt>=nCptBuffers)
        return false;
    return pBuffers[cpt];
}

void AudioBufferGroup::GetAllBuffers(float ** buf)
{
    for(int j=0;j<nCptBuffers;j++) {
        buf[j]=pBuffers[j]->GetPointer();
    }
}

void AudioBufferGroup::SetAllBuffers(float ** buff)
{
    for(int j=0;j<nCptBuffers;j++) {
        pBuffers[j]->SetPointer(buff[j]);
    }
}

