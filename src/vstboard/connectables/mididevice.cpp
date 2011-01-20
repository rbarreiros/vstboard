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

#include "mididevice.h"
#include "../globals.h"

using namespace Connectables;

MidiDevice::MidiDevice(MainHost *myHost,int index, const ObjectInfo &info) :
        Object(myHost,index, info),
        stream(0),
        queue(0),
        devInfo(0),
        deviceOpened(false)
{
}

MidiDevice::~MidiDevice()
{
    Close();
}

void MidiDevice::Render()
{
    if(!deviceOpened)
        return;

    if(devInfo->input) {
        PmEvent buffer;
        Lock();

        while (Pm_Dequeue(queue, &buffer) == 1) {
            foreach(Pin *pin,listMidiPinOut->listPins) {
                pin->SendMsg(PinMessage::MidiMsg,(void*)&buffer.message);
            }
        }
        Unlock();
    }
}

void MidiDevice::MidiMsgFromInput(long msg) {
    if(devInfo->output) {
        Lock();
        Pm_Enqueue(queue,(void*)&msg);
        Unlock();
    }
}

bool MidiDevice::OpenStream()
{
    if(deviceOpened)
        return true;

    QMutexLocker l(&objMutex);

    if(!FindDeviceByName()){
        errorMessage = tr("Device not found");
        return false;
    }

    queue = Pm_QueueCreate(QUEUE_SIZE, sizeof(PmEvent));
    if(!queue) {
        debug("MidiDevice::OpenStream can't create queue")
                return false;
    }

    if(objInfo.inputs>0) {
        PmError err = Pm_OpenInput(&stream, (PmDeviceID)objInfo.id, 0, 512, 0, 0);
        if (err!=pmNoError) {
            QString msgTxt;
            if(err==pmHostError) {
                char msg[255];
                Pm_GetHostErrorText(msg,255);
                debug("MidiDevice::OpenStream openInput host %s",msg)
                msgTxt=QString::fromAscii(msg);
            } else {
                debug("MidiDevice::OpenStream openInput %s",Pm_GetErrorText(err))
                msgTxt=QString::fromAscii(Pm_GetErrorText(err));
            }
            errorMessage=tr("Error while opening midi device %1 %2").arg(objInfo.name).arg(msgTxt);
            return false;
        }

        err = Pm_SetFilter(stream, PM_FILT_ACTIVE | PM_FILT_SYSEX | PM_FILT_CLOCK);
        if (err!=pmNoError) {
            QString msgTxt;
            if(err==pmHostError) {
                char msg[20];
                unsigned int len=20;
                Pm_GetHostErrorText(msg,len);
                debug("MidiDevice::OpenStream setFilter host %s",msg)
                msgTxt=QString::fromAscii(msg);
            } else {
                msgTxt=QString::fromAscii(Pm_GetErrorText(err));
                debug("MidiDevice::OpenStream setFilter %s",Pm_GetErrorText(err))
            }
            errorMessage=tr("Error while opening midi device %1 %2").arg(objInfo.name).arg(msgTxt);
            return false;
        }
    }

    if(objInfo.outputs>0) {
        PmError err = Pm_OpenOutput(&stream, (PmDeviceID)objInfo.id, 0, 512, 0, 0, 0);
        if (err!=pmNoError) {
            QString msgTxt;
            if(err==pmHostError) {
                char msg[20];
                unsigned int len=20;
                Pm_GetHostErrorText(msg,len);
                debug("MidiDevice::Open openInput host %s",msg)
                msgTxt=QString::fromAscii(msg);
            } else {
                debug("MidiDevice::Open openInput %s",Pm_GetErrorText(err))
                msgTxt=QString::fromAscii(Pm_GetErrorText(err));
            }
            errorMessage=tr("Error while opening midi device %1 %2").arg(objInfo.name).arg(msgTxt);
            return false;
        }
    }

    deviceOpened=true;
//    SetSleep(false);
    return true;
}

bool MidiDevice::CloseStream()
{
    if(!deviceOpened)
        return true;

//    SetSleep(true);


    QMutexLocker l(&objMutex);

    PmError err = pmNoError;

    err = Pm_Close(stream);
    if(err!=pmNoError) {
        debug("MidiDevice::Close error closing midi port");
    }

    err = Pm_QueueDestroy(queue);
    if(err!=pmNoError) {
        debug("error closing midi queue");
    }

    deviceOpened=false;

    return true;
}

bool MidiDevice::Close()
{
    CloseStream();
    return true;
}


bool MidiDevice::FindDeviceByName()
{
    int cptDuplicateNames=0;
    int canBe=-1;
    int deviceNumber=-1;

    for(int i=0;i<Pm_CountDevices();i++) {
        const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
        if(QString::fromStdString(info->name) == objInfo.name
           && info->input == objInfo.inputs
           && info->output == objInfo.outputs) {
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
            devInfo = Pm_GetDeviceInfo(deviceNumber);
        } else {
            errorMessage=tr("Error : device was deleted");
            debug("MidiDevice::FindDeviceByName device not found")
            return false;
        }
    }

    objInfo.id = deviceNumber;
    return true;
}

bool MidiDevice::Open()
{
    errorMessage="";
    closed=false;

    //device not found, create a dummy object
    if(!FindDeviceByName()){
        errorMessage = tr("Device not found");
        return true;
    }

    //error while opening device, delete it now
    if(!OpenStream()) {
        return false;
    }
    listMidiPinOut->ChangeNumberOfPins(devInfo->input);
    listMidiPinIn->ChangeNumberOfPins(devInfo->output);

    Object::Open();
    return true;
}
