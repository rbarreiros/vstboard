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


#include "containerprogram.h"
#include "mainhost.h"
#include "container.h"
#include "object.h"
#include "renderer/renderernode.h"

using namespace Connectables;

QTime ContainerProgram::unsavedTime;

ContainerProgram::ContainerProgram(MainHost *myHost,Container *container) :
        container(container),
        dirty(false),
        myHost(myHost)
{
}

ContainerProgram::ContainerProgram(const ContainerProgram& c)
{
    myHost = c.myHost;
    container = c.container;
    dirty=false;

    foreach(QSharedPointer<Object> objPtr, c.listObjects) {
        listObjects << objPtr;
    }

    foreach(Cable* cab, c.listCables) {
        listCables << new Cable(*cab);
    }

    QMap<int,ObjectContainerAttribs>::ConstIterator i = c.mapObjAttribs.constBegin();
    while(i!=c.mapObjAttribs.constEnd()) {
        mapObjAttribs.insert(i.key(),i.value());
        ++i;
    }

    foreach(RendererNode *node, c.listOfRendererNodes) {
        listOfRendererNodes << new RendererNode(*node);
    }
    timeSavedRendererNodes = c.timeSavedRendererNodes;
}

ContainerProgram::~ContainerProgram()
{
    foreach(Cable *c, listCables) {
        delete c;
    }
    listCables.clear();

    listObjects.clear();
    mapObjAttribs.clear();

    foreach(RendererNode *node, listOfRendererNodes) {
        delete node;
    }
}

ContainerProgram * ContainerProgram::Copy(int fromId, int toId)
{
    foreach(QSharedPointer<Object> objPtr, listObjects) {
        objPtr->CopyProgram(fromId,toId);
    }
    return new ContainerProgram(*this);
}

ContainerProgram * ContainerProgram::CopyTo(int toId)
{
    foreach(QSharedPointer<Object> objPtr, listObjects) {
        objPtr->CopyCurrentProgram(toId);
    }
    ContainerProgram *newPrg = new ContainerProgram(*this);
    newPrg->Save(false);
    return newPrg;
}

void ContainerProgram::Remove(int prgId)
{
    foreach(QSharedPointer<Object> objPtr, listObjects) {
        objPtr->RemoveProgram(prgId);
    }
}

bool ContainerProgram::PinExistAndVisible(const ConnectionInfo &info)
{
    if(info.type==PinType::Bridge)
        return true;

    Pin* pin=myHost->objFactory->GetPin(info);
    if(!pin)
        return false;
    if(!pin->GetVisible())
        return false;
    return true;
}

void ContainerProgram::Load(int progId)
{
    foreach(QSharedPointer<Object> objPtr, listObjects) {
        if(objPtr) {
//            container->AddChildObject(objPtr);
            objPtr->LoadProgram(progId);
        }
    }

    foreach(Cable *cab, listCables) {

        //check if the pin exists,
        //if the object is in error mode, dummy pins will be created
        if(PinExistAndVisible(cab->GetInfoOut()) &&
           PinExistAndVisible(cab->GetInfoIn())) {
            cab->AddToParentNode(container->GetCablesIndex());
            myHost->OnCableAdded(cab);
        } else {
            //delete cable from program if pins are not found and can't be created
            listCables.removeAll(cab);
        }
    }

    QMap<int,ObjectContainerAttribs>::Iterator i = mapObjAttribs.begin();
    while(i!=mapObjAttribs.end()) {
        QSharedPointer<Object> obj = myHost->objFactory->GetObjectFromId(i.key());
        if(!obj.isNull()) {
            obj->SetContainerAttribs(i.value());
        } else {
            //delete attrib if object not found
            i=mapObjAttribs.erase(i);
        }
        ++i;
    }

    dirty=false;
}

void ContainerProgram::Unload()
{
    foreach(Cable *cab, listCables) {
        myHost->OnCableRemoved(cab);
        cab->RemoveFromParentNode(container->GetCablesIndex());
    }

    foreach(QSharedPointer<Object> obj, listObjects) {
        if(!obj.isNull())
            obj->UnloadProgram();
    }
}

void ContainerProgram::SaveRendererState()
{
    timeSavedRendererNodes = QTime::currentTime();
    foreach(RendererNode *node, listOfRendererNodes)
        delete node;
    listOfRendererNodes = myHost->GetRenderer()->SaveNodes();
}

