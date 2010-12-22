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

PinsList::PinsList(MainHost *myHost, Object *parent) :
        QObject(parent),
        parent(parent),
        myHost(myHost)
{
    connect(this,SIGNAL(PinAdded(int)),
            this,SLOT(AddPin(int)));
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

bool PinsList::ChangeNumberOfPins(int newNb)
{
    int nbPins=listPins.count();

    if(newNb==nbPins)
        return true;

    if(newNb<nbPins) {
        QMap<quint16,Pin*>::Iterator i = listPins.end();
        for(int nb=newNb; nb<nbPins; nb++) {
            i=listPins.erase(i);
        }
    }
    if(newNb>nbPins){
        for(int nb=nbPins;nb<newNb;nb++) {
            AsyncAddPin(nb);
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

void PinsList::ConnectAllTo(Container* container, PinsList *other, bool hidden)
{
    QSharedPointer<Object>cntPtr = myHost->objFactory->GetObj(modelList.parent().parent());

    QMap<quint16,Pin*>::Iterator i = listPins.begin();
    while(i!=listPins.end()) {
        Pin *otherPin = other->listPins.value(i.key(),0);
        if(otherPin)
            container->AddCable(i.value()->GetConnectionInfo(),otherPin->GetConnectionInfo(),hidden);
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

void PinsList::AsyncAddPin(int nb)
{
    if(listPins.contains(nb))
        return;
    emit PinAdded(nb);
}

Pin * PinsList::AddPin(int nb)
{
    if(listPins.contains(nb))
        return listPins.value(nb);

    Pin *newPin = parent->CreatePin(connInfo,nb);

    if(!newPin) {
        debug("PinsList::AddPin pin not created")
        return 0;
    }

    listPins.insert(nb, newPin);

    if(modelList.isValid())
        newPin->SetParentModelIndex(modelList);

    return newPin;
}

QDataStream & PinsList::toStream(QDataStream & out) const
{
    const quint16 file_version = 1;
    out << file_version;

    out << connInfo;
    out << objInfo;

    out << (quint16)listPins.count();

    QMap<quint16,Pin*>::ConstIterator i = listPins.constBegin();
    while(i!=listPins.constEnd()) {
        Pin *pin=i.value();
        out << i.key();
        out << pin->GetValue();
        ++i;
    }

    return out;
}

QDataStream & PinsList::fromStream(QDataStream & in)
{
    quint16 file_version;
    in >> file_version;

    in >> connInfo;
    in >> objInfo;

    quint16 nbPins;
    in >> nbPins;

    for(quint16 i=0; i<nbPins; i++) {
        quint16 id;
        in >> id;
        QVariant value;
        in >> value;
        Pin *newPin = parent->CreatePin(connInfo,id);
        if(!newPin) {
            debug("PinsList::fromStream pin not created")
            return in;
        }
        listPins.insert(id,newPin);

    }

    return in;
}

QDataStream & operator<< (QDataStream & out, const Connectables::PinsList& value)
{
    return value.toStream(out);
}

QDataStream & operator>> (QDataStream & in, Connectables::PinsList& value)
{
    return value.fromStream(in);
}