#include "audiopin.h"

#include "pin.h"
#include "object.h"
#include "../globals.h"


using namespace Connectables;

/*!
  \class Connectables::AudioPin
  \brief base class for audio pins
  */

/*!
  Constructor, used by PinsList with the help of Object::CreatePin
  \param parent pointer to the parent Object
  \param direction the PinDirection
  \param number pin number in the list
  \param bufferSize size of the buffer
  \param doublePrecision true if the buffer should be a double precision buffer
  \param externalAllocation true if the audio buffer is not owned by the pin and shouldn't be deleted
  */
AudioPin::AudioPin(Object *parent, PinDirection::Enum direction, int number, unsigned long bufferSize, bool doublePrecision, bool externalAllocation) :
    Pin(parent,PinType::Audio,direction,number),
    doublePrecision(doublePrecision),
    buffer(0)
{
    buffer = new AudioBuffer(doublePrecision,externalAllocation);
    SetBufferSize(bufferSize);
    visible=true;

    if(connectInfo.direction==PinDirection::Input)
        setObjectName(QString("AudioIn%1").arg(number));
    else
        setObjectName(QString("AudioOut%1").arg(number));

    if(doublePrecision) {
        displayedText=objectName()+"=D=";
    } else {
        displayedText=objectName()+"=S=";
    }
}

/*!
  Destructor
  */
AudioPin::~AudioPin()
{
    if(buffer)
        delete buffer;
}


void AudioPin::NewRenderLoop()
{
    buffer->ResetStackCounter();
}

/*!
  set the buffer precision
  \param dblp true for double, false for float
  \return true on success
  */
bool AudioPin::SetDoublePrecision(bool dblp)
{
    if(dblp==doublePrecision)
        return true;

    if(doublePrecision) {
        displayedText=objectName()+"=D=";
    } else {
        displayedText=objectName()+"=S=";
    }

    doublePrecision=dblp;
    buffer->SetDoublePrecision(dblp);

//    if(doublePrecision) {
//        if(!bufferD) {
//            debug2(<<"AudioPin::SetDoublePrecision : no double precision buffer : can't convert to single precision")
//            return false;
//        }

//        if(!buffer)
//            buffer = new AudioBuffer(bufferD->IsExternallyAllocated());

//        buffer->SetSize(bufferD->GetSize());
//        buffer->ResetStackCounter();
//        buffer->AddToStack(bufferD);
//        delete bufferD;
//        bufferD = 0;
//    } else {
//        if(!buffer) {
//            debug2(<<"AudioPin::SetDoublePrecision : no single precision buffer : can't convert to double precision")
//            return false;
//        }

//        if(!bufferD)
//            bufferD = new AudioBufferD(buffer->IsExternallyAllocated());

//        bufferD->SetSize(buffer->GetSize());
//        bufferD->ResetStackCounter();
//        bufferD->AddToStack(buffer);
//        delete buffer;
//        buffer = 0;
//    }


    return true;
}

/*!
  Resize the current buffer
  \param size the new size
  */
void AudioPin::SetBufferSize(unsigned long size)
{
    buffer->SetSize(size);
}

float AudioPin::GetValue()
{
    float newVu=.0f;
    newVu=buffer->GetVu();

    if(newVu != value) {
        valueChanged=true;
    }
    return newVu;
}

void AudioPin::ReceiveMsg(const PinMessage::Enum msgType,void *data)
{
    if(msgType==PinMessage::AudioBuffer) {
        AudioBuffer *buf = static_cast<AudioBuffer*>(data);
        buffer->AddToStack(buf);
    }
}

/*!
  Send the current buffer to all connected pins
  */
void AudioPin::SendAudioBuffer()
{
    SendMsg(PinMessage::AudioBuffer,(void*)buffer);
}