void ContainerProgram::LoadRendererState()
{
    //if(container->parentContainer) {
        const QTime t = container->GetLastUpdate();
        if(t > timeSavedRendererNodes) {
            //my renderer map is outdated
            myHost->SetSolverUpdateNeeded();
        } else {
            myHost->GetRenderer()->LoadNodes( listOfRendererNodes );
        }
    //}
}


void ContainerProgram::ParkAllObj()
{
    foreach(QSharedPointer<Object> obj, listObjects) {
        container->ParkChildObject(obj);
    }
}

bool ContainerProgram::IsDirty()
{
    if(dirty)
        return true;

    foreach(QSharedPointer<Object> obj, listObjects) {
        if(!obj.isNull()) {
            if(obj->IsDirty())
                return true;

            ObjectContainerAttribs attr;
            obj->GetContainerAttribs(attr);
            if(attr != mapObjAttribs.value(obj->GetIndex()))
                return true;
        }
    }
    return false;
}

void ContainerProgram::SetDirty()
{
    dirty=true;
}

void ContainerProgram::Save(bool saveChildPrograms)
{

    if(saveChildPrograms) {
        foreach(QSharedPointer<Object> objPtr, listObjects) {
            objPtr->SaveProgram();
        }
    }

    mapObjAttribs.clear();
    foreach(QSharedPointer<Object> obj, listObjects) {
        if(!obj.isNull()) {
            ObjectContainerAttribs attr;
            obj->GetContainerAttribs(attr);
            mapObjAttribs.insert(obj->GetIndex(),attr);
        }
    }

    foreach(QSharedPointer<Object> obj, container->listStaticObjects) {
        if(!obj.isNull() ) {
            //don't save bridges position
            if(obj->info().nodeType==NodeType::bridge) {
                continue;
            }

            ObjectContainerAttribs attr;
            obj->GetContainerAttribs(attr);
            mapObjAttribs.insert(obj->GetIndex(),attr);
        }
    }
    dirty=false;
}

void ContainerProgram::AddObject(QSharedPointer<Object> objPtr)
{
    listObjects << objPtr;
    container->AddChildObject(objPtr);
    dirty=true;
}

void ContainerProgram::RemoveObject(QSharedPointer<Object> objPtr)
{
    RemoveCableFromObj(objPtr->GetIndex());
    listObjects.removeAll(objPtr);
    container->ParkChildObject(objPtr);
    dirty=true;
}

void ContainerProgram::ReplaceObject(QSharedPointer<Object> newObjPtr, QSharedPointer<Object> replacedObjPtr)
{
    //AddObject(newObjPtr);
    CopyCablesFromObj( newObjPtr->GetIndex(), replacedObjPtr->GetIndex() );
    //RemoveObject(replacedObjPtr);
}

bool ContainerProgram::AddCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin, bool hidden)
{
    if(CableExists(outputPin,inputPin))
        return true;

    if(!outputPin.CanConnectTo(inputPin))
        return false;

    //check if the pin exists,
    //if the object is in error mode, dummy pins will be created
    if(!PinExistAndVisible(inputPin) || !PinExistAndVisible(outputPin))
        return false;

    Cable *cab = new Cable(myHost,outputPin,inputPin);
    listCables << cab;

    if(!hidden && container)
        cab->AddToParentNode(container->GetCablesIndex());

    myHost->OnCableAdded(cab);
    dirty=true;
    return true;
}

void ContainerProgram::RemoveCable(Cable *cab)
{
    listCables.removeAll(cab);
    cab->RemoveFromParentNode(container->GetCablesIndex());
    myHost->OnCableRemoved(cab);
    delete cab;
    dirty=true;
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
            RemoveCable(cab);
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
            RemoveCable(cab);
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
            RemoveCable(cab);
        }
        --i;
    }
}

