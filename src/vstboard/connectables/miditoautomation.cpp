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

#include "miditoautomation.h"
#include "midipinin.h"
#include "../globals.h"
#include "mainhost.h"
#include "portmidi.h"

using namespace Connectables;

MidiToAutomation::MidiToAutomation(MainHost *myHost,int index) :
        Object(myHost,index, ObjectInfo(NodeType::object, ObjType::MidiToAutomation, tr("Midi->Automation")) )
{
    for(int i=0;i<128;i++) {
        listValues << i;
    }

    listMidiPinIn->ChangeNumberOfPins(1);
    static_cast<ParameterPinIn*>(listParameterPinIn->listPins.value(FixedPinNumber::learningMode))->SetAlwaysVisible(true);
}


void MidiToAutomation::Render()
{
//    QMutexLocker l(&objMutex);

    if(listChanged.isEmpty())
        return;

    QHash<quint16,quint8>::Iterator i = listChanged.begin();
    while(i!=listChanged.end()) {
        ParameterPinOut *pin = static_cast<ParameterPinOut*>(listParameterPinOut->listPins.value(i.key(),0));
        if(pin)
            pin->ChangeValue(i.value());
        ++i;
    }
    listChanged.clear();
}

void MidiToAutomation::MidiMsgFromInput(long msg)
{
    int command = Pm_MessageStatus(msg) & MidiConst::codeMask;

    switch(command) {
        case MidiConst::ctrl: {
            ChangeValue(Pm_MessageData1(msg),Pm_MessageData2(msg));
            break;
        }
        case MidiConst::prog : {
            ChangeValue(para_prog, Pm_MessageData1(msg) );
            break;
        }
        case MidiConst::noteOn : {
            ChangeValue(para_velocity, Pm_MessageData2(msg) );
            ChangeValue(para_notes+Pm_MessageData1(msg), Pm_MessageData2(msg) );
            break;
        }
        case MidiConst::noteOff : {
//            ChangeValue(para_notepitch, Pm_MessageData2(msg) );
            ChangeValue(para_notes+Pm_MessageData1(msg), Pm_MessageData2(msg) );
            break;
        }
        case MidiConst::pitchbend : {
            ChangeValue(para_pitchbend, Pm_MessageData2(msg) );
            break;
        }
        case MidiConst::chanpressure : {
            ChangeValue(para_chanpress, Pm_MessageData1(msg) );
            break;
        }
        case MidiConst::aftertouch : {
            ChangeValue(para_velocity, Pm_MessageData1(msg) );
            ChangeValue(para_aftertouch, Pm_MessageData2(msg) );
        }
    }
}

void MidiToAutomation::ChangeValue(int ctrl, int value) {

    if(value>127 || value<0) {
        debug("midi ctrl 0>127")
        return;
    }

    listChanged.insert(ctrl,value);

    if(!listParameterPinOut->listPins.contains(ctrl))
        listParameterPinOut->AsyncAddPin(ctrl);
}

Pin* MidiToAutomation::CreatePin(const ConnectionInfo &info, quint16 nb)
{
    Pin *newPin = Object::CreatePin(info,nb);
    if(newPin)
        return newPin;

    switch(info.direction) {
        case PinDirection::Output :
            if(nb<128)
                newPin = new ParameterPinOut(this,nb,0,&listValues,true,QString("CC%1").arg(nb),false);
            if(nb==para_prog)
                newPin = new ParameterPinOut(this,nb,0,&listValues,true,"prog",false);
            if(nb==para_velocity)
                newPin = new ParameterPinOut(this,nb,0,&listValues,true,"vel",false);
            if(nb==para_pitchbend)
                newPin = new ParameterPinOut(this,nb,0,&listValues,true,"p.bend",false);
            if(nb==para_chanpress)
                newPin = new ParameterPinOut(this,nb,0,&listValues,true,"pressr",false);
            if(nb==para_aftertouch)
                newPin = new ParameterPinOut(this,nb,0,&listValues,true,"aftr.t",false);
            if(nb>=para_notes)
                newPin = new ParameterPinOut(this,nb,0,&listValues,true,QString("note%1").arg(nb),false);
            break;

        default :
            debug("MidiToAutomation::CreatePin PinDirection")
            return 0;
            break;
    }

    if(newPin)
        newPin->SetContainerId(containerId);
    return newPin;
}
