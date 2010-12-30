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

HostController::HostController(MainHost *myHost,int index):
    Object(myHost,index, ObjectInfo(NodeType::object, ObjType::HostController, tr("HostController") ) ),
    tempoChanged(false),
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

    int tempo=120;
    int sign1=4;
    int sign2=4;
    myHost->GetTempo(tempo,sign1,sign2);

    listParameterPinIn->listPins.insert(Param_Tempo, new ParameterPinIn(this,Param_Tempo,tempo,&listTempo,true,"bpm"));
    listParameterPinIn->listPins.insert(Param_Sign1, new ParameterPinIn(this,Param_Sign1,sign1,&listSign1,true,"sign1"));
    listParameterPinIn->listPins.insert(Param_Sign2, new ParameterPinIn(this,Param_Sign2,sign2,&listSign2,true,"sign2"));
    listParameterPinIn->listPins.insert(Param_Group, new ParameterPinIn(this,Param_Group, myHost->programList->GetCurrentMidiGroup(),&listGrp,true,"Group"));
    listParameterPinIn->listPins.insert(Param_Prog, new ParameterPinIn(this,Param_Prog, myHost->programList->GetCurrentMidiProg(),&listPrg,true,"Prog"));

    connect(this, SIGNAL(progChange(int)),
            myHost->programList,SLOT(ChangeProg(int)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(grpChange(int)),
            myHost->programList,SLOT(ChangeGroup(int)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(tempoChange(int,int,int)),
            myHost,SLOT(SetTempo(int,int,int)),
            Qt::QueuedConnection);

    connect(myHost->programList,SIGNAL(ProgChanged(QModelIndex)),
           this,SLOT(OnHostProgChanged(QModelIndex)));
    connect(myHost->programList,SIGNAL(GroupChanged(QModelIndex)),
           this,SLOT(OnHostGroupChanged(QModelIndex)));
}

void HostController::Render()
{
    if(tempoChanged) {
        tempoChanged=false;

        int tempo = static_cast<ParameterPin*>(listParameterPinIn->listPins.value(Param_Tempo))->GetVariantValue().toInt();
        int sign1 = static_cast<ParameterPin*>(listParameterPinIn->listPins.value(Param_Sign1))->GetVariantValue().toInt();
        int sign2 = static_cast<ParameterPin*>(listParameterPinIn->listPins.value(Param_Sign2))->GetVariantValue().toInt();

        emit tempoChange(tempo,sign1,sign2);
    }

    if(progChanged) {
        progChanged=false;
        emit progChange( static_cast<ParameterPin*>(listParameterPinIn->listPins.value(Param_Prog))->GetVariantValue().toInt() );
    }
    if(grpChanged) {
        grpChanged=false;
        emit grpChange( static_cast<ParameterPin*>(listParameterPinIn->listPins.value(Param_Group))->GetVariantValue().toInt() );
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

void HostController::OnHostProgChanged(const QModelIndex &index)
{
    static_cast<ParameterPin*>(listParameterPinIn->listPins.value(Param_Prog))->ChangeValue( index.row(), true );
}

void HostController::OnHostGroupChanged(const QModelIndex &index)
{
    static_cast<ParameterPin*>(listParameterPinIn->listPins.value(Param_Group))->ChangeValue( index.row(), true );
}
