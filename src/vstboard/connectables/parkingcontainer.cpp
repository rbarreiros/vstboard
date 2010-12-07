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

#include "parkingcontainer.h"
//#include "objectfactory.h"
#include "../mainhost.h"

using namespace Connectables;

ParkingContainer::ParkingContainer(MainHost *myHost,int index, const ObjectInfo &info) :
    Container(myHost,index, info)
{
}

ParkingContainer::~ParkingContainer()
{
    closed=true;

    listStaticObjects.clear();

    if(modelIndex.isValid())
        myHost->GetParkingModel()->removeRow(modelIndex.row());
}

void ParkingContainer::AddObject(QSharedPointer<Object> objPtr)
{
    if(closed)
        return;

    objPtr->SetContainerId(-1);
    objPtr->SetParkingIndex(modelIndex);
    listStaticObjects << objPtr.toWeakRef();
}

void ParkingContainer::RemoveObject(QSharedPointer<Object> objPtr)
{
    if(objPtr.isNull())
        return;

    listStaticObjects.removeAll(objPtr.toWeakRef());
}

void ParkingContainer::Clear()
{
    listStaticObjects.clear();
}

void ParkingContainer::SetParentModeIndex(const QModelIndex &parentIndex)
{
    QStandardItem *modelNode = new QStandardItem();
    modelNode->setData(QVariant::fromValue(objInfo), UserRoles::objInfo);
    modelNode->setData(index, UserRoles::value);
    modelNode->setData(objInfo.name, Qt::DisplayRole);
    myHost->GetParkingModel()->appendRow(modelNode);
    modelIndex=modelNode->index();

    foreach(QSharedPointer<Object>objPtr, listStaticObjects) {
        objPtr->SetContainerId(-1);
        objPtr->SetParkingIndex(modelIndex);
    }
}

QDataStream & ParkingContainer::toStream (QDataStream &out)
{
    switch(myHost->filePass) {

        //save all the parked objects
        case 0:
        {
            foreach(QWeakPointer<Object> objPtr, listStaticObjects) {
                if(objPtr.isNull())
                    listStaticObjects.removeAll(objPtr);
            }

            out << (quint16)listStaticObjects.size();
            foreach(QSharedPointer<Object> objPtr, listStaticObjects) {
                if(!objPtr.isNull()) {

                    QByteArray tmpStream;
                    QDataStream tmp( &tmpStream , QIODevice::ReadWrite);
                    tmp << *objPtr.data();

                    out << objPtr->info();
                    out << (quint16)tmpStream.size();
                    out << tmpStream;
                } else {
                    debug("ParkingContainer::toStream pass1 skip deleted object")
                    out<<(ObjectInfo)ObjectInfo();
                }
            }
            break;
        }

        //do nothing : no program to save
//        case 1:
//            break;
//        case 2:
//            break;
    }
    return out;
}

QDataStream & ParkingContainer::fromStream (QDataStream &in)
{
    switch(myHost->filePass) {

        //load all the parked objects
        case 0:
        {
            quint16 nbObj;
            in >> nbObj;
            for(quint16 i=0; i<nbObj; i++) {
                ObjectInfo info;
                quint16 streamSize;
                QByteArray tmpStream;
                QDataStream tmp( &tmpStream , QIODevice::ReadWrite);

                in >> info;
                in >> streamSize;
                in >> tmpStream;

                if(info.objType != ObjType::dummy) {

                    QSharedPointer<Object> objPtr = myHost->objFactory->NewObject(info);
                    if(!objPtr.isNull()) {
                        AddObject(objPtr);
                        tmp >> *objPtr.data();

                        //keep the object alive while loading
                        listLoadingObjects << objPtr;

                    } else {
                        //error while creating the object, build a dummy object with the same saved id
                        info.objType=ObjType::dummy;
                        objPtr = myHost->objFactory->NewObject(info);
                        QDataStream tmp2( &tmpStream , QIODevice::ReadWrite);
                        if(!objPtr.isNull()) {
                            AddObject(objPtr);
                            tmp2 >> *objPtr.data();

                            //keep the object alive while loading
                            listLoadingObjects << objPtr;

                        } else {
                            //can't even create a dummy object ?
                            debug("Container::fromStream dummy object not created")
                        }
                    }
                }
            }
            break;
        }

        //do nothing : no program to load
//        case 1:
//            break;

        //clear the loading list, delete unused objects
        case 2:
            listLoadingObjects.clear();
            break;
    }
    return in;
}

QDataStream & operator<< (QDataStream & out, Connectables::ParkingContainer& value)
{
    return value.toStream(out);
}

QDataStream & operator>> (QDataStream & in, Connectables::ParkingContainer& value)
{
    return value.fromStream(in);
}
