#ifndef AUDIOBUFFERD_H
#define AUDIOBUFFERD_H


#define BLANK_BUFFER_SIZE 44100

#include "precomp.h"

class AudioBuffer;
class AudioBufferD
{
public:
        AudioBufferD(bool externalAllocation = false);
        ~AudioBufferD(void);
        bool SetSize(unsigned long size);
        void Clear();
        void AddToStack(AudioBufferD * buff);
        void AddToStack(AudioBuffer * buff);

        inline unsigned long GetSize() {return nSize;}
        float GetVu();
        float GetCurrentVu() {return currentVu;}
        inline void ResetStackCounter() {stackSize=0;}
        inline bool IsEmpty() {return (stackSize==0);}
        static double const blankBuffer[BLANK_BUFFER_SIZE];

        void SetPointer(double * buff, bool tmpBufferToBeFilled=false);
        double *GetPointer(bool willBeFilled=false);
        double *ConsumeStack();

protected:
        unsigned int stackSize;
        double * pBuffer;
        unsigned long nSize;
        unsigned long nAllocatedSize;
        double _maxVal;
        double currentVu;

        bool bExternalAllocation;
};


#endif // AUDIOBUFFERD_H
