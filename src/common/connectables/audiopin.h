#ifndef AUDIOPIN_H
#define AUDIOPIN_H

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4100 )
#endif

#include "pin.h"
#include "../audiobuffer.h"

namespace Connectables {

    class AudioPin : public Pin
    {
        Q_OBJECT
        Q_PROPERTY(AudioBuffer * buffer READ GetBuffer WRITE SetBuffer)
        Q_PROPERTY(float value READ GetVal)
    public:
        AudioPin(Object *parent, PinDirection::Enum direction, int number, unsigned long bufferSize, bool doublePrecision=false, bool externalAllocation=false);
        virtual ~AudioPin();

        float GetVal() { return buffer->GetCurrentVu(); }

        void SetBufferSize(unsigned long size);
        bool SetDoublePrecision(bool dblp);

        /// \return pointer to the current buffer
        AudioBuffer * GetBuffer() {return buffer;}
        void SetBuffer(AudioBuffer *buf) { buffer->AddToStack(buf); }

        float GetValue();
        void NewRenderLoop();

        void ReceiveMsg(const PinMessage::Enum msgType,void *data=0);
        void SendAudioBuffer();

    protected:

        /// true if double precision buffer
        bool doublePrecision;

        AudioBuffer *buffer;
    };
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // AUDIOPIN_H
