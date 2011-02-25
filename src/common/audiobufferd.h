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

        void SetPointer(double * buff, bool tmpBufferToBeFilled=false);
        double *GetPointer(bool willBeFilled=false);
        double *ConsumeStack();

        float GetVu();

        /// \return the current buffer size
        inline unsigned long GetSize() {return nSize;}

        /*!
         Get the last vu-meter value. Don't reset the peak value (used by bridges)
         \return the vu-meter value
        */
        float GetCurrentVu() {return currentVu;}

        /*!
          Clear the stack. Empty the buffer
          */
        inline void ResetStackCounter() {stackSize=0;}

        /*!
          \return true if the buffer is empty, no sound
          */
        inline bool IsEmpty() {return (stackSize==0);}

        /// \return true if we're not the owner of the buffer
        inline bool IsExternallyAllocated() {return bExternalAllocation;}

        /// a blank buffer
        static double const blankBuffer[BLANK_BUFFER_SIZE];

protected:
        /// the stack size
        unsigned int stackSize;

        /// pointer to the audio buffer
        double * pBuffer;

        /// buffer size
        unsigned long nSize;

        /// allocated buffer size, can be bigger than the useable buffer size
        unsigned long nAllocatedSize;

        /// vu-meter peak
        double _maxVal;

        /// vu-meter value
        double currentVu;

        /// true if we're not the owned of the buffer
        bool bExternalAllocation;
};


#endif // AUDIOBUFFERD_H
