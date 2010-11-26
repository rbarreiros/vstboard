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

#include "midipinin.h"
#include "pin.h"
#include "object.h"
#include "mididevice.h"
#include "../globals.h"

using namespace Connectables;

MidiPinIn::MidiPinIn(Object *parent, int number, bool bridge)
    :Pin(parent,PinType::Midi,PinDirection::Input,number,bridge)
{
//    activity = 0;

    falloff = 0.05f;

    setObjectName(QString("MidiIn%1").arg(number));
    visible=true;
}


void MidiPinIn::ReceiveMsg(const int msgType,void* data)
{
    if(msgType==PinMessage::MidiMsg) {
        parent->MidiMsgFromInput(*(long*)data);
        value+=.2f;
        valueChanged=true;
    }
}


float MidiPinIn::GetValue()
{
    float v=value;
    if(v>1.0f) v=1.0f;
    value=.0f;
    return v;
}

//void MidiPinIn::SetActivity(int channel)
//{
//    activity &= 1 << channel;
//}

//int MidiPinIn::GetActivity()
//{
//    int act = activity;
//    activity = 0;
//    return act;
//}
