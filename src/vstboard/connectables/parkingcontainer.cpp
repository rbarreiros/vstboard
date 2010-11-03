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

#include "parkingcontainer.h"
#include "objectfactory.h"
#include "../mainhost.h"

using namespace Connectables;

ParkingContainer::ParkingContainer() :
    filePass(0),
    modelNode(0),
    closing(false)
{
}

ParkingContainer::~ParkingContainer()
{
    closing=true;

    foreach(QSharedPointer<Object> objPtr, listObj) {
        RemoveObject(objPtr);
    }

    if(modelNode)
        modelNode->model()->invisibleRootItem()->removeRow(modelNode->row());
}

void ParkingContainer::AddObject(QSharedPointer<Object> &objPtr)
{
    if(closing)
        return;

    listObj << objPtr;
    objPtr->SetParkingNode(modelNode);
}

void ParkingContainer::RemoveObject(QSharedPointer<Object> &objPtr)
{
    if(objPtr.isNull())
        return;

    listObj.removeAll(objPtr);
}

void ParkingContainer::SetParentModelNode(QStandardItem* parent)
{
    modelNode = new QStandardItem();
    modelNode->setData("parking",Qt::DisplayRole);
    parent->appendRow(modelNode);

    foreach(QSharedPointer<Object>objPtr, listObj) {
        objPtr->SetParkingNode(modelNode);
    }
}

QDataStream & ParkingContainer::toStream (QDataStream &out) const
{
    switch(MainHost::Get()->filePass) {
        case 0:
        {
            out << (quint16)listObj.size();
            foreach(QSharedPointer<Object> objPtr, listObj) {
                if(!objPtr.isNull()) {
                    out<<objPtr->info();
                    out<<*objPtr.data();
                } else {
                    out<<(quint8)ObjType::ND;
                }
            }
            break;
        }
        case 1:
        {
            out << (quint16)listObj.size();
            foreach(QSharedPointer<Object> objPtr, listObj) {
                if(!objPtr.isNull()) {
                    out<<(quint16)objPtr->GetIndex();
                    out<<*objPtr.data();
                } else {
                    out<<(quint16)-1;
                }
            }
            break;
        }
    }
    return out;
}

QDataStream & ParkingContainer::fromStream (QDataStream &in)
{
    switch(MainHost::Get()->filePass) {
        case 0:
        {
            quint16 nbObj;
            in >> nbObj;
            for(quint16 i=0; i<nbObj; i++) {
                quint8 objType;
                in>>objType;
                if( (ObjType::Enum)objType!=ObjType::ND ) {
                    ObjectInfo info;
                    in>>info;

                    QSharedPointer<Object> objPtr = ObjectFactory::Get()->NewObject(info);
                    if(!objPtr.isNull()) {
                        AddObject(objPtr);
                        in>>*objPtr.data();
                   }
                }
            }
            break;
        }
        case 1:
        {
            quint16 nbObj;
            in >> nbObj;
            for(quint16 i=0; i<nbObj; i++) {
                quint16 objId;
                in>>objId;
                if(objId!=(quint16)-1) {
                    int id = ObjectFactory::Get()->IdFromSavedId(objId);
                    QSharedPointer<Object> objPtr = ObjectFactory::Get()->GetObjectFromId(id);
                    if(!objPtr.isNull()) {
                        in>>*objPtr.data();
                   }
                }
            }
            break;
        }
    }
    return in;
}

QDataStream & operator<< (QDataStream & out, const Connectables::ParkingContainer& value)
{
    return value.toStream(out);
}

QDataStream & operator>> (QDataStream & in, Connectables::ParkingContainer& value)
{
    return value.fromStream(in);
}
