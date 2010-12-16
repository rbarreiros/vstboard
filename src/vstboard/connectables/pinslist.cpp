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

#include "pinslist.h"
#include "pin.h"
#include "audiobuffer.h"
#include "audiopinin.h"
#include "audiopinout.h"
#include "midipinin.h"
#include "midipinout.h"
#include "bridgepinin.h"
#include "bridgepinout.h"
#include "mainhost.h"

using namespace Connectables;

PinsList::PinsList(MainHost *myHost, QObject *parent) :
        QObject(parent),
        myHost(myHost)
{
    connect(this,SIGNAL(PinAdded(int,QString,QVariant,bool,QList<QVariant>*,bool)),
            this,SLOT(AsyncAddPin(int,QString,QVariant,bool,QList<QVariant>*,bool)));
}

void PinsList::SetContainerId(quint16 id)
{
    containerId=id;
    foreach(Pin* pin, listPins) {
        pin->SetContainerId(containerId);
    }
}

void PinsList::Hide()
{
    foreach(Pin* pin, listPins) {
        pin->Close();
    }
}

bool PinsList::ChangeNumberOfPins(int newNbIn)
{
    int nbIn=listPins.count();

    if(newNbIn==nbIn)
        return true;

    if(newNbIn<nbIn) {
        QMap<quint16,Pin*>::Iterator i = listPins.end();
        for(int nb=newNbIn; nb<nbIn; nb++) {
            i=listPins.erase(i);
        }
    }
    if(newNbIn>nbIn){
        Pin *newPin=0;

        int bufferSize = myHost->GetBufferSize();
        for(int nb=nbIn;nb<newNbIn;nb++) {
            if(connInfo.direction==PinDirection::Input) {
                switch(connInfo.type) {
                    case PinType::Audio :
                        newPin = new AudioPinIn(parent,nb,connInfo.bridge);
                        static_cast<AudioPinIn*>(newPin)->buffer->SetSize(bufferSize);
                        break;
                    case PinType::Midi :
                        newPin = new MidiPinIn(parent,nb,connInfo.bridge);
                        break;
                    case PinType::Bridge :
                        newPin = new BridgePinIn(parent,nb,connInfo.bridge);
                        break;
                    case PinType::Parameter :
                        newPin = new ParameterPinIn(parent,nb,0,true);
                        break;
                    case PinType::ND :
                        debug("PinsList::ChangeNumberOfPins PinType::ND")
                        return false;
                }
            } else {
                switch(connInfo.type) {
                    case PinType::Audio :
                        newPin = new AudioPinOut(parent,nb,connInfo.bridge);
                        static_cast<AudioPinOut*>(newPin)->buffer->SetSize(bufferSize);
                        break;
                    case PinType::Midi :
                        newPin = new MidiPinOut(parent,nb,connInfo.bridge);
                        break;
                    case PinType::Bridge :
                        newPin = new BridgePinOut(parent,nb,connInfo.bridge);
                        break;
                    case PinType::Parameter :
                        newPin = new ParameterPinOut(parent,nb,0,true);
                        break;
                    case PinType::ND :
                        debug("PinsList::ChangeNumberOfPins PinType::ND")
                        return false;
                }
            }

            if(!newPin) {
                debug("PinsList::ChangeNumberOfPins pin not created")
                return false;
            }

            newPin->SetContainerId(containerId);
            listPins.insert(nb, newPin);
        }
    }

    return true;
}

void PinsList::SetInfo(Object *parent,const ConnectionInfo &connInfo, const ObjectInfo &objInfo)
{
    this->parent=parent;
    this->connInfo=connInfo;
    this->objInfo=objInfo;
}

void PinsList::SetVisible(bool visible) {
    foreach(Pin* pin, listPins) {
        pin->SetVisible(visible);
    }
}

void PinsList::SetBridge(bool bridge)
{
    foreach(Pin* pin, listPins) {
        pin->SetBridge(bridge);
    }
}

Pin * PinsList::GetPin(int pinNumber, bool autoCreate)
{
    //resize the list if needed
    if(!listPins.contains(pinNumber)) {
        if(autoCreate) {
            debug("PinsList::GetPin can't autocreate")
            ChangeNumberOfPins(pinNumber+1);
        } else {
            debug("PinsList::GetPin pin not in list")
            return 0;
        }
    }

    return listPins.value(pinNumber);
}

