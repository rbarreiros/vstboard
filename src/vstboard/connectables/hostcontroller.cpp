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

#include "hostcontroller.h"
#include "../globals.h"
#include "../mainhost.h"
#include "../projectfile/projectfile.h"

using namespace Connectables;

HostController::HostController(int index):
    Object(index, ObjectInfo(NodeType::object, ObjType::HostController, tr("HostController") ) ),
    tempoChanged(false),
    midiProgChanged(false),
    prog(0),
    progChanged(false),
    grpChanged(false)
{

        for(int i=1;i<300;i++) {
            listTempo << i;
        }

        for(int i=1;i<33;i++) {
            listSign1 << i;
        }

        for(int i=0;i<8;i++) {
            listSign2 << (1<<i);
        }
        for(int i=0;i<128;i++) {
            listPrg << i;
        }
        for(int i=0;i<128;i++) {
            listGrp << i;
        }

    listMidiPinIn << new MidiPinIn(this);

    listParameterPinIn.insert(Param_Tempo, new ParameterPinIn(this,Param_Tempo,QVariant::fromValue<int>(120),&listTempo,true,"bpm"));
    listParameterPinIn.insert(Param_Sign1, new ParameterPinIn(this,Param_Sign1,4,&listSign1,true,"sign1"));
    listParameterPinIn.insert(Param_Sign2, new ParameterPinIn(this,Param_Sign2,4,&listSign2,true,"sign2"));
    listParameterPinIn.insert(Param_Group, new ParameterPinIn(this,Param_Group,0,&listGrp,true,"ProgGrp"));
    listParameterPinIn.insert(Param_Prog, new ParameterPinIn(this,Param_Prog,0,&listPrg,true,"Prog"));

    connect(this, SIGNAL(progChange(int)),
            MainHost::Get()->programList,SLOT(ChangeProg(int)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(grpChange(int)),
            MainHost::Get()->programList,SLOT(ChangeGroup(int)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(tempoChange(int,int,int)),
            MainHost::Get(),SLOT(SetTempo(int,int,int)),
            Qt::QueuedConnection);
}

void HostController::Render()
{
    if(tempoChanged) {
        tempoChanged=false;

        int tempo = listParameterPinIn.value(Param_Tempo)->GetVariantValue().toInt();
        int sign1 = listParameterPinIn.value(Param_Sign1)->GetVariantValue().toInt();
        int sign2 = listParameterPinIn.value(Param_Sign2)->GetVariantValue().toInt();

        emit tempoChange(tempo,sign1,sign2);
    }

    if(midiProgChanged) {
        midiProgChanged=false;
        emit progChange(prog);
    }

    if(progChanged) {
        progChanged=false;
        emit progChange( listParameterPinIn.value(Param_Prog)->GetVariantValue().toInt() );
    }
    if(grpChanged) {
        grpChanged=false;
        emit grpChange( listParameterPinIn.value(Param_Group)->GetVariantValue().toInt() );
    }
}

void HostController::MidiMsgFromInput(long msg)
{
    int command = Pm_MessageStatus(msg) & MidiConst::codeMask;
    if (command == MidiConst::prog) {
        prog = Pm_MessageData1(msg);
        midiProgChanged=true;
    }
}

void HostController::OnParameterChanged(ConnectionInfo pinInfo, float value)
{
    Object::OnParameterChanged(pinInfo,value);

    switch(pinInfo.pinNumber) {
        case Param_Tempo :
        case Param_Sign1 :
        case Param_Sign2 :
            tempoChanged=true;
            break;
        case Param_Group :
            grpChanged=true;
            break;
        case Param_Prog :
            progChanged=true;
            break;
    }
}
