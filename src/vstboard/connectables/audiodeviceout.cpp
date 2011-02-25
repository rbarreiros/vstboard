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

#include "connectables/audiodeviceout.h"
#include "connectables/audiodevice.h"
#include "globals.h"
#include "audiobuffer.h"
#include "mainhosthost.h"
#include "audiodevices.h"

using namespace Connectables;

/*!
  \class Connectables::AudioDeviceOut
  \brief an audio device output. used by AudioDevice
  */

/*!
  \param myHost pointer to the MainHost
  \param index object number
  \param info object description
  */
AudioDeviceOut::AudioDeviceOut(MainHost *myHost,int index, const ObjectInfo &info) :
    Object(myHost,index, info),
    parentDevice(0)
{
}

AudioDeviceOut::~AudioDeviceOut()
{
    Close();
}

bool AudioDeviceOut::Close()
{
    if(parentDevice) {
        parentDevice->SetObjectOutput(0);
        parentDevice.clear();
    }
    return true;
}

bool AudioDeviceOut::Open()
{
    errorMessage="";

    //can we find this device on this computer ?
    if(!AudioDevice::FindDeviceByName(objInfo)) {
        errorMessage = tr("Device not found");
        return true;
    }

    closed=false;

    //create the audiodevice if needed
    if(!parentDevice) {
        QMutexLocker l(&AudioDevice::listDevMutex);
        MainHostHost *host=static_cast<MainHostHost*>(myHost);

        if(!host->audioDevices->listAudioDevices.contains(objInfo.id)) {
            AudioDevice *dev = new AudioDevice(host,objInfo);
            parentDevice = QSharedPointer<AudioDevice>(dev);
            if(!parentDevice->Open()) {
                parentDevice.clear();
                errorMessage=tr("Error while opening the interface");
                return true;
            }
            host->audioDevices->listAudioDevices.insert(objInfo.id, parentDevice);
        } else {
            parentDevice = host->audioDevices->listAudioDevices.value(objInfo.id);
        }
    }

    if(!parentDevice) {
        errorMessage=tr("Error : device was deleted");
        return true;
    }

    //if no output channels
    if(parentDevice->devInfo.maxOutputChannels==0) {
        parentDevice.clear();
        //should be deleted : return false
        return false;
    }

    listAudioPinIn->ChangeNumberOfPins( parentDevice->devInfo.maxOutputChannels );

    //device already has a child
    if(!parentDevice->SetObjectOutput(this)) {
        parentDevice.clear();
        errorMessage=tr("Already in use");
        return true;
    }

    Object::Open();
    return true;
}
