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
#include "circularbuffer.h"
//#include "precomp.h"

CircularBuffer::CircularBuffer(unsigned int buffSize) :
    buffSize(buffSize),
    buffer(0)
{
    if(buffSize!=0) {
        buffer = new float[buffSize];
        ZeroMemory(buffer,sizeof(float)*buffSize);
        bufStart = &buffer[0];
        bufEnd = &buffer[buffSize-1];
        filledStart = bufStart;
        filledEnd = bufStart;
    }

    filledSize=0;
}

CircularBuffer::~CircularBuffer()
{
    if(buffSize!=0)
        delete[] buffer;
}

void CircularBuffer::SetSize(unsigned int size)
{
    if(buffSize!=0)
        delete[] buffer;

    buffSize = size;

    if(buffSize!=0) {
        buffer = new float[buffSize];
        ZeroMemory(buffer,sizeof(float)*buffSize);
        bufStart = &buffer[0];
        bufEnd = &buffer[buffSize-1];
        filledStart = bufStart;
        filledEnd = bufStart;
    }
    filledSize=0;
}

void CircularBuffer::Clear()
{
    if(buffSize==0)
        return;

    filledStart = bufStart;
    filledEnd = bufStart;
    filledSize=0;
    buffer[0]=.0f;
}

bool CircularBuffer::Put(float *buf, unsigned int size)
{
    if(buffSize==0)
        return false;

    if((buffSize-filledSize)<size) {
       //debug("CircularBuffer::Put not enough free space")
       unsigned int overlapping = size-(buffSize-filledSize);
       filledStart+=overlapping;
       filledSize-=overlapping;
       if(filledStart>bufEnd)
           filledStart-=buffSize;
//       return false;
    }

    for(unsigned int i=0; i<size; i++) {
        filledEnd++;

        if(filledEnd>bufEnd) {
            filledEnd=bufStart;
        }

        *filledEnd=*buf;
        buf++;
    }

    filledSize+=size;
    return true;
}

bool CircularBuffer::Put(double *buf, unsigned int size)
{
    if(buffSize==0)
        return false;

    if((buffSize-filledSize)<size) {
       //debug("CircularBuffer::Put not enough free space")
       unsigned int overlapping = size-(buffSize-filledSize);
       filledStart+=overlapping;
       filledSize-=overlapping;
       if(filledStart>bufEnd)
           filledStart-=buffSize;
//       return false;
    }

    for(unsigned int i=0; i<size; i++) {
        filledEnd++;

        if(filledEnd>bufEnd) {
            filledEnd=bufStart;
        }

        *filledEnd=(float)*buf;
        buf++;
    }

    filledSize+=size;
    return true;
}

bool CircularBuffer::Get(float *buf, unsigned int size)
{
    if(buffSize==0)
        return false;

    if(filledSize<size) {
        LOG("not enough data");
        return false;
    }


    for(unsigned int i=0; i<size; i++) {
        *buf=*filledStart;
        buf++;
        filledStart++;

        if(filledStart>bufEnd) {
            filledStart=bufStart;
        }
    }

    filledSize-=size;
    return true;
}

bool CircularBuffer::Get(double *buf, unsigned int size)
{
    if(buffSize==0)
        return false;

    if(filledSize<size) {
        LOG("not enough data");
        return false;
    }


    for(unsigned int i=0; i<size; i++) {
        *buf=(double)*filledStart;
        buf++;
        filledStart++;

        if(filledStart>bufEnd) {
            filledStart=bufStart;
        }
    }

    filledSize-=size;
    return true;
}

bool CircularBuffer::Skip(unsigned int size)
{
    if(buffSize==0)
        return false;

    if(filledSize<size) {
        LOG("can't skip more than filledsize");
        return false;
    }

    filledStart+=size;
    if(filledStart>bufEnd) {
        filledStart=bufStart+(filledStart-bufEnd);
    }
    filledSize-=size;
    return true;
}

//bool CircularBuffer::Keep(unsigned int size) {
//    if(filledSize<size) {
//        debug("CircularBuffer::Get can't keep more than filledsize")
//        return false;
//    }

//    filledStart=filledEnd-size;
//    if(filledStart<bufStart) {
//        filledStart=bufEnd-(bufStart-filledStart);
//    }
//    filledSize=size;
//    return true;
//}
