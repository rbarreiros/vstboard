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

#include "vstaudiodevice.h"
#include "connectables/object.h"
#include "vstaudiodevicein.h"
#include "vstaudiodeviceout.h"
#include "globals.h"
#include "audiobuffer.h"
#include "renderer.h"
#include "mainhost.h"

using namespace Connectables;


int VstAudioDevice::countDevicesReady=0;
int VstAudioDevice::countInputDevices=0;

VstAudioDevice::VstAudioDevice(const ObjectInfo &info, QObject *parent) :
    QObject(parent),
    bufferReady(false),
    sampleRate(44100.0f),
    bufferSize(4096),
    devIn(0),
    devOut(0),
    closed(true)
{
    devOutClosing=false;
    setObjectName(objInfo.name);

    connect(MainHost::Get(),SIGNAL(SampleRateChanged(float)),
            this,SLOT(SetSampleRate(float)));

    connect(this,SIGNAL(InUseChanged(ObjectInfo,bool)),
            MainHost::Get(),SIGNAL(OnVstAudioDeviceToggleInUse(ObjectInfo,bool)));
}

VstAudioDevice::~VstAudioDevice()
{
    Close();
    debug("%s deleted",objectName().toAscii().constData())
}

void VstAudioDevice::DeleteIfUnused()
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
    }

}

bool VstAudioDevice::SetObjectInput(VstAudioDeviceIn *obj)
{
    QMutexLocker l(&devicesMutex);

    if(devIn && devIn == obj) {
        //it's the same object
        return true;
    }

    if(devIn && obj) {
        debug("VstAudioDevice::SetObjectInput already used")
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

bool VstAudioDevice::SetObjectOutput(VstAudioDeviceOut *obj)
{
    QMutexLocker l(&devicesMutex);

    if(devOut && devOut == obj) {
        //it's the same object
        return true;
    }

    devOutClosing=false;

    if(devOut && obj) {
        debug("VstAudioDevice::SetObjectOutput already used")
        return false;
    }

    devOut = obj;

    if(!obj) {
        QTimer::singleShot(2000,this,SLOT(DeleteIfUnused()));
        devOutClosing=true;
    }

    return true;
}

void VstAudioDevice::SetSampleRate(float rate)
{
}


bool VstAudioDevice::OpenStream(double sampleRate)
{
    return true;
}

bool VstAudioDevice::Open()
{

    return true;
}

bool VstAudioDevice::CloseStream()
{
    return true;
}

void VstAudioDevice::DeleteCircualBuffers()
{

}

bool VstAudioDevice::Close()
{
    return true;
}

void VstAudioDevice::SetSleep(bool sleeping)
{

}

