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

#include "connectables/audiodevice.h"
#include "connectables/object.h"
#include "connectables/audiodevicein.h"
#include "connectables/audiodeviceout.h"
#include "globals.h"
#include "audiobuffer.h"
#include "audiobufferd.h"
#include "renderer/renderer.h"
#include "mainhosthost.h"
#include "audiodevices.h"

using namespace Connectables;

//QHash<int,QSharedPointer<AudioDevice> >AudioDevice::listAudioDevices;
QMutex AudioDevice::mutexCountInputDevicesReady;
int AudioDevice::countDevicesReady=0;
int AudioDevice::countInputDevices=0;

/*!
  \class Connectables::AudioDevice
  \brief an audio device using PortAudio. created by AudioDeviceIn or AudioDeviceOut
  */

/*!
  /param myHost the MainHost
  /param info ObjectInfo describing the device
  /param parent a parent QObject (unused ?)
  */
AudioDevice::AudioDevice(PaDeviceIndex devId,PaDeviceInfo &devInfo,MainHostHost *myHost,const ObjectInfo &info, QObject *parent) :
    QObject(parent),
    sampleRate(44100.0f),
    bufferSize(4096),
    stream(0),
    objInfo(info),
    devId(devId),
    devInfo(devInfo),
    devIn(0),
    devOut(0),
    closed(true),
    myHost(myHost),
    inputBufferReady(false)
{
    devOutClosing=false;
    setObjectName(objInfo.name);

    connect(myHost,SIGNAL(SampleRateChanged(float)),
            this,SLOT(SetSampleRate(float)));

    connect(this,SIGNAL(InUseChanged(ObjectInfo,bool)),
            myHost,SIGNAL(OnAudioDeviceToggleInUse(ObjectInfo,bool)));
}

AudioDevice::~AudioDevice()
{
    Close();
    debug("%s deleted",objectName().toAscii().constData())
}

/*!
  Close the device if not in use. Called after a timeout when the AudioDeviceIn or AudioDeviceOut are removed
  */
void AudioDevice::DeleteIfUnused()
{
    if(isClosing)
        return;

    bool del=false;
    mutexDevicesInOut.lock();
    if(!devIn && !devOut)
        del=true;
    mutexDevicesInOut.unlock();

    if(del) {
        SetSleep(true);
        myHost->audioDevices->RemoveDevice(devId);
    }

}

/*!
  Set the input Object
  The device is closed after 2 seconds if it has no input or output object
  \param obj the AudioDeviceIn, null to unlink
  \return true on success
  */
bool AudioDevice::SetObjectInput(AudioDeviceIn *obj)
{
    QMutexLocker l(&mutexDevicesInOut);

    if(devIn && devIn == obj) {
        //it's the same object
        return true;
    }

    if(devIn && obj) {
        debug("AudioDevice::SetObjectInput already used")
        return false;
    }

    mutexCountInputDevicesReady.lock();
    if(obj) {
        countInputDevices++;
    } else {
        countInputDevices--;
    }
    mutexCountInputDevicesReady.unlock();

    devIn = obj;

    if(!obj) {
        QTimer::singleShot(2000,this,SLOT(DeleteIfUnused()));
    }

    return true;
}


/*!
  Set or remove the output Object.
  The device is closed after 2 seconds if it has no input or output object
  \param obj the AudioDeviceOut, null to unlink
  \return true on success
  */
bool AudioDevice::SetObjectOutput(AudioDeviceOut *obj)
{
    QMutexLocker l(&mutexDevicesInOut);

    if(devOut && devOut == obj) {
        //it's the same object
        return true;
    }

    devOutClosing=false;

    if(devOut && obj) {
        debug("AudioDevice::SetObjectOutput already used")
        return false;
    }

    devOut = obj;

    if(!obj) {
        QTimer::singleShot(2000,this,SLOT(DeleteIfUnused()));
        devOutClosing=true;
    }

    return true;
}

/*!
  Reopen the device with the new sample rate
  \param rate the new rate
  */
void AudioDevice::SetSampleRate(float rate)
{
    if(!closed) {
        SetSleep(true);
        SetSleep(false);
    }
}


/*!
  Open the PortAudio stream, used by AudioDevice::Open
  \param sampleRate the desired sample rate
  \return true on success
  */
