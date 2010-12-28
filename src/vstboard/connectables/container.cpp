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

#include "container.h"
#include "objectfactory.h"
#include "mainhost.h"

using namespace Connectables;

Container::Container(MainHost *myHost,int index, const ObjectInfo &info) :
    Object(myHost,index, info ),
    bridgeIn(0),
    bridgeOut(0),
    currentProgram(0),
    cablesNode(QModelIndex()),
    progToSet(-1)
{
    parkModel.setObjectName("parkModel"%objectName());
    LoadProgram(TEMP_PROGRAM);
    connect(myHost,SIGNAL(BufferSizeChanged(ulong)),
            this,SLOT(SetBufferSize(ulong)));
    connect(myHost,SIGNAL(SampleRateChanged(float)),
            this,SLOT(SetSampleRate(float)));
}

Container::~Container()
{
    Close();
}

const QModelIndex &Container::GetCablesIndex()
{
    if(cablesNode.isValid())
        return cablesNode;

    if(parked)
        return QModelIndex();

    if(modelIndex.isValid()) {
        QStandardItem *item = myHost->GetModel()->itemFromIndex(modelIndex);
        if(!item) {
            debug("Container::GetCablesIndex modelindex not found")
            return cablesNode;
        }
        QStandardItem *cab = new QStandardItem("cables");
        item->appendRow(cab);
        cablesNode = cab->index();
        return cablesNode;
    }
    return cablesNode;
}

void Container::SetContainerId(quint16 id)
{
    Object::SetContainerId(id);

    foreach(Object* obj, listLoadedObjects) {
        obj->SetContainerId(index);
    }
}

void Container::ConnectBridges(QSharedPointer<Object> bridgeA, QSharedPointer<Object> bridgeB, bool hidden)
{
    if(bridgeA.isNull() || bridgeB.isNull())
        return;

    bridgeA->GetListBridgePinOut()->ConnectAllTo(this,bridgeB->GetListBridgePinIn(), hidden);
}

