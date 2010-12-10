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
        Object(myHost,index, ObjectInfo(NodeType::object, ObjType::MidiToAutomation, tr("MidiCC->Automation")) )
{

    for(int i=0;i<128;i++) {
        listValues << i;
    }

    listMidiPinIn << new MidiPinIn(this);

    for(int i=0;i<128;i++) {
        listParameterPinOut.insert(i,new ParameterPinOut(this,i,0,&listValues,false,QString::number(i)));
    }

    listParameterPinIn.value(FixedPinNumber::learningMode)->SetAlwaysVisible(true);
}


void MidiToAutomation::Render()
{
//    QMutexLocker l(&objMutex);

    if(listChanged.isEmpty())
        return;

    QHash<quint8,quint8>::const_iterator i = listChanged.constBegin();
    while(i!=listChanged.constEnd()) {
        if(listParameterPinOut.contains(i.key())) {
            listParameterPinOut.value(i.key())->ChangeValue(i.value());
        }
        ++i;
    }
    listChanged.clear();
}

void MidiToAutomation::MidiMsgFromInput(long msg)
{
    int command = Pm_MessageStatus(msg) & MidiConst::codeMask;
    if (command == MidiConst::ctrl) {
        int ctrl = Pm_MessageData1(msg);
        int val = Pm_MessageData2(msg);

        if(ctrl>127 || val>127 || ctrl<0 || val<0) {
            debug("midi ctrl 0>127")
            return;
        }
//        objMutex.lock();
        listChanged.insert(ctrl,val);
//        objMutex.unlock();
    }
}