bool AudioDevice::OpenStream(double sampleRate)
{

    unsigned long framesPerBuffer = paFramesPerBufferUnspecified;

    PaStreamParameters *inputParameters = NULL;
    PaStreamParameters *outputParameters = NULL;
    PaStreamFlags flags = paClipOff; //paNoFlag;

    if(devInfo.maxInputChannels > 0) {

        inputParameters = new(PaStreamParameters);
        bzero( inputParameters, sizeof( PaStreamParameters ) );
        inputParameters->channelCount = devInfo.maxInputChannels;
        inputParameters->device = devId;
        inputParameters->hostApiSpecificStreamInfo = NULL;
        inputParameters->sampleFormat = paFloat32 | paNonInterleaved;
        inputParameters->suggestedLatency = Pa_GetDeviceInfo(devId)->defaultLowInputLatency ;

        switch(Pa_GetHostApiInfo( devInfo.hostApi )->type) {
            case paDirectSound :
                directSoundStreamInfo.size = sizeof(PaWinDirectSoundStreamInfo);
                directSoundStreamInfo.hostApiType = paDirectSound;
                directSoundStreamInfo.version = 1;
                //directSoundStreamInfo.flags = paWinDirectSoundUseChannelMask;
                //directSoundStreamInfo.channelMask = PAWIN_SPEAKER_5POINT1; /* request 5.1 output format */
                inputParameters->hostApiSpecificStreamInfo = &directSoundStreamInfo;
                break;
            case paMME :
                wmmeStreamInfo.size = sizeof(PaWinMmeStreamInfo);
                wmmeStreamInfo.hostApiType = paMME;
                wmmeStreamInfo.version = 1;
                wmmeStreamInfo.flags = paWinMmeUseLowLevelLatencyParameters | paWinMmeDontThrottleOverloadedProcessingThread;
                wmmeStreamInfo.framesPerBuffer = 512;
                wmmeStreamInfo.bufferCount = 8;//devInfo->maxInputChannels;
                inputParameters->hostApiSpecificStreamInfo = &wmmeStreamInfo;
                inputParameters->suggestedLatency = 0;
                break;
            case paASIO :
                break;
            case paSoundManager :
                break;
            case paCoreAudio :
                break;
            case paOSS :
                break;
            case paALSA :
                break;
            case paAL :
                break;
            case paBeOS :
                break;
            case paWDMKS :
                break;
            case paJACK :
                break;
            case paWASAPI :
                break;
            case paAudioScienceHPI :
                break;
            default:
                break;
        }
    }

    if(devInfo.maxOutputChannels > 0) {

        outputParameters = new(PaStreamParameters);
        bzero( outputParameters, sizeof( PaStreamParameters ) );
        outputParameters->channelCount = devInfo.maxOutputChannels;
        outputParameters->device = devId;
        outputParameters->hostApiSpecificStreamInfo = NULL;
        outputParameters->sampleFormat = paFloat32 | paNonInterleaved;
        outputParameters->suggestedLatency = Pa_GetDeviceInfo(devId)->defaultLowOutputLatency ;

        switch(Pa_GetHostApiInfo( devInfo.hostApi )->type) {
            case paDirectSound :
                directSoundStreamInfo.size = sizeof(PaWinDirectSoundStreamInfo);
                directSoundStreamInfo.hostApiType = paDirectSound;
                directSoundStreamInfo.version = 1;
                //directSoundStreamInfo.flags = paWinDirectSoundUseChannelMask;
                //directSoundStreamInfo.channelMask = PAWIN_SPEAKER_5POINT1; /* request 5.1 output format */
                outputParameters->hostApiSpecificStreamInfo = &directSoundStreamInfo;
                break;
            case paMME :
                wmmeStreamInfo.size = sizeof(PaWinMmeStreamInfo);
                wmmeStreamInfo.hostApiType = paMME;
                wmmeStreamInfo.version = 1;
                wmmeStreamInfo.flags = paWinMmeUseLowLevelLatencyParameters | paWinMmeDontThrottleOverloadedProcessingThread;
                wmmeStreamInfo.framesPerBuffer = 512;
                wmmeStreamInfo.bufferCount = 8;//devInfo->maxOutputChannels;
                outputParameters->hostApiSpecificStreamInfo = &wmmeStreamInfo;
                outputParameters->suggestedLatency = 0;
                break;
            case paASIO :
                break;
            case paSoundManager :
                break;
            case paCoreAudio :
                break;
            case paOSS :
                break;
            case paALSA :
                break;
            case paAL :
                break;
            case paBeOS :
                break;
            case paWDMKS :
                break;
            case paJACK :
                break;
            case paWASAPI :
                break;
            case paAudioScienceHPI :
                break;
            default :
                break;
        }
    }

    if(!Pa_IsFormatSupported( inputParameters, outputParameters, sampleRate ) == paFormatIsSupported) {
        debug("AudioDevice::OpenStream Pa_IsFormatSupported format not supported")
        errorMessage = tr("Stream format not supported");
        if(inputParameters)
            delete inputParameters;
        if(outputParameters)
            delete outputParameters;
        return false;
    }

    PaError err = Pa_OpenStream(
            &stream,
            inputParameters,
            outputParameters,
            sampleRate,
            framesPerBuffer,
            flags,
            paCallback, //your callback function
            (void *)this ); //data to be passed to callback. In C++, it is frequently (void *)this

    if(inputParameters)
        delete inputParameters;
    if(outputParameters)
        delete outputParameters;

    if( err != paNoError ) {
        Pa_CloseStream(stream);
        debug("AudioDevice::OpenStream Pa_OpenStream %s",Pa_GetErrorText( err ))
        errorMessage = Pa_GetErrorText( err );
        return false;
    }

    err = Pa_SetStreamFinishedCallback( stream, &paStreamFinished );
    if( err != paNoError ) {
        Pa_CloseStream(stream);
        debug("AudioDevice::OpenStream Pa_SetStreamFinishedCallback %s",Pa_GetErrorText( err ))
        errorMessage = Pa_GetErrorText( err );
        return false;
    }

    emit InUseChanged(objInfo,true);
//    const PaStreamInfo *inf = Pa_GetStreamInfo(&stream);
    return true;
}

