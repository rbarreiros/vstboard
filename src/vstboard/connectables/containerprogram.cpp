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

#include "containerprogram.h"
#include "mainhost.h"
#include "container.h"
#include "parkingcontainer.h"
#include "object.h"

using namespace Connectables;

ContainerProgram::ContainerProgram(Container *container) :
        container(container)
{
}

ContainerProgram::ContainerProgram(const ContainerProgram& c)
{
    container = c.container;

    foreach(QSharedPointer<Object> objPtr, c.listObjects) {
        listObjects << objPtr;
    }

    foreach(Cable* cab, c.listCables) {
        listCables << new Cable(*cab);
    }

    QMap<int,objAttirbs>::ConstIterator i = c.mapObjAttribs.constBegin();
    while(i!=c.mapObjAttribs.constEnd()) {
        mapObjAttribs.insert(i.key(),i.value());
        ++i;
    }
}

ContainerProgram::~ContainerProgram()
{
    foreach(Cable *c, listCables) {
        delete c;
    }
    listCables.clear();

    listObjects.clear();
    mapObjAttribs.clear();
}

void ContainerProgram::Load(int progId)
{
    foreach(QSharedPointer<Object> objPtr, listObjects) {
        if(objPtr) {
            container->AddChildObject(objPtr);
            objPtr->LoadProgram(progId);
        }
    }

    foreach(Cable *cab, listCables) {

        if(ObjectFactory::Get()->GetObjectFromId(cab->GetInfoOut().objId) &&
           ObjectFactory::Get()->GetObjectFromId(cab->GetInfoIn().objId)) {

            cab->AddToParentNode(container->cablesNode);
            MainHost::Get()->OnCableAdded(cab->GetInfoOut(),cab->GetInfoIn());
        } else {
            //delete cable if objects are not found
            listCables.removeAll(cab);
        }
    }

    QMap<int,objAttirbs>::Iterator i = mapObjAttribs.begin();
    while(i!=mapObjAttribs.end()) {
        QSharedPointer<Object> obj = ObjectFactory::Get()->GetObjectFromId(i.key());
        if(!obj.isNull()) {
            obj->SetPosition(i.value().position);
            obj->SetSize(i.value().size);
            if(i.value().editor)
                obj->OpenEditor();
        } else {
            //delete attrib if object not found
            i=mapObjAttribs.erase(i);
        }
        ++i;
    }
}

void ContainerProgram::Unload()
{
    foreach(Cable *cab, listCables) {
        MainHost::Get()->OnCableRemoved(cab->GetInfoOut(),cab->GetInfoIn());
        cab->RemoveFromParentNode(container->cablesNode);
    }

    foreach(QSharedPointer<Object> obj, listObjects) {
        if(!obj.isNull())
            obj->UnloadProgram();
    }

    foreach(QSharedPointer<Object> obj, listObjects) {
        container->RemoveChildObject(obj);
    }
}

void ContainerProgram::Save()
{
    foreach(QSharedPointer<Object> objPtr, listObjects) {
        objPtr->SaveProgram();
    }

    mapObjAttribs.clear();
    foreach(QSharedPointer<Object> obj, listObjects) {
        if(!obj.isNull()) {
            objAttirbs attr;
            attr.position = obj->GetPosition();
            attr.size = obj->GetSize();
            attr.editor = obj->GetEditorVisible();
            mapObjAttribs.insert(obj->GetIndex(),attr);
        }
    }
    foreach(QSharedPointer<Object> obj, container->listStaticObjects) {
        if(!obj.isNull()) {
            objAttirbs attr;
            attr.position = obj->GetPosition();
            attr.size = obj->GetSize();
            attr.editor = obj->GetEditorVisible();
            mapObjAttribs.insert(obj->GetIndex(),attr);
        }
    }
}

void ContainerProgram::AddObject(QSharedPointer<Object> objPtr)
{
    listObjects << objPtr;
    container->AddChildObject(objPtr);
}

void ContainerProgram::RemoveObject(QSharedPointer<Object> objPtr)
{
    RemoveCableFromObj(objPtr->GetIndex());
    listObjects.removeAll(objPtr);
    container->RemoveChildObject(objPtr);
}

void ContainerProgram::AddCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin, bool hidden)
{
    if(CableExists(outputPin,inputPin))
        return;

    Cable *cab = new Cable(outputPin,inputPin);
    listCables << cab;

    if(!hidden)
        cab->AddToParentNode(container->cablesNode);

    MainHost::Get()->OnCableAdded(outputPin,inputPin);
}


