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

#include "mididevice.h"
#include "globals.h"
#include "mainhost.h"
#include "vst.h"

using namespace Connectables;

MidiDevice::MidiDevice(MainHost *myHost, int index, const ObjectInfo &info) :
        Object(myHost,index, info),
        queue(0)
{
}

MidiDevice::~MidiDevice()
{
    Close();
}

void MidiDevice::Render()
{
    if(objInfo.inputs) {
        PmEvent buffer;
        Lock();

        while (Pm_Dequeue(queue, &buffer) == 1) {
            foreach(MidiPinOut *pin,listMidiPinOut) {
                pin->SendMsg(PinMessage::MidiMsg,(void*)&buffer.message);
            }
        }
        Unlock();
    }
}

void MidiDevice::MidiMsgFromInput(long msg) {
    if(objInfo.outputs) {
        Lock();
        Pm_Enqueue(queue,(void*)&msg);
        Unlock();
    }
}

bool MidiDevice::Close()
{
    if(objInfo.inputs>0)
        myHost->myVstPlugin->removeMidiIn(this);

    if(objInfo.outputs>0)
        myHost->myVstPlugin->removeMidiOut(this);

    return Object::Close();
}

bool MidiDevice::Open()
{
    if(objInfo.inputs>0)
        myHost->myVstPlugin->addMidiIn(this);

    if(objInfo.outputs>0)
        myHost->myVstPlugin->addMidiOut(this);



    queue = Pm_QueueCreate(QUEUE_SIZE, sizeof(PmEvent));
    if(!queue) {
        debug("MidiDevice::Open can't create queue")
                return false;
    }

    for(int i=0;i<objInfo.inputs;i++) {
        MidiPinOut *pin = new MidiPinOut(this);
        listMidiPinOut << pin;
    }

    for(int i=0;i<objInfo.outputs;i++) {
        MidiPinIn *pin = new MidiPinIn(this);
        listMidiPinIn << pin;
    }

    Object::Open();
    return true;
}
