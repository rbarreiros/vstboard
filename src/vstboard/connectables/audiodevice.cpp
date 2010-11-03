/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
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


using namespace Connectables;

QHash<int,AudioDevice*>AudioDevice::listAudioDevices;

AudioDevice::AudioDevice(const ObjectInfo &info, QObject *parent) :
    QObject(parent),
    sampleRate(44100.0f),
    bufferSize(4096),
    stream(0),
    devInfo(0),
    devIn(0),
    devOut(0),
    closeFlag(false),
    closed(true),
    objInfo(info)
{
    setObjectName(objInfo.name);
    listAudioDevices.insert(objInfo.id,this);

    connect(MainHost::Get(),SIGNAL(SampleRateChanged(float)),
            this,SLOT(SetSampleRate(float)));
}

AudioDevice::~AudioDevice()
{
    Close();
    listAudioDevices.remove(objInfo.id);

    debug("%s deleted",objectName().toAscii().constData())
}

bool AudioDevice::SetObjectInput(AudioDeviceIn *obj)
{
    if(devIn && obj) {
        debug("AudioDevice::SetObjectInput already used")
        return false;
    }

    devIn = obj;
    return true;
}

bool AudioDevice::SetObjectOutput(AudioDeviceOut *obj)
{
    if(devOut && obj) {
        debug("AudioDevice::SetObjectOutput already used")
        return false;
    }

    devOut = obj;
    return true;
}

void AudioDevice::SetSampleRate(float rate)
{
    if(!closed) {
        closeFlag=true;
        QMutexLocker lock(&objMutex);
        CloseStream();
        Open();
    }
}

bool AudioDevice::FindDeviceFromName()
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
                devInfo = info;
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
            devInfo = Pa_GetDeviceInfo(deviceNumber);
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

    if(devInfo->maxInputChannels > 0) {
        inputParameters = new(PaStreamParameters);
        bzero( inputParameters, sizeof( PaStreamParameters ) );
        inputParameters->channelCount = devInfo->maxInputChannels;
        inputParameters->device = objInfo.id;
        inputParameters->hostApiSpecificStreamInfo = NULL;
        inputParameters->sampleFormat = paFloat32 | paNonInterleaved;
        inputParameters->suggestedLatency = Pa_GetDeviceInfo(objInfo.id)->defaultLowInputLatency ;
    }

    if(devInfo->maxOutputChannels > 0) {
        outputParameters = new(PaStreamParameters);
        bzero( outputParameters, sizeof( PaStreamParameters ) );
        outputParameters->channelCount = devInfo->maxOutputChannels;
        outputParameters->device = objInfo.id;
        outputParameters->hostApiSpecificStreamInfo = NULL;
        outputParameters->sampleFormat = paFloat32 | paNonInterleaved;
        outputParameters->suggestedLatency = Pa_GetDeviceInfo(objInfo.id)->defaultLowOutputLatency ;
    }

    PaError err = Pa_OpenStream(
            &stream,
            inputParameters,
            outputParameters,
            sampleRate,
            framesPerBuffer,
            paNoFlag, //flags that can be used to define dither, clip settings and more
            paCallback, //your callback function
            (void *)this ); //data to be passed to callback. In C++, it is frequently (void *)this

    if(inputParameters)
        delete inputParameters;
    if(outputParameters)
        delete outputParameters;

    if( err != paNoError ) {
        debug(Pa_GetErrorText( err ))
        return false;
    }

    return true;
}

bool AudioDevice::Open()
{
//    debug("%s open",objectName().toAscii().constData())

    //find the corresponding device
    if(!FindDeviceFromName()) {
        return false;
    }

    //try to open at the host rate
    double sampleRate = MainHost::Get()->GetSampleRate();
    if(!OpenStream(sampleRate)) {

        //if it fails, try to open with the default rate
        sampleRate = devInfo->defaultSampleRate;
        if(!OpenStream(sampleRate)) {
            return false;
        }

        //opened with the default rate, change the host rate
        MainHost::Get()->SetSampleRate(sampleRate);
    }
    closeFlag=false;

    //start the stream
    PaError err = Pa_StartStream( stream );

    //failed to open the stream
    if( err != paNoError ) {

        debug(Pa_GetErrorText( err ))

        if(stream)
        {
            err = Pa_IsStreamActive(stream);
            err = Pa_AbortStream(stream);
            err = Pa_CloseStream(stream);
            stream = 0;
        }

        return false;
    }

    closed=false;
    return true;
}

bool AudioDevice::CloseStream()
{
//    debug("AudioDevice::CloseStream")

    PaError err;

    if(stream)
    {
        err = Pa_IsStreamActive(stream);
        if( err < 0 ) {
            debug(Pa_GetErrorText( err ))
        } else if(err>0) {
            //err = Pa_StopStream( stream );
            err = Pa_AbortStream(stream);
            if( err != paNoError ) {
                debug(Pa_GetErrorText( err ))
            }

            err = Pa_CloseStream(stream);
            if( err != paNoError ) {
                debug(Pa_GetErrorText( err ))
            }
        }

        stream = 0;
    }

    return true;
}

bool AudioDevice::Close()
{
    if(closed)
        return true;

//    debug("%s close",objectName().toAscii().constData())

    closeFlag=true;
    QMutexLocker lock(&objMutex);

    if(devIn) {
        devIn->parentDevice=0;
        devIn=0;
    }
    if(devOut) {
        devOut->parentDevice=0;
        devOut=0;
    }

    CloseStream();

    return true;
}

void AudioDevice::SetSleep(bool sleeping)
{
    if(devIn)
        devIn->SetSleep(sleeping);
    if(devOut)
        devOut->SetSleep(sleeping);
}

void AudioDevice::Render()
{
    //emit CpuLoad((float)100*Pa_GetStreamCpuLoad(stream));
}

int AudioDevice::paCallback( const void *inputBuffer, void *outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* /*timeInfo*/,
                                 PaStreamCallbackFlags /*statusFlags*/,
                                 void *userData )
{
    Q_ASSERT(userData);
    AudioDevice* device = (AudioDevice*)userData;

    QMutexLocker lock(&device->objMutex);

    int cpt = 0;
    if(framesPerBuffer != device->bufferSize) {
//        debug("AudioDevice::paCallback buffer size %ld -> %ld",device->bufferSize,framesPerBuffer)
        device->bufferSize = framesPerBuffer;
        MainHost::Get()->SetBufferSize((long)framesPerBuffer);
    }

    //set buffers pointers on output pins
    if(device->devOut) {
        foreach(AudioPinIn* pin,device->devOut->listAudioPinIn) {
            pin->buffer->SetPointer( ((float **) outputBuffer)[cpt], true );
            pin->buffer->SetSize(framesPerBuffer);
            cpt++;
        }
    }

    cpt = 0;
    //set buffers pointers on input pins
    if(device->devIn) {
        foreach(AudioPinOut* pin,device->devIn->listAudioPinOut) {
            pin->buffer->SetPointer( ((float **) inputBuffer)[cpt] );
            pin->buffer->SetSize(framesPerBuffer);
            pin->buffer->ConsumeStack();
            cpt++;
        }
    }

    //render everything
    MainHost::Get()->Render(framesPerBuffer);

    //consume resulting buffers (for audio level calculation only, we already own the pointers)
    if(device->devOut) {
        foreach(AudioPinIn* pin,device->devOut->listAudioPinIn) {
            pin->buffer->ConsumeStack();
        }
    }

    if(device->closeFlag) {
        debug("AudioDevice::paCallback paAbort")
        return paAbort;
    }

    return paContinue;

}
