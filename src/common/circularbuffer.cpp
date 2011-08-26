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
        memset(buffer,0,sizeof(float)*buffSize);
        bufStart = &buffer[0];
        bufEnd = &buffer[buffSize-1];
        readPos = bufStart;
        writePos = bufStart;
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
        memset(buffer,0,sizeof(float)*buffSize);
        bufStart = &buffer[0];
        bufEnd = &buffer[buffSize-1];
        readPos = bufStart;
        writePos = bufStart;
    }
    filledSize=0;
}

void CircularBuffer::Clear()
{
    if(buffSize==0)
        return;

    readPos = bufStart;
    writePos = bufStart;
    filledSize=0;
    buffer[0]=.0f;
}

bool CircularBuffer::Put(float *buf, unsigned int size)
{
    if(buffSize==0)
        return false;

    if((buffSize-filledSize)<size) {
//       LOG("CircularBuffer::Put not enough free space");
       unsigned int overlapping = size-(buffSize-filledSize);
       readPos+=overlapping;
       filledSize-=overlapping;
       while(readPos>bufEnd)
           readPos-=buffSize;
    }

    if(writePos+size <= bufEnd) {
        memcpy(writePos,buf,size*sizeof(float));
        writePos+=size;
        if(writePos>bufEnd)
            writePos-=buffSize;
    } else {
        int size1 = bufEnd - writePos +1;
        int size2 = size - size1;
        memcpy(writePos,buf,size1*sizeof(float));
        memcpy(bufStart,buf+size1,size2*sizeof(float));
        writePos=bufStart+size2;
    }
    filledSize+=size;

//    for(unsigned int i=0; i<size; i++) {
//        if(writePos>bufEnd) {
//            writePos=bufStart;
//        }
//        *writePos=*buf;
//        buf++;
//        writePos++;
//    }
//    if(writePos>bufEnd) {
//        writePos=bufStart;
//    }
//    filledSize+=size;

    return true;
}

bool CircularBuffer::Put(double *buf, unsigned int size)
{
    if(buffSize==0)
        return false;

    if((buffSize-filledSize)<size) {
       //debug("CircularBuffer::Put not enough free space")
       unsigned int overlapping = size-(buffSize-filledSize);
       readPos+=overlapping;
       filledSize-=overlapping;
       if(readPos>bufEnd)
           readPos-=buffSize;
    }

    for(unsigned int i=0; i<size; i++) {

        if(writePos>bufEnd) {
            writePos=bufStart;
        }

        *writePos=(float)*buf;
        buf++;
        writePos++;
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

    if(readPos+size <= bufEnd) {
        memcpy(buf,readPos,size*sizeof(float));
        readPos+=size;
    } else {
        int size1 = bufEnd - readPos +1;
        int size2 = size - size1;
        memcpy(buf,readPos,size1*sizeof(float));
        memcpy(buf+size1,bufStart,size2*sizeof(float));
        readPos=bufStart+size2;
    }
    filledSize-=size;

//    for(unsigned int i=0; i<size; i++) {
//        *buf=*readPos;
//        buf++;
//        readPos++;

//        if(readPos>bufEnd) {
//            readPos=bufStart;
//        }
//    }

//    filledSize-=size;

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
        *buf=(double)*readPos;
        buf++;
        readPos++;

        if(readPos>bufEnd) {
            readPos=bufStart;
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

    readPos+=size;
    if(readPos>bufEnd) {
        readPos=bufStart+(readPos-bufEnd);
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
