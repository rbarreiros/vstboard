#include "audiobufferd.h"
#include "audiobuffer.h"

/*!
  \class AudioBufferD
  \brief a double precision resizeable audio buffer
  */

double const AudioBufferD::blankBuffer[] = {.0f};

/*!
  \param externalAllocation true if the buffer pointer is handle by something else : don't create or delete it
  \todo check if the externalAllocation is still in use
  */
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

/*!
  Delete the buffer if it's not an external
  */
AudioBufferD::~AudioBufferD(void)
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
        ZeroMemory( pBuffer, sizeof(double)*size );
    }

    nSize = nAllocatedSize = size;
    if(!bExternalAllocation) Clear();
    return true;
}

/*!
  Erase the buffer by copying a blank buffer
  */
void AudioBufferD::Clear()
{
    memcpy(pBuffer,blankBuffer,sizeof(double)*nSize);
}

/*!
  Add a buffer to the stack. Can resize the buffer if needed if the current stack is empty
  \param buff buffer to be added to the mix
  */
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

/*!
  Convert a single precision buffer and add it to the stack
  \param buff single precision buffer to be added to the mix
  */
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

/*!
  Set the pointer to the externally allocated buffer
  \param buff the pointer
  \param tmpBufferToBeFilled true if the buffer is currently blank, false if it contains the first stack of the mix
  */
void AudioBufferD::SetPointer(double * buff, bool tmpBufferToBeFilled)
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
double * AudioBufferD::GetPointer(bool willBeFilled)
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

/*!
  Get the vu-meter value and reset the peak
  */
float AudioBufferD::GetVu()
{
    currentVu=_maxVal;
    _maxVal=.0;
    return (float)currentVu;
}

