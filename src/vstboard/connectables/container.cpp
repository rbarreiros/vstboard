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

#include "container.h"
#include "objectfactory.h"
#include "mainhost.h"

using namespace Connectables;

Container::Container(int index, const ObjectInfo &info) :
        Object(index, info ),
    bridgeIn(0),
    bridgeOut(0),
    currentProgram(0),
    cablesNode(0)
{
    LoadProgram(TEMP_PROGRAM);
}

Container::~Container()
{
    Close();
}

void Container::SetParentModelNode(QStandardItem* parent)
{
    if(modelNode && modelNode->parent() == parent)
        return;

    Object::SetParentModelNode(parent);

    if(currentProgram)
        foreach(QSharedPointer<Object>objPtr, currentProgram->listObjects) {
            objPtr->SetParentModelNode(modelNode);
        }

    foreach(QSharedPointer<Object>objPtr, listStaticObjects) {
        objPtr->SetParentModelNode(modelNode);
    }

    cablesNode = new QStandardItem("cables");
    modelNode->appendRow(cablesNode);
}

void Container::UpdateModelNode()
{
    if(!modelNode)
        return;

    Object::UpdateModelNode();
}

void Container::ConnectBridges(QSharedPointer<Object> bridgeA, QSharedPointer<Object> bridgeB)
{
    if(bridgeA.isNull() || bridgeB.isNull())
        return;

    for(int i=0; i<bridgeB->GetListBridgePinIn().size(); i++) {
        AddCable(bridgeA->GetListBridgePinOut().at(i)->GetConnectionInfo(), bridgeB->GetListBridgePinIn().at(i)->GetConnectionInfo(), true);
    }
}

bool Container::Close()
{
    if(closed)
        return false;

    foreach(QSharedPointer<Object> objPtr, listStaticObjects) {
        if(objPtr.isNull())
            continue;
        RemoveObject(objPtr);
    }
    listStaticObjects.clear();

    if(currentProgram) {
        currentProgram->Unload();
        delete currentProgram;
        currentProgram=0;
    }

    foreach(ContainerProgram *prog, listContainerPrograms) {
        delete prog;
    }
    listContainerPrograms.clear();

    bridgeIn.clear();
    bridgeOut.clear();

    Object::Close();

    return true;
}

void Container::SetSleep(bool sleeping)
{
    Object::SetSleep(sleeping);

    if(!currentProgram)
        return;

    foreach(QSharedPointer<Object> objPtr, currentProgram->listObjects) {
        if(!objPtr.isNull())
            objPtr->SetSleep(sleep);
    }
}

void Container::Hide()
{
    cablesNode=0;

    if(currentProgram) {
        foreach(QSharedPointer<Object> objPtr, currentProgram->listObjects) {
            if(!objPtr.isNull())
                objPtr->Hide();
        }
    }
    foreach(QSharedPointer<Object> objPtr, listStaticObjects) {
        if(!objPtr.isNull())
            objPtr->Hide();
    }

    Object::Hide();

}

void Container::LoadProgram(int prog)
{
    //if prog is already loaded, update model
    if(prog==currentProgId && currentProgram) {
        UpdateModelNode();
        return;
    }

    //if a program is loaded, unload it without saving
    int progWas = currentProgId;
    if(currentProgId!=EMPTY_PROGRAM && currentProgram)
        UnloadProgram();

    currentProgId=prog;

    if(!listContainerPrograms.contains(currentProgId))
        listContainerPrograms.insert(currentProgId,new ContainerProgram(this));

    ContainerProgram *tmp = listContainerPrograms.value(currentProgId);
    currentProgram = new ContainerProgram(*tmp);
    currentProgram->Load(prog);
    UpdateModelNode();

    //if the loaded program was a temporary prog, delete it
    if(progWas==TEMP_PROGRAM)
        listContainerPrograms.remove(TEMP_PROGRAM);
}

void Container::SaveProgram()
{
    if(!currentProgram)
        return;

    currentProgram->Save();
    delete listContainerPrograms.take(currentProgId);
    listContainerPrograms.insert(currentProgId,currentProgram);
    ContainerProgram *tmp = new ContainerProgram(*currentProgram);
    currentProgram = tmp;
}

void Container::UnloadProgram()
{
    if(!currentProgram)
        return;

    currentProgram->Unload();
    delete currentProgram;
    currentProgram=0;

    currentProgId=EMPTY_PROGRAM;
}

void Container::CopyProgram(int ori, int dest)
{
    if(!listContainerPrograms.contains(ori)) {
        //not important : the program is empty and will be created when needed
        //debug("Container::CopyProgram ori not found")
        return;
    }
    if(listContainerPrograms.contains(dest)) {
        debug("Container::CopyProgram dest already exists")
        return;
    }

    ContainerProgram* progOri = listContainerPrograms.value(ori);
    ContainerProgram* progCpy = progOri->Copy(ori,dest);
    listContainerPrograms.insert(dest,progCpy);
}

void Container::RemoveProgram(int prg)
{
    if(!listContainerPrograms.contains(prg)) {
//        debug("Container::RemoveProgram not found")
        return;
    }
    ContainerProgram* prog = listContainerPrograms.value(prg);
    prog->Remove(prg);
    listContainerPrograms.remove(prg);
}

void Container::AddObject(QSharedPointer<Object> objPtr)
{
    //bridges are not stored in program
    if(objPtr->info().nodeType==NodeType::bridge ) {
        if(objPtr->info().objType==ObjType::BridgeIn) {
            bridgeIn=objPtr;
        }
        if(objPtr->info().objType==ObjType::BridgeOut) {
            bridgeOut=objPtr;
        }

        objPtr->listenProgramChanges=false;
        listStaticObjects << objPtr;
        AddChildObject(objPtr);
        objPtr->LoadProgram(currentProgId);
        return;
    }

    currentProgram->AddObject(objPtr);
    objPtr->LoadProgram(currentProgId);
}

