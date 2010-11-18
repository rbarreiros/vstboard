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
//       debug("CircularBuffer::Put not enough free space")
       return false;
//        filledStart = bufStart;
//        filledEnd = bufStart;
//        filledSize=0;
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
       // debug("CircularBuffer::Get not enough data")
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
