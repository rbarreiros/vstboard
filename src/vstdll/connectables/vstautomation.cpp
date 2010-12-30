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
    for(int i=0; i<VST_AUTOMATION_DEFAULT_NB_PINS; i++) {
        listParameterPinIn->AddPin(i);
        listParameterPinOut->AddPin(i);
    }

    for(int i=0;i<128;i++) {
        listValues << i;
    }

    ParameterPinIn *nbPin = new ParameterPinIn(this,FixedPinNumber::numberOfPins,VST_AUTOMATION_DEFAULT_NB_PINS,&listValues,true,tr("NbPins"));
    nbPin->SetAlwaysVisible(true);
    nbPin->SetLimitsEnabled(false);
    listParameterPinIn->listPins.insert(FixedPinNumber::numberOfPins, nbPin);

    static_cast<ParameterPinIn*>(listParameterPinIn->listPins.value(FixedPinNumber::learningMode))->SetAlwaysVisible(true);
}

VstAutomation::~VstAutomation()
{
    Close();
}


void VstAutomation::Render()
{
//    QMutexLocker l(&objMutex);

    if(!listChanged.isEmpty()) {
        Vst *vst=myHost->myVstPlugin;
        QHash<int,float>::const_iterator i = listChanged.constBegin();
        while(i!=listChanged.constEnd()) {
            if(listParameterPinOut->listPins.contains(i.key())) {
                static_cast<ParameterPin*>( listParameterPinOut->listPins.value(i.key()) )->ChangeValue(i.value());
            }
            vst->beginEdit(i.key());
            vst->setParameterAutomated(i.key(),i.value());
            vst->endEdit(i.key());
            ++i;
        }
        listChanged.clear();
    }
}

void VstAutomation::ValueFromHost(int pinNum, float value)
{
    switch(GetLearningMode()) {
        case LearningMode::unlearn :
            listParameterPinOut->AsyncRemovePin(pinNum);
            listParameterPinIn->AsyncRemovePin(pinNum);
            break;
        case LearningMode::learn :
            listParameterPinOut->AsyncAddPin(pinNum);
            listParameterPinIn->AsyncAddPin(pinNum);
        case LearningMode::off :
            listChanged.insert(pinNum,value);
            break;
    }
}

void VstAutomation::OnParameterChanged(ConnectionInfo pinInfo, float value)
{
    Object::OnParameterChanged(pinInfo,value);
    if(pinInfo.pinNumber==FixedPinNumber::numberOfPins) {
        int nbPins=static_cast<ParameterPin*>(listParameterPinIn->listPins.value(FixedPinNumber::numberOfPins) )->GetIndex();

        if(nbPins>numberOfPins) {
            for(int i=numberOfPins; i<nbPins; i++) {
                listParameterPinIn->AddPin(i);
                listParameterPinOut->AddPin(i);
            }
        }

        if(nbPins<numberOfPins) {
            for(int i=nbPins; i<numberOfPins; i++) {
                listParameterPinIn->RemovePin(i);
                listParameterPinOut->RemovePin(i);
            }
        }

        numberOfPins=nbPins;
        return;
    }

    if(pinInfo.direction==PinDirection::Input && pinInfo.pinNumber<200)
        listChanged.insert(pinInfo.pinNumber,value);
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

Pin* VstAutomation::CreatePin(const ConnectionInfo &info)
{
    Pin *newPin = Object::CreatePin(info);
    if(newPin)
        return newPin;

    if(info.type!=PinType::Parameter) {
        debug("VstAutomation::CreatePin PinType")
        return 0;
    }

    switch(info.direction) {
        case PinDirection::Output :
            newPin = new ParameterPinOut(this,info.pinNumber,0,true,QString("autom%1").arg(info.pinNumber),false);
            break;
        case PinDirection::Input :
            newPin = new ParameterPinIn(this,info.pinNumber,0,true,QString("autom%1").arg(info.pinNumber),false);
            break;

        default :
            debug("VstAutomation::CreatePin PinDirection")
            return 0;
            break;
    }
    return newPin;
}
