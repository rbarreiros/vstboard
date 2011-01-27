/**************************************************************************
#    Copyright 2010-2011 Raphaël François
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
    connect(this,SIGNAL(PinRemoved(int)),
            this,SLOT(RemovePin(int)));
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
        //must start from zero
        for(int nb=0;nb<newNb;nb++) {
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
            AddPin(pinNumber);
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

    if(!listPins.contains(pinNumber)) {
        debug2(<<"PinsList::GetBuffer"<< pinNumber <<"not found")
        return 0;
    }

    if(connInfo.direction == PinDirection::Input)
        return static_cast<AudioPinIn*>(listPins.value(pinNumber))->buffer;
    if(connInfo.direction == PinDirection::Output)
        return static_cast<AudioPinOut*>(listPins.value(pinNumber))->buffer;

    return 0;
}

AudioBufferD *PinsList::GetBufferD(int pinNumber)
{
    if(connInfo.type != PinType::Audio)
        return 0;

    if(!listPins.contains(pinNumber)) {
        debug2(<<"PinsList::GetBuffer"<< pinNumber <<"not found")
        return 0;
    }

    if(connInfo.direction == PinDirection::Input)
        return static_cast<AudioPinIn*>(listPins.value(pinNumber))->bufferD;
    if(connInfo.direction == PinDirection::Output)
        return static_cast<AudioPinOut*>(listPins.value(pinNumber))->bufferD;

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

void PinsList::AsyncRemovePin(int nb)
{
    if(!listPins.contains(nb))
        return;
    emit PinRemoved(nb);
}

Pin * PinsList::AddPin(int nb)
{
    if(listPins.contains(nb))
        return listPins.value(nb);

    connInfo.pinNumber=nb;
    Pin *newPin = parent->CreatePin(connInfo);

    if(!newPin) {
        debug("PinsList::AddPin pin not created")
        return 0;
    }

    listPins.insert(nb, newPin);

    if(modelList.isValid())
        newPin->SetParentModelIndex(modelList);

    parent->OnProgramDirty();
    return newPin;
}

void PinsList::RemovePin(int nb)
{
    if(!listPins.contains(nb))
        return;

    parent->OnProgramDirty();
    delete listPins.take(nb);
}

QDataStream & PinsList::toStream(QDataStream & out) const
{
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
    in >> connInfo;
    in >> objInfo;

    quint16 nbPins;
    in >> nbPins;

    for(quint16 i=0; i<nbPins; i++) {
        quint16 id;
        in >> id;
        QVariant value;
        in >> value;
        connInfo.pinNumber=id;
        Pin *newPin = parent->CreatePin(connInfo);
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