void ContainerProgram::CreateBridgeOverObj(int objId)
{
    int i=listCables.size()-1;
    while(i>=0) {
        Cable *cab = listCables.at(i);
        if(cab->GetInfoOut().objId==objId || cab->GetInfoIn().objId==objId ||
           cab->GetInfoOut().container==objId || cab->GetInfoIn().container==objId) {

            //for all output cables
            if(cab->GetInfoOut().objId==objId && cab->GetInfoOut().type!=PinType::Parameter ) {
                int j=listCables.size()-1;
                while(j>=0) {
                    Cable *otherCab = listCables.at(j);
                    ConnectionInfo otherPin( cab->GetInfoOut() );
                    otherPin.direction=PinDirection::Input;

                    //find corresponding input cables
                    if(otherCab->GetInfoIn()==otherPin) {
                        //create a bridge
                        AddCable(otherCab->GetInfoOut(), cab->GetInfoIn());
                    }
                    --j;
                }
            }

            //for all input cables
            if(cab->GetInfoIn().objId==objId && cab->GetInfoIn().type!=PinType::Parameter ) {
                int j=listCables.size()-1;
                while(j>=0) {
                    Cable *otherCab = listCables.at(j);
                    ConnectionInfo otherPin( cab->GetInfoOut() );
                    otherPin.direction=PinDirection::Output;

                    //find corresponding output cables
                    if(otherCab->GetInfoOut()==otherPin) {
                        //create a bridge
                        AddCable(cab->GetInfoOut(), otherCab->GetInfoIn() );
                    }
                    --j;
                }
            }
        }
        --i;
    }
}

void ContainerProgram::CopyCablesFromObj(int newObjId, int oldObjId)
{
    int i=listCables.size()-1;
    while(i>=0) {
        Cable *cab = listCables.at(i);
        if(cab->GetInfoOut().objId==oldObjId) {
            ConnectionInfo newConnect = cab->GetInfoOut();
            newConnect.objId = newObjId;
            AddCable(newConnect, cab->GetInfoIn());
        }
        if(cab->GetInfoIn().objId==oldObjId) {
            ConnectionInfo newConnect = cab->GetInfoIn();
            newConnect.objId = newObjId;
            AddCable(cab->GetInfoOut(), newConnect);
        }
        --i;
    }
}

void ContainerProgram::MoveOutputCablesFromObj(int newObjId, int oldObjId)
{
    int i=listCables.size()-1;
    while(i>=0) {
        Cable *cab = listCables.at(i);
        if(cab->GetInfoOut().objId==oldObjId && cab->GetInfoOut().type!=PinType::Parameter) {
            ConnectionInfo newConnect = cab->GetInfoOut();
            newConnect.objId = newObjId;
            if( AddCable(newConnect, cab->GetInfoIn()) ) {
                RemoveCable(cab);
            }
        }
        --i;
    }
}

void ContainerProgram::MoveInputCablesFromObj(int newObjId, int oldObjId)
{
    int i=listCables.size()-1;
    while(i>=0) {
        Cable *cab = listCables.at(i);
        if(cab->GetInfoIn().objId==oldObjId && cab->GetInfoIn().type!=PinType::Parameter) {
            ConnectionInfo newConnect = cab->GetInfoIn();
            newConnect.objId = newObjId;
            if( AddCable(cab->GetInfoOut(), newConnect) ) {
                RemoveCable(cab);
            }
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
    QMap<int,ObjectContainerAttribs>::ConstIterator i = mapObjAttribs.constBegin();
    while(i!=mapObjAttribs.constEnd()) {
        out << i.key();
        out << i.value();
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
        id = myHost->objFactory->IdFromSavedId(id);
        listObjects << myHost->objFactory->GetObjectFromId(id);
    }

    quint16 nbCables;
    in >> nbCables;
    for(quint16 i=0; i<nbCables; i++) {
        ConnectionInfo infoOut;
        infoOut.myHost = myHost;
        in >> infoOut;

        ConnectionInfo infoIn;
        infoIn.myHost = myHost;
        in >> infoIn;

        //check if this pin exists
        //myHost->objFactory->GetPin(infoOut);
        //myHost->objFactory->GetPin(infoIn);

        Cable *cab = new Cable(myHost,infoOut,infoIn);
        listCables << cab;
    }

    quint16 nbPos;
    in >> nbPos;
    for(quint16 i=0; i<nbPos; i++) {
        int objId;
        ObjectContainerAttribs attr;
        in >> objId;
        in >> attr;
        objId=myHost->objFactory->IdFromSavedId(objId);
        mapObjAttribs.insert(objId,attr);
    }

    dirty=false;
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


