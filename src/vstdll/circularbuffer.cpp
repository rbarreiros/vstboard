#include "circularbuffer.h"
#include "precomp.h"

CircularBuffer::CircularBuffer() :
        buffSize(4096)
{
    buffer = new float[buffSize];
    bufStart = &buffer[0];
    bufEnd = &buffer[buffSize-1];
    filledStart = bufStart;
    filledEnd = bufStart;
    filledSize=0;
}

CircularBuffer::~CircularBuffer()
{
    delete[] buffer;
}

void CircularBuffer::Clear()
{
    filledStart = bufStart;
    filledEnd = bufStart;
    filledSize=0;
    buffer[0]=.0f;
}

bool CircularBuffer::Put(float *buf, unsigned int size)
{
    if((buffSize-filledSize)<size) {
       debug("CircularBuffer::Put not enough free space")
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

bool CircularBuffer::Get(float *buf, unsigned int size)
{
    if(filledSize<size) {
        debug("CircularBuffer::Get not enough data")
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


bool CircularBuffer::Skip(unsigned int size)
{
    if(filledSize<size) {
        debug("CircularBuffer::Get can't skip more than filledsize")
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
