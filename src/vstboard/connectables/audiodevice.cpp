/******************************************************************************
#    Copyright 2010 Raphaël François
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
******************************************************************************/

#include "audiodevice.h"
#include "object.h"
#include "audiodevicein.h"
#include "audiodeviceout.h"
#include "../globals.h"
#include "../audiobuffer.h"
#include "../renderer.h"
#include "../mainhost.h"
#include "../audiodevices.h"

using namespace Connectables;

//QHash<int,QSharedPointer<AudioDevice> >AudioDevice::listAudioDevices;
QMutex AudioDevice::listDevMutex;
int AudioDevice::countDevicesReady=0;
int AudioDevice::countInputDevices=0;

AudioDevice::AudioDevice(MainHost *myHost,const ObjectInfo &info, QObject *parent) :
    QObject(parent),
    bufferReady(false),
    sampleRate(44100.0f),
    bufferSize(4096),
    stream(0),
    devIn(0),
    devOut(0),
    closed(true),
    objInfo(info),
    cpuUsage(.0f),
    myHost(myHost)
{
//    QSharedPointer<AudioDevice> sptr(this);
//    sharedPointer=sptr;
//    listAudioDevices.insert(objInfo.id, sharedPointer);

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

void AudioDevice::DeleteIfUnused()
{
    if(isClosing)
        return;

    bool del=false;
    devicesMutex.lock();
    if(!devIn && !devOut)
        del=true;
    devicesMutex.unlock();

    if(del) {
        SetSleep(true);
        listDevMutex.lock();
        myHost->audioDevices->listAudioDevices.remove(objInfo.id);
        listDevMutex.unlock();
    }

}

bool AudioDevice::SetObjectInput(AudioDeviceIn *obj)
{
    QMutexLocker l(&devicesMutex);

    if(devIn && devIn == obj) {
        //it's the same object
        return true;
    }

    if(devIn && obj) {
        debug("AudioDevice::SetObjectInput already used")
        return false;
    }

    if(obj) {
        countInputDevices++;
    } else {
        countInputDevices--;
    }

    devIn = obj;

    if(!obj) {
        QTimer::singleShot(2000,this,SLOT(DeleteIfUnused()));
    }

    return true;
}

bool AudioDevice::SetObjectOutput(AudioDeviceOut *obj)
{
    QMutexLocker l(&devicesMutex);

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

void AudioDevice::SetSampleRate(float rate)
{
    if(!closed) {
        SetSleep(true);
        SetSleep(false);
    }
}

bool AudioDevice::FindDeviceByName(ObjectInfo &objInfo, PaDeviceInfo *devInfo)
{
    int cptDuplicateNames=0;
    int canBe=-1;
    int deviceNumber=-1;

    for(int i=0;i<Pa_GetDeviceCount();i++) {
        const PaDeviceInfo *info = Pa_GetDeviceInfo(i);
        if(QString::fromStdString(info->name) == objInfo.name
           && info->maxInputChannels == objInfo.inputs
           && info->maxOutputChannels == objInfo.outputs) {
            //can be this one, but the interface number can change form a comp to another
            if(cptDuplicateNames==0)
                canBe=i;

            //we found the same number and the same name
            if(objInfo.duplicateNamesCounter == cptDuplicateNames) {
                if(devInfo)
                    *devInfo = *info;
                deviceNumber = i;
                break;
            }
            cptDuplicateNames++;
        }
    }

    //didn't found an exact match
    if(deviceNumber==-1) {
        //but we found a device with the same name
        if(canBe!=-1) {
            deviceNumber=canBe;
            if(devInfo)
                *devInfo = *Pa_GetDeviceInfo(deviceNumber);
        } else {
            debug("AudioDevice::FindDeviceFromName device not found")
            return false;
        }
    }

    objInfo.id = deviceNumber;
    return true;
}

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
        inputParameters->device = objInfo.id;
        inputParameters->hostApiSpecificStreamInfo = NULL;
        inputParameters->sampleFormat = paFloat32 | paNonInterleaved;
        inputParameters->suggestedLatency = Pa_GetDeviceInfo(objInfo.id)->defaultLowInputLatency ;

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
        outputParameters->device = objInfo.id;
        outputParameters->hostApiSpecificStreamInfo = NULL;
        outputParameters->sampleFormat = paFloat32 | paNonInterleaved;
        outputParameters->suggestedLatency = Pa_GetDeviceInfo(objInfo.id)->defaultLowOutputLatency ;

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
        return false;
    }

    err = Pa_SetStreamFinishedCallback( stream, &paStreamFinished );
    if( err != paNoError ) {
        Pa_CloseStream(stream);
        debug("AudioDevice::OpenStream Pa_SetStreamFinishedCallback %s",Pa_GetErrorText( err ))
        return false;
    }

    emit InUseChanged(objInfo,true);
//    const PaStreamInfo *inf = Pa_GetStreamInfo(&stream);
    return true;
}

