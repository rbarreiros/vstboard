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

#include "objectfactory.h"
#ifndef VST_PLUGIN
    #include "audiodevicein.h"
    #include "audiodeviceout.h"
#else
    #include "connectables/vstaudiodevicein.h"
    #include "connectables/vstaudiodeviceout.h"
    #include "connectables/vstautomation.h"
#endif
#include "mididevice.h"
#include "midisender.h"
#include "miditoautomation.h"
#include "hostcontroller.h"
#include "container.h"
#include "maincontainer.h"
#include "parkingcontainer.h"
#include "bridge.h"
#include "mainhost.h"

#ifdef VSTSDK
    #include "vstplugin.h"
    #include "../vst/cvsthost.h"
#endif

using namespace Connectables;

//ObjectFactory *ObjectFactory::theObjFactory=0;

//ObjectFactory * ObjectFactory::Create(QObject *parent)
//{
//    if(!theObjFactory)
//        theObjFactory = new ObjectFactory(parent);

//    return theObjFactory;
//}

ObjectFactory::ObjectFactory(MainHost *myHost) :
    QObject(myHost),
    cptListObjects(50),
    myHost(myHost)
{

}

ObjectFactory::~ObjectFactory()
{
    Clear();
}

void ObjectFactory::Clear()
{
    cptListObjects=0;
    listObjects.clear();
}

QSharedPointer<Object> ObjectFactory::GetObjectFromId(int id)
{
    if(!listObjects.contains(id)) {
        debug("ObjectFactory::GetObjectFromId : object not found %d",id)
        return QSharedPointer<Object>();
    }

    return listObjects.value(id);
}

void ObjectFactory::ResetSavedId()
{
    hashObjects::iterator i = listObjects.begin();
    while(i != listObjects.end()) {
        QSharedPointer<Object> objPtr = i.value().toStrongRef();
        if(objPtr.isNull()) {
            i=listObjects.erase(i);
        } else {
            //don't reset forcced ids
            if(i.key()>=50) {
                objPtr->ResetSavedIndex();
            }
            ++i;
        }
    }
}

int ObjectFactory::IdFromSavedId(int savedId)
{
    hashObjects::const_iterator i = listObjects.constBegin();
    while(i != listObjects.constEnd()) {
        QSharedPointer<Object> objPtr = i.value().toStrongRef();
        if(objPtr && objPtr->GetSavedIndex()==savedId) {
            return i.key();
        }
        ++i;
    }
    debug("object factory idfromsavedid: id not found %d",savedId)
    return -1;
}

Pin *ObjectFactory::GetPin(const QModelIndex & index)
{
    ConnectionInfo info = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();
    return GetPin(info);
}

Pin *ObjectFactory::GetPin(const ConnectionInfo &pinInfo)
{
    if(!listObjects.contains(pinInfo.objId)) {
        debug("objectfactory getpin : obj not found")
        return 0;
    }

    QSharedPointer<Object> objPtr = listObjects.value(pinInfo.objId).toStrongRef();
    if(objPtr)
        return objPtr->GetPin(pinInfo);

    return 0;
}

void ObjectFactory::RemoveObject(int id)
{
    if(!listObjects.contains(id)) {
        debug(QString("ObjectFactory::RemoveObject object %1 already deleted").arg(id).toAscii())
        return;
    }

     QSharedPointer<Object> objPtr = listObjects.take(id);
     if(objPtr.isNull())
         return;
}

QSharedPointer<Object> ObjectFactory::GetObj(const QModelIndex & index)
{
    //the object is not created, do it
    if(!index.data(UserRoles::value).isValid()) {
        return NewObject( index.data(UserRoles::objInfo).value<ObjectInfo>() );
    }

    //or return the existing object
    return GetObjectFromId(index.data(UserRoles::value).toInt());
}

QSharedPointer<Object> ObjectFactory::NewObject(const ObjectInfo &info)
{
    int objId = cptListObjects;
    if(info.forcedObjId) {
        objId = info.forcedObjId;
    }

    Object *obj=0;

    switch(info.nodeType) {

        case NodeType::container :
            switch(info.objType) {
                case ObjType::Container:
                    obj = new Container(myHost,objId, info);
                    break;

                case ObjType::MainContainer:
                    obj = new MainContainer(myHost,objId, info);
                    break;

                case ObjType::ParkingContainer:
                    obj = new ParkingContainer(myHost,objId, info);
                    break;

                default :
                    debug("ObjectFactory::NewObject : unknown object type")
                    return QSharedPointer<Object>();
            }
            break;

        case NodeType::bridge :
            obj = new Bridge(myHost,objId, info);
            break;

        case NodeType::object :

            switch(info.objType) {
#ifndef VST_PLUGIN
                case ObjType::AudioInterfaceIn:
                    obj = new AudioDeviceIn(myHost,objId, info);
                    break;

                case ObjType::AudioInterfaceOut:
                    obj = new AudioDeviceOut(myHost,objId, info);
                    break;
#else
            case ObjType::AudioInterfaceIn:
                obj = new VstAudioDeviceIn(myHost,objId, info);
                break;

            case ObjType::AudioInterfaceOut:
                obj = new VstAudioDeviceOut(myHost,objId, info);
                break;

            case ObjType::VstAutomation:
                obj = new VstAutomation(myHost,objId);
                break;
#endif
                case ObjType::MidiInterface:
                    obj = new MidiDevice(myHost,objId, info);
                    break;

                case ObjType::MidiSender:
                    obj = new MidiSender(myHost,objId);
                    break;

                case ObjType::MidiToAutomation:
                    obj = new MidiToAutomation(myHost,objId);
                    break;

                case ObjType::HostController:
                    obj = new HostController(myHost,objId);
                    break;

        #ifdef VSTSDK
                case ObjType::VstPlugin:
                    obj = new VstPlugin(myHost,objId, info);
                    break;
        #endif

                case ObjType::dummy :
                    obj = new Object(myHost, objId, info);
                    break;

                default:
                    debug("ObjectFactory::NewObject : unknown object type")
                    return QSharedPointer<Object>();
            }
            break;


        default :
            debug("ObjectFactory::NewObject unknown nodeType")
            return QSharedPointer<Object>();
    }



    QSharedPointer<Object> sharedObj(obj);
    listObjects.insert(objId,sharedObj.toWeakRef());

    if(!obj->Open()) {
        listObjects.remove(objId);
        sharedObj.clear();
        return QSharedPointer<Object>();
    }

    if(info.forcedObjId) {
        obj->ResetSavedIndex(info.forcedObjId);
    } else {
        cptListObjects++;
    }

    return sharedObj;

}