AudioBuffer *PinsList::GetBuffer(int pinNumber)
{
    if(connInfo.type != PinType::Audio)
        return 0;

    if(connInfo.direction == PinDirection::Input)
        return static_cast<AudioPinIn*>(listPins.value(pinNumber))->buffer;
    if(connInfo.direction == PinDirection::Output)
        return static_cast<AudioPinOut*>(listPins.value(pinNumber))->buffer;

    return 0;
}

void PinsList::ConnectAllTo(PinsList *other, bool hidden)
{
    QSharedPointer<Object>cntPtr = myHost->objFactory->GetObj(modelList.parent().parent());

    QMap<quint16,Pin*>::Iterator i = listPins.begin();
    while(i!=listPins.end()) {
        Pin *otherPin = other->listPins.value(i.key(),0);
        if(otherPin)
            static_cast<Container*>(cntPtr.data())->AddCable(i.value()->GetConnectionInfo(),otherPin->GetConnectionInfo(),hidden);
        ++i;
    }
}



void PinsList::UpdateModelNode(QStandardItem *parentNode)
{
    if(!modelList.isValid() && parentNode) {
        QStandardItem *item = new QStandardItem("lstPins");
        item->setData( QVariant::fromValue(objInfo) , UserRoles::objInfo);
        parentNode->appendRow(item);
        modelList=item->index();
    }

    if(!modelList.isValid())
        return;

    foreach(Pin* pin, listPins) {
        pin->SetParentModelIndex(modelList);
    }
}

void PinsList::AddPin(int nb, const QString &name, QVariant value, bool nameCanChange, QList<QVariant> *listValues, bool defaultVisible)
{
    if(listPins.contains(nb))
        return;
    emit PinAdded(nb,name,value,nameCanChange,listValues,defaultVisible);
}

void PinsList::AsyncAddPin(int nb, const QString &name, QVariant value, bool nameCanChange, QList<QVariant> *listValues, bool defaultVisible)
{
    if(listPins.contains(nb))
        return;

    Pin *newPin=0;
    int bufferSize = myHost->GetBufferSize();

    if(connInfo.direction==PinDirection::Input) {
        switch(connInfo.type) {
            case PinType::Audio :
                newPin = new AudioPinIn(parent,nb,connInfo.bridge);
                static_cast<AudioPinIn*>(newPin)->buffer->SetSize(bufferSize);
                break;
            case PinType::Midi :
                newPin = new MidiPinIn(parent,nb,connInfo.bridge);
                break;
            case PinType::Bridge :
                newPin = new BridgePinIn(parent,nb,connInfo.bridge);
                break;
            case PinType::Parameter :
                if(listValues)
                    newPin = new ParameterPinIn(parent,nb,value,listValues,defaultVisible,name,nameCanChange);
                else
                    newPin = new ParameterPinIn(parent,nb,value.toFloat(),defaultVisible,name,nameCanChange);
                break;
            case PinType::ND :
                debug("PinsList::AsyncAddPin PinType::ND")
                return;
        }
    } else {
        switch(connInfo.type) {
            case PinType::Audio :
                newPin = new AudioPinOut(parent,nb,connInfo.bridge);
                static_cast<AudioPinOut*>(newPin)->buffer->SetSize(bufferSize);
                break;
            case PinType::Midi :
                newPin = new MidiPinOut(parent,nb,connInfo.bridge);
                break;
            case PinType::Bridge :
                newPin = new BridgePinOut(parent,nb,connInfo.bridge);
                break;
            case PinType::Parameter :
                if(listValues)
                    newPin = new ParameterPinOut(parent,nb,value,listValues,defaultVisible,name,nameCanChange);
                else
                    newPin = new ParameterPinOut(parent,nb,value.toFloat(),defaultVisible,name,nameCanChange);
                break;
            case PinType::ND :
                debug("PinsList::AsyncAddPin PinType::ND")
                return;
        }
    }

    if(!newPin) {
        debug("PinsList::AddPin pin not created")
        return;
    }

    newPin->SetContainerId(containerId);
    listPins.insert(nb, newPin);

    if(modelList.isValid())
        newPin->SetParentModelIndex(modelList);
}

QDataStream & PinsList::toStream(QDataStream & out) const
{

}

QDataStream & PinsList::fromStream(QDataStream & in)
{

}

QDataStream & operator<< (QDataStream & out, const PinsList::Object& value)
{
    return value.toStream(out);
}

QDataStream & operator>> (QDataStream & in, PinsList::Object& value)
{
    return value.fromStream(in);
}