bool AudioDevice::Open()
{
    //already opened
    if(!closed) {
        debug("AudioDevice::Open already opened")
        return true;
    }

    isClosing=false;

//    debug("%s open",objectName().toAscii().constData())

    //find the corresponding device
    if(!FindDeviceByName(objInfo,&devInfo)) {
        return false;
    }

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
            err = Pa_IsStreamActive(stream);
            err = Pa_AbortStream(stream);
            err = Pa_CloseStream(stream);
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

bool AudioDevice::CloseStream()
{
    devicesMutex.lock();
    if(isClosing) {
        devicesMutex.unlock();
        debug("AudioDevice::CloseStream already closing")
        return false;
    }
    isClosing=true;
    closed=true;

    emit InUseChanged(objInfo,false);

//    foreach(CircularBuffer *buf, listCircularBuffersOut) {
//        buf->Clear();
//    }
//    foreach(CircularBuffer *buf, listCircularBuffersIn) {
//        buf->Clear();
//    }


    devicesMutex.unlock();

    bufferReady=false;

    PaError err;

    if(stream)
    {
        err = Pa_IsStreamActive(stream);
        if( err < 0 ) {
            debug("AudioDevice::CloseStream Pa_IsStreamActive %s %s",objectName().toAscii().constData(),Pa_GetErrorText( err ))

            //error : abort stream

            err = Pa_AbortStream(stream);
            if( err != paNoError ) {
                debug("AudioDevice::CloseStream Pa_AbortStream %s %s",objectName().toAscii().constData(),Pa_GetErrorText( err ))
            }

        } else {
            //no error : clean close stream

            if( err > 0 ) {
                //stream is active
                debug("pa_stopstream %s",objectName().toAscii().constData())
                err = Pa_StopStream(stream);
                if( err != paNoError ) {
                    debug("AudioDevice::CloseStream Pa_StopStream %s %s",objectName().toAscii().constData(),Pa_GetErrorText( err ))

//                    //retry
//                    Sleep(1000);
//                    err = Pa_StopStream(stream);
//                    if( err != paNoError ) {
//                        debug("AudioDevice::CloseStream Pa_StopStream 2 %s %s",objectName().toAscii().constData(),Pa_GetErrorText( err ))
//                        DeleteCircualBuffers();
//                        return false;
//                    }
                }
            }

            debug("pa_closestream")
            err = Pa_CloseStream(stream);
            if( err != paNoError ) {
                debug("AudioDevice::CloseStream Pa_CloseStream %s %s",objectName().toAscii().constData(),Pa_GetErrorText( err ))

//                //retry
//                Sleep(1000);
//                err = Pa_CloseStream(stream);
//                if( err != paNoError ) {
//                    debug("AudioDevice::CloseStream Pa_CloseStream 2 %s %s",objectName().toAscii().constData(),Pa_GetErrorText( err ))
//                    DeleteCircualBuffers();
//                    return false;
//                }
            }
        }
        stream = 0;
    }

    DeleteCircualBuffers();

    if(myHost)
        myHost->SetBufferSize(1);
    return true;
}

void AudioDevice::DeleteCircualBuffers()
{
    foreach(CircularBuffer *buf, listCircularBuffersIn)
        delete buf;
    listCircularBuffersIn.clear();

    foreach(CircularBuffer *buf, listCircularBuffersOut)
        delete buf;
    listCircularBuffersOut.clear();

}

bool AudioDevice::Close()
{
    if(closed)
        return true;

    closed=true;

    devicesMutex.lock();

    if(devIn) {
        devIn->parentDevice.clear();
        devIn=0;
    }
    if(devOut) {
        devOut->parentDevice.clear();
        devOut=0;
    }
    devicesMutex.unlock();

    CloseStream();

    return true;
}

void AudioDevice::SetSleep(bool sleeping)
{

    if(!sleeping)
        Open();

    devicesMutex.lock();
    if(devIn)
        devIn->SetSleep(sleeping);
    if(devOut)
        devOut->SetSleep(sleeping);
    devicesMutex.unlock();

    if(sleeping)
        CloseStream();
}

float AudioDevice::GetCpuUsage()
{
   return Pa_GetStreamCpuLoad(stream);
}

void AudioDevice::paStreamFinished( void* userData )
{
//    AudioDevice* device = (AudioDevice*)userData;
//    debug("paStreamFinished %s",device->objectName().toAscii().constData())
}

int AudioDevice::paCallback( const void *inputBuffer, void *outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* /*timeInfo*/,
                                 PaStreamCallbackFlags /*statusFlags*/,
                                 void *userData )
{
    AudioDevice* device = (AudioDevice*)userData;

    if(!device->myHost)
        return paComplete;

    unsigned long hostBuffSize = device->myHost->GetBufferSize();
    if(framesPerBuffer > hostBuffSize) {
       device->myHost->SetBufferSize(framesPerBuffer);
       hostBuffSize = framesPerBuffer;
    }

    {
        QMutexLocker devLock(&device->devicesMutex);

        if(device->isClosing)
            return paComplete;

        if(device->devIn) {

            bool readyToRender=true;

            //fill circular buffer with device audio
            int cpt=0;
            foreach(CircularBuffer *buf, device->listCircularBuffersIn) {

                buf->Put( ((float **) inputBuffer)[cpt], framesPerBuffer );

                if(buf->filledSize < hostBuffSize )
                    readyToRender=false;

                cpt++;
            }

            //if we filled enough buffer
            //if(device->listCircularBuffersIn.at(0)->filledSize >= hostBuffSize ) {
            if(readyToRender) {
                //put circular buffers into pins buffers
                cpt=0;
                foreach(CircularBuffer *buf, device->listCircularBuffersIn) {
                    AudioBuffer *pinBuf = device->devIn->listAudioPinOut.at(cpt)->buffer;

                    if(pinBuf->GetSize() < hostBuffSize) {
                        pinBuf->SetSize(hostBuffSize);
//                        debug("AudioDevice::paCallback pin buffer too small")
                        continue;
                    }

                    if(buf->filledSize >= hostBuffSize)
                        buf->Get( pinBuf->GetPointer(true), hostBuffSize );

                    cpt++;
                }

                if(!device->bufferReady) {
                    device->bufferReady=true;

                    QMutexLocker locker(&listDevMutex);
                    countDevicesReady++;
                }
            }
        }
    }


    {
        QMutexLocker locker(&listDevMutex);

        //all devices are ready : render
        if(countDevicesReady>=countInputDevices) {
            countDevicesReady=0;

            device->myHost->Render();

            foreach(QSharedPointer<AudioDevice>dev, device->myHost->audioDevices->listAudioDevices) {
                if(dev.isNull())
                    continue;

                QMutexLocker devLock(&dev->devicesMutex);

                if(dev->isClosing)
                    continue;

                if(dev->devOut) {
                    int cpt=0;
                    //put pins buffer into circular buffers
                    foreach(CircularBuffer *buf, dev->listCircularBuffersOut) {
                        AudioBuffer *pinBuf = dev->devOut->listAudioPinIn.at(cpt)->buffer;
    //                    if(!pinBuf->IsEmpty()) {
                            buf->Put( pinBuf->ConsumeStack(), pinBuf->GetSize() );
                            pinBuf->ResetStackCounter();
    //                    }
                        cpt++;
                    }
                }
                dev->bufferReady=false;
            }
        }
    }


    {
//        QMutexLocker devLock(&device->devicesMutex);
        if(device->isClosing)
            return paComplete;

        if(device->devOut) {
            //send circular buffer to device if there's enough data
            int cpt=0;
            foreach(CircularBuffer *buf, device->listCircularBuffersOut) {
//                while(buf->filledSize >= hostBuffSize+framesPerBuffer ) {
//                    debug2(<< "AudioDevice::paCallback skip buffer filled:" << buf->filledSize << " host:" << hostBuffSize << " frame:" << framesPerBuffer )
//                    buf->Skip(framesPerBuffer);
//                    debug2(<< buf->filledSize)
//                }

                if(buf->filledSize>=framesPerBuffer)
                    buf->Get( ((float **) outputBuffer)[cpt], framesPerBuffer );
                cpt++;
            }
        } else {
            if(device->devOutClosing) {
                //the device was removed : clear the output buffer one time
                device->devOutClosing=false;
                int cpt=0;
                foreach(CircularBuffer *buf, device->listCircularBuffersOut) {
                    //empty the circular buffer, in case we reopen this device
                    buf->Clear();
                    //send a blank buffer to the device
                    memcpy(((float **) outputBuffer)[cpt], AudioBuffer::blankBuffer, sizeof(float)*framesPerBuffer );
                    cpt++;
                }
            }
        }
    }

    return paContinue;

}
