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

#include "buffer.h"
#include "mainhost.h"

using namespace Connectables;

Buffer::Buffer(MainHost *host, int index, const ObjectInfo &info) :
    Object(host,index,ObjectInfo(NodeType::object, ObjType::Buffer, tr("Delay"))),
    delayChanged(false),
    addedSize(0),
    offset(0),
    adjustDelay(0)
{
    initialDelay = info.initDelay;
    buffer.SetSize(myHost->GetBufferSize()*2 + initialDelay);
    listAudioPinIn->SetNbPins(1);
    listAudioPinOut->SetNbPins(1);
    listParameterPinIn->listPins.insert(0, new ParameterPinIn(this,0,(float)initialDelay/50000,"Delay",true));
}

Buffer::~Buffer()
{
    if(adjustDelay)
        delete[] adjustDelay;
}

void Buffer::SetDelay(long d)
{
    addedSize+=(d-initialDelay);
    initialDelay=d;
    delayChanged=true;
    static_cast<ParameterPin*>(listParameterPinIn->GetPin(0))->ChangeValue((float)d/50000, true);
}

void Buffer::Render()
{
    if(delayChanged) {
        if(addedSize<0)
            addedSize=0;

        delayChanged=false;
        buffer.SetSize(myHost->GetBufferSize()*2 + initialDelay);
    }

    //get buffer from input
    AudioBuffer *pinInBuf = listAudioPinIn->GetBuffer(0);
    if(pinInBuf->GetDoublePrecision())
        buffer.Put( (double*)pinInBuf->ConsumeStack(), pinInBuf->GetSize() );
    else {
        float *buf = (float*)pinInBuf->ConsumeStack();
        buffer.Put( buf, pinInBuf->GetSize() );
        if(addedSize>0) {
            if(!adjustDelay)
                adjustDelay = new float[5000];

            memcpy( adjustDelay+offset, buf, pinInBuf->GetSize()*sizeof(float) );
            offset+=pinInBuf->GetSize();

            if(offset>=addedSize) {
                float *start=adjustDelay;
                long size=addedSize;
                CutBufferAtZeroCrossing(start,size);
                buffer.SetWritePosToLastZeroCrossing();
                buffer.Put(start, size);
                offset=0;
                addedSize=0;
                delete[] adjustDelay;
                adjustDelay=0;
            } else if(offset>=1500) {
                float *start=adjustDelay;
                long size=offset;
                CutBufferAtZeroCrossing(start,size);
                buffer.SetWritePosToLastZeroCrossing();
                buffer.Put(start,size);
                addedSize-=size;
                offset=0;
            }
        }
    }
    pinInBuf->ResetStackCounter();

    AudioBuffer *pinOutBuf = listAudioPinOut->GetBuffer(0);
    if(buffer.filledSize>=initialDelay+pinOutBuf->GetSize() || (addedSize>0 && buffer.filledSize>=pinOutBuf->GetSize()) ) {
        //set buffer to output
        if(pinOutBuf->GetDoublePrecision())
            buffer.Get( (double*)pinOutBuf->GetPointerWillBeFilled(), pinOutBuf->GetSize() );
        else
            buffer.Get( (float*)pinOutBuf->GetPointerWillBeFilled(), pinOutBuf->GetSize() );

        pinOutBuf->ConsumeStack();
        static_cast<AudioPin*>(listAudioPinOut->listPins.value(0))->SendAudioBuffer();
    }
}

void Buffer::OnParameterChanged(ConnectionInfo pinInfo, float value)
{
    Object::OnParameterChanged(pinInfo,value);
    long d = listParameterPinIn->listPins.value(0)->GetValue()*50000;
    if(abs(d-initialDelay)>1) {
        addedSize+=(d-initialDelay);
        initialDelay=d;
        delayChanged=true;
    }
}

bool Buffer::CutBufferAtZeroCrossing(float *buffer, long size)
{
    float *start=buffer;
    float *end=buffer+size-1;

    while(*start>0) {
        ++start;
        if(start==end) {
            LOG("zero not found");
            return false;
        }
    }

    while(*start<0) {
        ++start;
        if(start==end) {
            LOG("zero not found");
            return false;
        }
    }

    while(*end<0) {
        --end;
        if(start==end) {
            LOG("zero not found");
            return false;
        }
    }
    while(*end>0) {
        --end;
        if(start==end) {
            LOG("zero not found");
            return false;
        }
    }

    buffer=start;
    size=end-start+1;
    return true;
}
