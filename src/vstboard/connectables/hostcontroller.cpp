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

#include "hostcontroller.h"
#include "../globals.h"
#include "../mainhost.h"
#include "../projectfile/projectfile.h"

using namespace Connectables;

//QList<QVariant> HostController::listTempo;
//QList<QVariant> HostController::listSign1;
//QList<QVariant> HostController::listSign2;

HostController::HostController(int index):
    Object(index, ObjectInfo(NodeType::object, ObjType::HostController,"HostController" ) ),
    tempoChanged(false),
    progChanged(false),
    prog(0)
{
//    if(listTempo.isEmpty()) {
        for(int i=1;i<300;i++) {
            listTempo << i;
        }
//    }

//    if(listSign1.isEmpty()) {
        for(int i=1;i<33;i++) {
            listSign1 << i;
        }
//    }

//    if(listSign2.isEmpty()) {
        for(int i=0;i<8;i++) {
            listSign2 << (1<<i);
        }
//    }


    listMidiPinIn << new MidiPinIn(this);

    listParameterPinIn.insert(Param_Tempo, new ParameterPinIn(this,Param_Tempo,120,&listTempo,true,"bpm"));
    listParameterPinIn.insert(Param_Sign1, new ParameterPinIn(this,Param_Sign1,4,&listSign1,true,"sign1"));
    listParameterPinIn.insert(Param_Sign2, new ParameterPinIn(this,Param_Sign2,4,&listSign2,true,"sign2"));

    connect(this, SIGNAL(progChange(int)),
            MainHost::Get(),SLOT(SetProgram(int)),
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
//        MainHost::Get()->SetTempo(tempo,sign1,sign2);
    }

    if(progChanged) {
        progChanged=false;

        Project::Program *prg =  Project::ProjectFile::theProjectFile->GetProgram(prog);
        if(!prg) {
            debug("HostController 65 : prog not found %d",prog)
            return;
        }

        emit progChange(prg->progIndex);
//        MainHost::Get()->SetProgram(prg->progIndex);
    }
}

void HostController::MidiMsgFromInput(long msg)
{
    int command = Pm_MessageStatus(msg) & MidiConst::codeMask;
    if (command == MidiConst::prog) {
        prog = Pm_MessageData1(msg);
        progChanged=true;
    }
}

void HostController::OnParameterChanged(ConnectionInfo pinInfo, float value)
{
    tempoChanged=true;
}