void ContainerProgram::RemoveCable(const QModelIndex & index)
{
    ConnectionInfo outInfo = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();
    ConnectionInfo inInfo = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();
    RemoveCable(outInfo,inInfo);
}

void ContainerProgram::RemoveCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
{
    int i=listCables.size()-1;
    while(i>=0) {
        Cable *cab = listCables.at(i);
        if(cab->GetInfoOut()==outputPin && cab->GetInfoIn()==inputPin) {
            listCables.removeAt(i);
            cab->RemoveFromParentNode(container->cablesNode);
            MainHost::Get()->OnCableRemoved(outputPin,inputPin);
            delete cab;
            return;
        }
        --i;
    }
}

void ContainerProgram::RemoveCableFromPin(const ConnectionInfo &pin)
{
    int i=listCables.size()-1;
    while(i>=0) {
        Cable *cab = listCables.at(i);
        if(cab->GetInfoOut()==pin || cab->GetInfoIn()==pin) {
            listCables.removeAt(i);
            cab->RemoveFromParentNode(container->cablesNode);
            MainHost::Get()->OnCableRemoved(cab->GetInfoOut(),cab->GetInfoIn());
            delete cab;
        }
        --i;
    }
}

void ContainerProgram::RemoveCableFromObj(int objId)
{
    int i=listCables.size()-1;
    while(i>=0) {
        Cable *cab = listCables.at(i);
        if(cab->GetInfoOut().objId==objId || cab->GetInfoIn().objId==objId ||
           cab->GetInfoOut().container==objId || cab->GetInfoIn().container==objId) {
            listCables.removeAt(i);
            cab->RemoveFromParentNode(container->cablesNode);
            MainHost::Get()->OnCableRemoved(cab->GetInfoOut(),cab->GetInfoIn());
            delete cab;
        }
        --i;
    }
}

bool ContainerProgram::CableExists(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
{
    foreach(Cable *c, listCables) {
        if(c->GetInfoOut()==outputPin && c->GetInfoIn()==inputPin)
            return true;
    }
    return false;
}

QDataStream & ContainerProgram::toStream (QDataStream& out) const
{
    out << (quint16)listObjects.size();
    foreach(QSharedPointer<Object> objPtr, listObjects) {
        out << (qint16)objPtr->GetIndex();
    }

    out << (quint16)listCables.size();
    foreach(Cable *cab, listCables) {
        out << cab->GetInfoOut();
        out << cab->GetInfoIn();
    }

    out << (quint16)mapObjAttribs.size();
    QMap<int,objAttirbs>::ConstIterator i = mapObjAttribs.constBegin();
    while(i!=mapObjAttribs.constEnd()) {
        out << i.key();
        out << i.value().position;
        out << i.value().size;
        ++i;
    }
    return out;
}

QDataStream & ContainerProgram::fromStream (QDataStream& in)
{
    quint16 nbobj;
    in >> nbobj;
    for(quint16 i=0; i<nbobj; i++) {
        quint16 id;
        in >> id;
        id = ObjectFactory::Get()->IdFromSavedId(id);
        listObjects << ObjectFactory::Get()->GetObjectFromId(id);
    }

    quint16 nbCables;
    in >> nbCables;
    for(quint16 i=0; i<nbCables; i++) {
        ConnectionInfo infoOut;
        in >> infoOut;

        ConnectionInfo infoIn;
        in >> infoIn;

        //check if this pin exists
        ObjectFactory::Get()->GetPin(infoOut);
        ObjectFactory::Get()->GetPin(infoIn);

        Cable *cab = new Cable(infoOut,infoIn);
        listCables << cab;
    }

    quint16 nbPos;
    in >> nbPos;
    for(quint16 i=0; i<nbPos; i++) {
        int objId;
        objAttirbs attr;
        in >> objId;
        in >> attr.position;
        in >> attr.size;
        objId=ObjectFactory::Get()->IdFromSavedId(objId);
        mapObjAttribs.insert(objId,attr);
    }

    return in;
}

QDataStream & operator<< (QDataStream & out, const Connectables::ContainerProgram& value)
{
    return value.toStream(out);
}

QDataStream & operator>> (QDataStream & in, Connectables::ContainerProgram& value)
{
    return value.fromStream(in);
}