void Container::RemoveObject(QSharedPointer<Object> objPtr)
{
    //containers not listening to program changes : delete the object from the only program
    if(!listenProgramChanges) {
        foreach(ContainerProgram *prg, listContainerPrograms) {
            prg->RemoveObject(objPtr);
        }
    }

    if(currentProgram)
        currentProgram->RemoveObject(objPtr);

    if(objPtr==bridgeIn)
        bridgeIn.clear();
    if(objPtr==bridgeOut)
        bridgeOut.clear();

    listStaticObjects.removeAll(objPtr);
}

void Container::RemoveCable(QModelIndex & index)
{
    currentProgram->RemoveCable(index);
}

void Container::AddChildObject(QSharedPointer<Object> objPtr)
{
    objPtr->SetParentModelNode(modelNode);
    MainHost::Get()->OnObjectAdded(objPtr);
}

void Container::RemoveChildObject(QSharedPointer<Object> objPtr)
{
    if(objPtr.isNull())
        return;

    objPtr->Hide();
    MainHost::Get()->OnObjectRemoved(objPtr, this);
}


void Container::AddCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin, bool hidden)
{
    if(!currentProgram)
        return;
    currentProgram->AddCable(outputPin,inputPin, hidden);
}


void Container::RemoveCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
{
    if(!currentProgram)
        return;
    currentProgram->RemoveCable(outputPin,inputPin);
}

void Container::RemoveCableFromPin(const ConnectionInfo &pin)
{
    if(!currentProgram)
        return;
    currentProgram->RemoveCableFromPin(pin);
}

void Container::RemoveCableFromObj(int objId)
{
    if(!currentProgram)
        return;
    currentProgram->RemoveCableFromObj(objId);
}

QDataStream & Container::toStream (QDataStream& out) const
{
    switch(MainHost::Get()->filePass) {
        case 0:
        {
            out << (qint16)index;
            out << objectName();
            out << sleep;

            if(currentProgram) {
                out << (quint16)currentProgram->listObjects.size();
                foreach(QSharedPointer<Object> objPtr, currentProgram->listObjects) {
                    if(!objPtr.isNull()) {

                        QByteArray tmpStream;
                        QDataStream tmp( &tmpStream , QIODevice::ReadWrite);
                        tmp << *objPtr.data();

                        out << objPtr->info();
                        out << (qint16)objPtr->GetIndex();
                        out << (quint16)tmpStream.size();
                        out << tmpStream;
                    } else {
                        out<<(quint8)ObjType::ND;
                    }
                }
            } else {
                out << (quint16)0;
            }
            break;
        }

        case 1:
        {

            out << (quint32)listContainerPrograms.size();
            QHash<int,ContainerProgram*>::const_iterator i = listContainerPrograms.constBegin();
            while(i!=listContainerPrograms.constEnd()) {
                out << (quint32)i.key();
                out << *(i.value());
                ++i;
            }

            out << (quint32)currentProgId;

            if(currentProgram) {
                out << (quint16)currentProgram->listObjects.size();
                foreach(QSharedPointer<Object> objPtr, currentProgram->listObjects) {
                    if(!objPtr.isNull()) {
                        out<<(quint16)objPtr->GetIndex();
                        out<<*objPtr.data();
                    } else {
                        out<<(quint16)-1;
                    }
                }
            } else {
                out << (quint16)0;
            }
            break;
        }
    }

    return out;

}

QDataStream & Container::fromStream (QDataStream& in)
{
    switch(MainHost::Get()->filePass) {
        case 0:
        {
            qint16 id;
            in >> id;
            savedIndex = id;
            QString name;
            in >> name;
            setObjectName(name);
            in >> sleep;

            quint16 nbObj;
            in >> nbObj;
            for(quint16 i=0; i<nbObj; i++) {

                    ObjectInfo info;
                    qint16 savedIndex;
                    quint16 size;
                    QByteArray tmpStream;
                    QDataStream tmp( &tmpStream , QIODevice::ReadWrite);

                    in >> info;
                    in >> savedIndex;
                    in >> size;
                    in >> tmpStream;

                    QSharedPointer<Object> objPtr = ObjectFactory::Get()->NewObject(info);
                    if(!objPtr.isNull()) {
                        AddObject(objPtr);
                        tmp >> *objPtr.data();
                    } else {
                        //error while creating the object, build a dummy object with the same saved id
                        info.objType=ObjType::dummy;
                        objPtr = ObjectFactory::Get()->NewObject(info);
                        QDataStream tmp2( &tmpStream , QIODevice::ReadWrite);
                        if(!objPtr.isNull()) {
                            AddObject(objPtr);
                            tmp2 >> *objPtr.data();
                        } else {
                            //can't even create a dummy object ?
                            debug("Container::fromStream dummy object not created")
                        }
                    }
            }
            break;
        }

        case 1:
        {

            quint32 nbProg;
            in >> nbProg;
            for(quint32 i=0; i<nbProg; i++) {
                quint32 progId;
                in >> progId;

                ContainerProgram *prog=0;
                if(listContainerPrograms.contains(progId)) {
                    prog = listContainerPrograms.value(progId);
                } else {
                    prog = new ContainerProgram(this);
                    listContainerPrograms.insert(progId,prog);
                }

                in >> *prog;

            }

            quint32 progId;
            in >> progId;

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
