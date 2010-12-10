/**************************************************************************
#    Copyright 2010 Rapha�l Fran�ois
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

#include "vstautomation.h"
#include "../vstboard/mainhost.h"
#include "../vst.h"

using namespace Connectables;

VstAutomation::VstAutomation(MainHost *myHost,int index) :
        Object(myHost,index, ObjectInfo(NodeType::object, ObjType::VstAutomation, tr("VstAutomation")) ),
        numberOfPins(VST_AUTOMATION_DEFAULT_NB_PINS)
{
    for(int i=0;i<128;i++) {
        listValues << i;
    }

    bool visible=true;
    for(int i=0;i<128;i++) {
        if(i==VST_AUTOMATION_DEFAULT_NB_PINS) visible=false;
        listParameterPinOut.insert(i,new ParameterPinOut(this,i,0,&listValues,visible,QString::number(i)));
        listParameterPinIn.insert(i,new ParameterPinIn(this,i,0,&listValues,visible,QString::number(i)));
    }

    ParameterPinIn *nbPin = new ParameterPinIn(this,FixedPinNumber::numberOfPins,VST_AUTOMATION_DEFAULT_NB_PINS,&listValues,true,tr("NbPins"));
    nbPin->SetAlwaysVisible(true);
    listParameterPinIn.insert(FixedPinNumber::numberOfPins, nbPin);

}

VstAutomation::~VstAutomation()
{
    Close();
}


void VstAutomation::Render()
{
//    QMutexLocker l(&objMutex);

    if(!listChangedByHost.isEmpty()) {
        QHash<int,float>::const_iterator i = listChangedByHost.constBegin();
        while(i!=listChangedByHost.constEnd()) {
            if(listParameterPinOut.contains(i.key())) {
                listParameterPinOut.value(i.key())->OnValueChanged(i.value());
            }
            ++i;
        }
        listChangedByHost.clear();
    }

    if(!listChangedByPin.isEmpty()) {
        Vst *vst=myHost->myVstPlugin;
        QHash<int,float>::const_iterator j = listChangedByPin.constBegin();
        while(j!=listChangedByPin.constEnd()) {
            if(listParameterPinIn.contains(j.key())) {
                vst->beginEdit(j.key());
                vst->setParameterAutomated(j.key(),j.value());
                vst->endEdit(j.key());
            }
            ++j;
        }
        listChangedByPin.clear();
    }
}

void VstAutomation::ValueFromHost(int pinNum, float value)
{
    listChangedByHost.insert(pinNum,value);
}

void VstAutomation::OnParameterChanged(ConnectionInfo pinInfo, float value)
{
    Object::OnParameterChanged(pinInfo,value);
    if(pinInfo.pinNumber==FixedPinNumber::numberOfPins) {
        OnNumberOfPinsChanged( listParameterPinIn.value(FixedPinNumber::numberOfPins)->GetIndex() );
        return;
    }

//    if(pinInfo.direction==PinDirection::Input)
        listChangedByPin.insert(pinInfo.pinNumber,value);
}

void VstAutomation::OnNumberOfPinsChanged(int newNb)
{
    if(numberOfPins==newNb)
        return;

    if(newNb<numberOfPins) {
        for(int i=newNb; i<numberOfPins; i++) {
            listParameterPinOut.value(i)->SetVisible(false);
            listParameterPinIn.value(i)->SetVisible(false);
        }
    }
    if(newNb>numberOfPins){
        for(int i=numberOfPins;i<newNb;i++) {
            listParameterPinOut.value(i)->SetVisible(true);
            listParameterPinIn.value(i)->SetVisible(true);
        }
    }
    numberOfPins=newNb;
}

bool VstAutomation::Close()
{
    myHost->myVstPlugin->removeVstAutomation(this);
    return Object::Close();
}

bool VstAutomation::Open()
{
    myHost->myVstPlugin->addVstAutomation(this);
    return Object::Open();
}