/*!
  Open the device
  \return true on success
  */
bool AudioDevice::Open()
{
    //already opened
    if(!closed) {
        debug("AudioDevice::Open already opened")
        return true;
    }

    isClosing=false;

    //try to open at the host rate
    double sampleRate = myHost->GetSampleRate();
    if(!OpenStream(sampleRate)) {

        //if it fails, try to open with the default rate
        sampleRate = devInfo.defaultSampleRate;
        if(!OpenStream(sampleRate)) {
            return false;
        }

        //opened with the default rate, change the host rate
        //MainHost::Get()->SetSampleRate(sampleRate);
    }

    //start the stream
    PaError err = Pa_StartStream( stream );

    //failed to open the stream
    if( err != paNoError ) {

        debug("AudioDevice::Open %s",Pa_GetErrorText( err ))

        if(stream)
        {
            Pa_StopStream(stream);
            stream = 0;
        }

        return false;
    }

    for(int i=0; i<devInfo.maxInputChannels; i++ )
        listCircularBuffersIn << new CircularBuffer();
    for(int i=0; i<devInfo.maxOutputChannels; i++ )
        listCircularBuffersOut << new CircularBuffer();

    closed=false;
    return true;
}

/*!
  Close the PortAudio stream, used by AudioDevice::Close and AudioDevice::SetSleep
  \return true on success
  */
bool AudioDevice::CloseStream()
{
    if(isClosing) {
        debug("AudioDevice::CloseStream already closing")
        return false;
    }
    isClosing=true;
    closed=true;
    inputBufferReady=false;

    emit InUseChanged(objInfo,false);

    if(stream)
    {
        Pa_StopStream(stream);
        stream = 0;
    }

    DeleteCircualBuffers();

    if(myHost)
        myHost->SetBufferSize(1);
    return true;
}

/*!
  Delete ring buffers, used by AudioDevice::CloseStream
  */
void AudioDevice::DeleteCircualBuffers()
{
    foreach(CircularBuffer *buf, listCircularBuffersIn)
        delete buf;
    listCircularBuffersIn.clear();

    foreach(CircularBuffer *buf, listCircularBuffersOut)
        delete buf;
    listCircularBuffersOut.clear();

}

/*!
  Close the device
  \return true on success
  */
bool AudioDevice::Close()
{
    if(closed)
        return true;

    closed=true;

    mutexDevicesInOut.lock();
    if(devIn) {
        devIn->SetParentDevice(0);
        devIn=0;
    }
    if(devOut) {
        devOut->SetParentDevice(0);
        devOut=0;
    }
    mutexDevicesInOut.unlock();

    CloseStream();

    return true;
}