bool Container::Close()
{
    if(closed)
        return false;

    foreach(QSharedPointer<Object> objPtr, listStaticObjects) {
        if(objPtr.isNull())
            continue;
        ParkObject(objPtr);
    }
    listStaticObjects.clear();

    if(currentProgram) {
        currentProgram->Unload();
        currentProgram->ParkAllObj();
        delete currentProgram;
        currentProgram=0;
    }

    foreach(ContainerProgram *prog, listContainerPrograms) {
        delete prog;
    }
    listContainerPrograms.clear();

    bridgeIn.clear();
    bridgeOut.clear();

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
    cablesNode=QModelIndex();

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

//void Container::SetProgramDirty()
//{
//    currentProgram->dirty=true;
//}

bool Container::IsDirty()
{
    return currentProgram->IsDirty();
}

void Container::SetProgram(const QModelIndex &prg)
{
    progToSet=prg.data(UserRoles::value).toInt();
}

void Container::Render()
{
    if(progToSet!=-1) {
        if(progToSet != currentProgId) {
            LoadProgram(progToSet);
        }
        progToSet=-1;
    }
}

void Container::SetBufferSize(unsigned long size)
{
    foreach(Object *obj, listLoadedObjects) {
        obj->SetBufferSize(size);
    }
}

void Container::SetSampleRate(float rate)
{
    foreach(Object *obj, listLoadedObjects) {
        obj->SetSampleRate(rate);
    }
}

void Container::LoadProgram(int prog)
{
    //if prog is already loaded, update model
    if(prog==currentProgId && currentProgram) {
        UpdateModelNode();
        return;
    }

    if(!listContainerPrograms.contains(prog))
        listContainerPrograms.insert(prog,new ContainerProgram(myHost,this));

    ContainerProgram *oldProg = currentProgram;
    ContainerProgram *newProg = listContainerPrograms.value(prog);


    if(oldProg) {
        //remove objects from the old program if not needed anymore
        foreach(QSharedPointer<Object>objPtr, oldProg->listObjects) {
            if(!newProg->listObjects.contains(objPtr)) {
                ParkChildObject(objPtr);
//                QTimer::singleShot(0, objPtr.data(), SLOT(OnHideEditor()));
                objPtr->OnHideEditor();
            }
        }

        //unload old prog
        if(currentProgId!=EMPTY_PROGRAM ) {
            oldProg->Unload();
        } else {
            oldProg = 0;
        }
    }

    currentProgId=prog;
    currentProgram = new ContainerProgram(*newProg);

    //add new objects
    foreach(QSharedPointer<Object>objPtr, newProg->listObjects) {
        if(!oldProg->listObjects.contains(objPtr)) {
            AddChildObject(objPtr);
        }
    }

    currentProgram->Load(prog);
    UpdateModelNode();

    if(oldProg)
        delete oldProg;

}

void Container::SaveProgram()
{
    if(!currentProgram && currentProgId==TEMP_PROGRAM)
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

    if(ori==currentProgId) {
        //copy the current program
        ContainerProgram* progCpy = currentProgram->CopyTo(dest);
        listContainerPrograms.insert(dest,progCpy);
    } else {
        //copy a saved program
        ContainerProgram* progOri = listContainerPrograms.value(ori);
        ContainerProgram* progCpy = progOri->Copy(ori,dest);
        listContainerPrograms.insert(dest,progCpy);
    }
}

void Container::RemoveProgram(int prg)
{
    if(!listContainerPrograms.contains(prg)) {
//        debug("Container::RemoveProgram not found")
        return;
    }
    ContainerProgram* prog = listContainerPrograms.take(prg);
    prog->Remove(prg);
    delete prog;
}

void Container::AddObject(QSharedPointer<Object> objPtr)
{
    objPtr->SetContainerId(index);

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

    if(!listLoadedObjects.contains(objPtr.data()))
        listLoadedObjects << objPtr.data();
    currentProgram->AddObject(objPtr);
    objPtr->LoadProgram(currentProgId);
}

void Container::AddParkedObject(QSharedPointer<Object> objPtr)
{
    objPtr->SetContainerId(index);

    listLoadedObjects << objPtr.data();
    ParkChildObject(objPtr);
}

void Container::ParkObject(QSharedPointer<Object> objPtr)
{
    objPtr->ToggleEditor(false);

    //this container is not programmable : delete the object from the saved program too
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

void Container::AddChildObject(QSharedPointer<Object> objPtr)
{
    if(objPtr->modelIndex.isValid() && objPtr->modelIndex.model()==&parkModel)
        parkModel.removeRow(objPtr->modelIndex.row());

    QStandardItem *item = objPtr->GetFullItem();
    myHost->GetModel()->itemFromIndex( modelIndex )->appendRow(item);
    objPtr->modelIndex=item->index();
    objPtr->parked=false;

#ifndef VST_PLUGIN
    if(objPtr->info().objType == ObjType::MidiInterface)
        myHost->midiDevices->OpenDevice(objPtr);
#endif

    myHost->SetSolverUpdateNeeded();
}

void Container::ParkChildObject(QSharedPointer<Object> objPtr)
{
    if(objPtr.isNull())
        return;

    if(objPtr->modelIndex.isValid() && objPtr->modelIndex.model()==myHost->GetModel())
        myHost->GetModel()->removeRow(objPtr->modelIndex.row(), objPtr->modelIndex.parent());

    QStandardItem *item = objPtr->GetParkingItem();
    parkModel.invisibleRootItem()->appendRow(item);
    objPtr->modelIndex=item->index();
    objPtr->parked=true;

#ifndef VST_PLUGIN
    if(objPtr->info().objType == ObjType::MidiInterface)
        myHost->midiDevices->CloseDevice(objPtr);
#endif

    myHost->SetSolverUpdateNeeded();
}


void Container::OnChildDeleted(Object *obj)
{
    listLoadedObjects.removeAll(obj);

    if(!obj->modelIndex.isValid())
        return;

    if(obj->parked) {
        if(obj->modelIndex.model()==&parkModel)
            parkModel.removeRow(obj->modelIndex.row());
    } else {
        if(obj->modelIndex.model()==myHost->GetModel())
            myHost->GetModel()->removeRow(obj->modelIndex.row(), obj->modelIndex.parent());
    }
}

void Container::RemoveCable(QModelIndex & index)
{
    currentProgram->RemoveCable(index);
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
    const quint16 file_version = 1;
    out << file_version;

    switch(myHost->filePass) {

        //save all loaded objects
        case 0:
        {
            out << (qint16)index;
            out << objectName();
            out << sleep;

            out << (quint16)listLoadedObjects.size();
            foreach(Object* obj, listLoadedObjects) {
                QByteArray tmpStream;
                QDataStream tmp( &tmpStream , QIODevice::ReadWrite);
                tmp << *obj;

                out << obj->info();
                out << (quint16)tmpStream.size();
                out << tmpStream;
            }
            break;
        }

        //save all programs
        case 1:
        {
            out << (quint32)listContainerPrograms.size();
            QHash<int,ContainerProgram*>::const_iterator i = listContainerPrograms.constBegin();
            while(i!=listContainerPrograms.constEnd()) {
                out << (quint32)i.key();
                out << *(i.value());
                ++i;
            }
            break;
        }
        case 2:
            out << (quint32)currentProgId;
            break;
    }

    return out;

}

QDataStream & Container::fromStream (QDataStream& in)
{
    quint16 file_version;
    in >> file_version;

    switch(myHost->filePass) {

        //load all objects
        case 0:
        {
            LoadProgram(TEMP_PROGRAM);

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
                    quint16 streamSize;
                    QByteArray tmpStream;
                    QDataStream tmp( &tmpStream , QIODevice::ReadWrite);

                    in >> info;
                    in >> streamSize;
                    in >> tmpStream;

                    QSharedPointer<Object> objPtr = myHost->objFactory->NewObject(info);
                    if(!objPtr.isNull()) {
                        AddParkedObject(objPtr);
                        tmp >> *objPtr.data();

                        //keep the object alive while loading
                        listLoadingObjects << objPtr;
                    } else {
                        //error while creating the object, build a dummy object with the same saved id
                        info.objType=ObjType::dummy;
                        objPtr = myHost->objFactory->NewObject(info);
                        QDataStream tmp2( &tmpStream , QIODevice::ReadWrite);
                        if(!objPtr.isNull()) {
                            AddParkedObject(objPtr);
                            tmp2 >> *objPtr.data();

                            //keep the object alive while loading
                            listLoadingObjects << objPtr;
                        } else {
                            //can't even create a dummy object ?
                            debug("Container::fromStream dummy object not created")
                        }
                    }
            }
            break;
        }

        //load the programs
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
                    prog = new ContainerProgram(myHost,this);
                    listContainerPrograms.insert(progId,prog);
                }

                in >> *prog;
            }
            break;
        }

        //clear the loading list : delete unused objects
        case 2:
            quint32 prog;
            in >> prog;
            //UnloadProgram();
            LoadProgram(prog);
            listLoadingObjects.clear();
            listContainerPrograms.remove(TEMP_PROGRAM);
            break;
    }
    return in;
}
