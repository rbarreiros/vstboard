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

#include "midisender.h"
#include "../globals.h"

using namespace Connectables;

//QList<QVariant> MidiSender::listMsgType;
//QList<QVariant> MidiSender::listValues;
//QList<QVariant> MidiSender::listChannels;

MidiSender::MidiSender(int index) :
        Object(index, ObjectInfo(NodeType::object, ObjType::MidiSender, tr("MidiSender")) ),
    midiMsg(0),
    msgChanged(false)
{

//    if(listMsgType.isEmpty()) {
        listMsgType << "Note off";
        listMsgType << "Note on";
        listMsgType << "Ctrl";
        listMsgType << "ProgChang";
//    }
//    if(listValues.isEmpty()) {
        for(int i=0;i<128;i++) {
            listValues << i;
        }
//    }
//    if(listChannels.isEmpty()) {
        for(int i=0;i<16;i++) {
            listChannels << (i+1);
        }
//    }

    listMidiPinOut << new MidiPinOut(this);

    listParameterPinIn.insert(Param_MsgType, new ParameterPinIn(this,Param_MsgType,"Ctrl",&listMsgType,true,"MsgType"));
    listParameterPinIn.insert(Param_Value1, new ParameterPinIn(this,Param_Value1,0,&listValues,true,"Value1"));
    listParameterPinIn.insert(Param_Value2, new ParameterPinIn(this,Param_Value2,0,&listValues,true,"Value2"));
    listParameterPinIn.insert(Param_Channel, new ParameterPinIn(this,Param_Channel,1,&listChannels,true,"Channel"));


}


void MidiSender::Render()
{
    if(!msgChanged)
        return;

    msgChanged=false;

    unsigned char status=0;

    unsigned char msgType = listParameterPinIn.value(Param_MsgType)->GetIndex();
    unsigned char value1 = listParameterPinIn.value(Param_Value1)->GetVariantValue().toInt();
    unsigned char value2 = listParameterPinIn.value(Param_Value2)->GetVariantValue().toInt();
    unsigned char channel = listParameterPinIn.value(Param_Channel)->GetVariantValue().toInt();

    switch(msgType) {
        case 0:
            status = MidiConst::noteOff;
            break;
        case 1:
            status = MidiConst::noteOn;
            break;
        case 2:
            status = MidiConst::ctrl;
            break;
        case 3:
            status = MidiConst::prog;
            break;
    }

    status |= channel;

    midiMsg = Pm_Message(status, value1, value2);

    listMidiPinOut.first()->SendMsg(PinMessage::MidiMsg, (void*)&midiMsg);
}

void MidiSender::OnParameterChanged(ConnectionInfo /*pinInfo*/, float /*value*/)
{
    msgChanged=true;
}