/*!
  Set the sleep state
  \param sleeping the new state
  */
void AudioDevice::SetSleep(bool sleeping)
{

    if(!sleeping)
        Open();

    mutexDevicesInOut.lock();
    if(devIn)
        devIn->SetSleep(sleeping);
    if(devOut)
        devOut->SetSleep(sleeping);
    mutexDevicesInOut.unlock();

    if(sleeping)
        CloseStream();
}

/*!
  Get the cpu usage returned by PortAudio
  \return cpu usage
  */
float AudioDevice::GetCpuUsage()
{
   return Pa_GetStreamCpuLoad(stream);
}

/*!
  PortAudio callback on stream finished, unused
  \param userData pointer to the corresponding AudioDevice
  */
void AudioDevice::paStreamFinished( void* userData )
{
//    AudioDevice* device = (AudioDevice*)userData;
//    debug("paStreamFinished %s",device->objectName().toAscii().constData())
}

bool AudioDevice::DeviceToRingBuffers( const void *inputBuffer, unsigned long framesPerBuffer)
{
    unsigned long hostBuffSize = myHost->GetBufferSize();
    if(framesPerBuffer > hostBuffSize) {
       myHost->SetBufferSize(framesPerBuffer);
       hostBuffSize = framesPerBuffer;
    }

    if(isClosing)
        return false;

    if(!devIn)
        return true;

    bool readyToRender=true;

    //fill circular buffer with device audio
    int cpt=0;
    foreach(CircularBuffer *buf, listCircularBuffersIn) {
        buf->Put( ((float **) inputBuffer)[cpt], framesPerBuffer );
        if(buf->filledSize < hostBuffSize )
            readyToRender=false;
        cpt++;
    }

    //if we filled enough buffer
    if(readyToRender)
        RingBuffersToPins();

    return true;
}

void AudioDevice::RingBuffersToPins()
{
    devIn->SetBufferFromRingBuffer(listCircularBuffersIn);

    if(!inputBufferReady) {
        inputBufferReady=true;

        QMutexLocker locker(&mutexCountInputDevicesReady);
        countDevicesReady++;
    }
}

void AudioDevice::PinsToRingBuffers()
{
    if(isClosing)
        return;

    if(devOut)
        devOut->SetRingBufferFromPins(listCircularBuffersOut);

    inputBufferReady=false;
}

bool AudioDevice::RingBuffersToDevice( void *outputBuffer, unsigned long framesPerBuffer)
{
    if(isClosing)
        return false;

    if(devOut) {
        //send circular buffer to device if there's enough data
        int cpt=0;
        foreach(CircularBuffer *buf, listCircularBuffersOut) {
            if(buf->filledSize>=framesPerBuffer)
                buf->Get( ((float **) outputBuffer)[cpt], framesPerBuffer );
            cpt++;
        }
    } else {
        if(devOutClosing) {
            //the device was removed : clear the output buffer one time
            devOutClosing=false;
            int cpt=0;
            foreach(CircularBuffer *buf, listCircularBuffersOut) {
                //empty the circular buffer, in case we reopen this device
                buf->Clear();
                //send a blank buffer to the device
                memcpy(((float **) outputBuffer)[cpt], AudioBuffer::blankBuffer, sizeof(float)*framesPerBuffer );
                cpt++;
            }
        }
    }

    return true;
}

/*!
  PortAudio callback
  put the audio provided by PortAudio in ring buffers
  starts a rendering loop when we have enough data
  send the resulting buffers back to PortAudio
  */
int AudioDevice::paCallback( const void *inputBuffer, void *outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* /*timeInfo*/,
                                 PaStreamCallbackFlags /*statusFlags*/,
                                 void *userData )
{
    AudioDevice* device = (AudioDevice*)userData;
    if(!device->DeviceToRingBuffers(inputBuffer, framesPerBuffer))
        return paComplete;

    //all devices are ready : render
    mutexCountInputDevicesReady.lock();
    if(countDevicesReady>=countInputDevices) {
        countDevicesReady=0;
        mutexCountInputDevicesReady.unlock();

        device->myHost->Render();
        device->myHost->audioDevices->PutPinsBuffersInRingBuffers();
    } else {
        mutexCountInputDevicesReady.unlock();
    }

    if(!device->RingBuffersToDevice(outputBuffer, framesPerBuffer))
        return paComplete;

    return paContinue;
}
