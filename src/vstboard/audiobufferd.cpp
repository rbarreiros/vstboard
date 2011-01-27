#include "audiobufferd.h"
#include "audiobuffer.h"

double const AudioBufferD::blankBuffer[] = {.0f};

AudioBufferD::AudioBufferD(bool externalAllocation) :
    stackSize(0),
    pBuffer(0),
    nSize(0),
    nAllocatedSize(0),
    _maxVal(.0f),
    currentVu(.0f),
    bExternalAllocation(externalAllocation)

{
}

AudioBufferD::~AudioBufferD(void)
{
    if(pBuffer && !bExternalAllocation)
        delete[] pBuffer;
}

bool AudioBufferD::SetSize(unsigned long size)
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

        pBuffer = new double[size];
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

void AudioBufferD::Clear()
{
    memcpy(pBuffer,blankBuffer,sizeof(double)*nSize);
}

void AudioBufferD::AddToStack(AudioBufferD * buff)
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
        memcpy(pBuffer,buff->GetPointer(),sizeof(double)*nSize);

    } else {

        //add the next buffers to the stack
        double *buffToAdd = buff->GetPointer();
        double *myBuff = pBuffer;

        for(unsigned long i=0;i<nSize;i++) {
            *myBuff += *buffToAdd;
            ++myBuff;
            ++buffToAdd;
        }
    }

    stackSize++;
}

void AudioBufferD::AddToStack(AudioBuffer * buff)
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
        double *dest=pBuffer;
        float *ori=buff->GetPointer();
        for(uint i=0; i<nSize; ++i) {
            *dest=(double)*ori;
            ++dest;
            ++ori;
        }

    } else {

        //add the next buffers to the stack
        float *buffToAdd = buff->GetPointer();
        double *myBuff = pBuffer;

        for(unsigned long i=0;i<nSize;++i) {
            *myBuff += (double)*buffToAdd;
            ++myBuff;
            ++buffToAdd;
        }
    }
    stackSize++;
}

//if tmpBufferToBeFilled : this buff is not a sound and a blank buffer will be returned if no other sounds are added to the stack
void AudioBufferD::SetPointer(double * buff, bool tmpBufferToBeFilled)
{
    pBuffer=buff;

    if(tmpBufferToBeFilled)
        stackSize=0;
    else
        stackSize=1;
}

//if willBeFilled : the stack now contains 1 buffer
double * AudioBufferD::GetPointer(bool willBeFilled)
{
    if(willBeFilled)
        stackSize=1;

    return pBuffer;
}

double *AudioBufferD::ConsumeStack()
{
    double ma = .0;
    double mi = .0;
    double *buf;

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
    if(_maxVal > 1.0) {
        buf = pBuffer;
        for(unsigned long i=0;i<nSize;i++) {
            if(*buf > 1.0)
                *buf = .8;
            if(*buf < -1.0)
                *buf = -.8;
           ++buf;
        }
        _maxVal = 1.0;
    }
    return pBuffer;
}

float AudioBufferD::GetVu()
{
    currentVu=_maxVal;
    _maxVal=.0;
    return (float)currentVu;
}

