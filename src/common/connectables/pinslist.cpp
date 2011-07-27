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
#include "audiopin.h"
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
//    connect(this, SIGNAL(NbPinChanged(int)),
//            this,SLOT(SetNbPins(int)));
}

void PinsList::SetContainerId(quint16 id)
{
    connInfo.container=id;
    foreach(Pin* pin, listPins) {
        pin->SetContainerId(id);
    }
}

void PinsList::Hide()
{
    foreach(Pin* pin, listPins) {
        pin->Close();
    }
}

void PinsList::ChangeNumberOfPins(int newNb)
{
    emit SetNbPins(newNb);
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
            LOG("pin not in list"<<pinNumber);
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
        LOG("pin not found"<<pinNumber);
        return 0;
    }

    return static_cast<AudioPin*>(listPins.value(pinNumber))->GetBuffer();
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

void PinsList::SetNbPins(int nb)
{
   QMap<quint16,Pin*>::iterator i = listPins.end();
    while(listPins.size()>nb && !listPins.empty()) {
        if(i.key()<FIXED_PIN_STARTINDEX) {
            RemovePin(i.key());
            i=listPins.erase(i);
        }
        --i;
    }

    int cpt=0;
    while(cpt<nb) {
        if(!listPins.contains(cpt))
            AddPin(cpt);
        cpt++;
    }
}

Pin * PinsList::AddPin(int nb)
{
    if(listPins.contains(nb))
        return listPins.value(nb);

    connInfo.pinNumber=nb;
    Pin *newPin = parent->CreatePin(connInfo);

    if(!newPin) {
        LOG("pin not created"<<nb);
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
            LOG("pin not created"<<id);
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
