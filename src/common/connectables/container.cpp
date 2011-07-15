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
#include "container.h"
#include "objectfactory.h"
#include "mainhost.h"
#include "projectfile/projectfile.h"

using namespace Connectables;

/*!
  \class Connectables::Container
  \brief contains Objects an Cables, uses Bridges to communicate with other Containers
  */

/*!
  \param myHost pointer to the MainHost
  \param index object number
  \param info object description
  */
Container::Container(MainHost *myHost,int index, const ObjectInfo &info) :
    Object(myHost,index, info ),
    bridgeIn(0),
    bridgeOut(0),
    optimizerFlag(false),
    currentContainerProgram(0),
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

/*!
  Close on destruction
  */
Container::~Container()
{
    Close();
}

/*!
  Get the cables model node (cables are in a child node)
  \return the node or an invalid index
  */
const QModelIndex &Container::GetCablesIndex()
{
    // the node exists, return it
    if(cablesNode.isValid())
        return cablesNode;

    // the container is parked or the node is not valid, return an invalid index
    if(parked || !modelIndex.isValid()) {
        cablesNode = QModelIndex();
        return cablesNode;
    }

    //create the cables node
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

void Container::SetContainerId(quint16 id)
{
    Object::SetContainerId(id);

    foreach(Object* obj, listLoadedObjects) {
        obj->SetContainerId(index);
    }
}

/*!
  Connect the output pins of fromObjOutputs to the inputs of toObjInputs
  \param fromObjOutputs
  \param toObjInputs
  \param hidden true to create hidden cables
  */
void Container::ConnectObjects(QSharedPointer<Object> fromObjOutputs, QSharedPointer<Object> toObjInputs, bool hiddenCables)
{
    if(fromObjOutputs.isNull() || toObjInputs.isNull())
        return;

    fromObjOutputs->GetListAudioPinOut()->ConnectAllTo(this,toObjInputs->GetListAudioPinIn(), hiddenCables);
    fromObjOutputs->GetListMidiPinOut()->ConnectAllTo(this,toObjInputs->GetListMidiPinIn(), hiddenCables);
    fromObjOutputs->GetListBridgePinOut()->ConnectAllTo(this,toObjInputs->GetListBridgePinIn(), hiddenCables);
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

    if(currentContainerProgram) {
        currentContainerProgram->Unload();
        currentContainerProgram->ParkAllObj();
        delete currentContainerProgram;
        currentContainerProgram=0;
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

    if(!currentContainerProgram)
        return;

    foreach(QSharedPointer<Object> objPtr, currentContainerProgram->listObjects) {
        if(!objPtr.isNull())
            objPtr->SetSleep(sleep);
    }
}

void Container::Hide()
{
    cablesNode=QModelIndex();

    if(currentContainerProgram) {
        foreach(QSharedPointer<Object> objPtr, currentContainerProgram->listObjects) {
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

bool Container::IsDirty()
{
    return currentContainerProgram->IsDirty();
}

void Container::SetDirty()
{
    currentContainerProgram->SetDirty();
}

/*!
  Will change program on the next render loop
  \param prg a program model index
  */
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
    QMutexLocker ml(&progLoadMutex);

    //if prog is already loaded, update model
    if(prog==currentProgId && currentContainerProgram) {
        UpdateModelNode();
        return;
    }

    if(!listContainerPrograms.contains(prog))
        listContainerPrograms.insert(prog,new ContainerProgram(myHost,this));

    ContainerProgram *oldProg = currentContainerProgram;
    ContainerProgram *newProg = listContainerPrograms.value(prog);


    if(oldProg) {
        //update the saved rendering map
        if(optimizerFlag && currentProgId!=EMPTY_PROGRAM && currentProgId!=TEMP_PROGRAM) {
            ContainerProgram *p = listContainerPrograms.value(currentProgId,0);
            if(p)
                p->SaveRendererState();
        }

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
    currentContainerProgram = new ContainerProgram(*newProg);

    //add new objects
    foreach(QSharedPointer<Object>objPtr, newProg->listObjects) {
        if(!oldProg->listObjects.contains(objPtr)) {
            AddChildObject(objPtr);
        }
    }

    currentContainerProgram->Load(prog);
    if(optimizerFlag)
        currentContainerProgram->LoadRendererState();
    UpdateModelNode();

    if(oldProg) {
        delete oldProg;
    }

    //Updated();
}

const QTime Container::GetLastUpdate() {
    QTime parentTime;
    QTime myTime;

    if(parentContainer)
        parentTime = parentContainer.toStrongRef()->GetLastUpdate();

    if(currentContainerProgram)
        myTime = currentContainerProgram->timeSavedRendererNodes;

    if(myTime>parentTime)
        return myTime;
    else
        return parentTime;
}

void Container::SaveProgram()
{
    if(!currentContainerProgram && currentProgId==TEMP_PROGRAM)
        return;

    currentContainerProgram->Save();

    delete listContainerPrograms.take(currentProgId);
    listContainerPrograms.insert(currentProgId,currentContainerProgram);
    ContainerProgram *tmp = new ContainerProgram(*currentContainerProgram);
    currentContainerProgram = tmp;
}

void Container::UnloadProgram()
{
    if(!currentContainerProgram)
        return;

    currentContainerProgram->Unload();
    delete currentContainerProgram;
    currentContainerProgram=0;

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
        ContainerProgram* progCpy = currentContainerProgram->CopyTo(dest);
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

    if(prg == currentProgId) {
        debug2(<<"Container::RemoveProgram removing current program ! "<<prg<<objectName())
        return;
    }

    ContainerProgram* prog = listContainerPrograms.take(prg);
    prog->Remove(prg);
    delete prog;
}

/*!
  Add a new object in the current program
  \param objPtr shared pointer to the object
  */
void Container::AddObject(QSharedPointer<Object> objPtr)
{
    objPtr->SetContainerId(index);
    objPtr->UnloadProgram();

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
    currentContainerProgram->AddObject(objPtr);
    objPtr->LoadProgram(currentProgId);
}

/*!
  Place an object and park it
  \param objPtr shared pointer to the object
  */
void Container::AddParkedObject(QSharedPointer<Object> objPtr)
{
    objPtr->SetContainerId(index);
    objPtr->UnloadProgram();

    listLoadedObjects << objPtr.data();
    ParkChildObject(objPtr);
}

void Container::UserAddObject(const QSharedPointer<Object> &objPtr,
                              InsertionType::Enum insertType,
                              QList< QPair<ConnectionInfo,ConnectionInfo> > *listOfAddedCables,
                              QList< QPair<ConnectionInfo,ConnectionInfo> > *listOfRemovedCables,
                              const QSharedPointer<Object> &targetPtr)
{
    AddObject(objPtr);

    if(targetPtr) {
        currentContainerProgram->CollectCableUpdates( listOfAddedCables, listOfRemovedCables );

        switch(insertType) {
            case InsertionType::InsertBefore:
                MoveInputCablesFromObj(objPtr, targetPtr);
                ConnectObjects(objPtr, targetPtr, false);
                break;
            case InsertionType::InsertAfter:
                MoveOutputCablesFromObj(objPtr, targetPtr);
                ConnectObjects(targetPtr, objPtr, false);
                break;
            case InsertionType::AddBefore:
                ConnectObjects(objPtr, targetPtr, false);
                break;
            case InsertionType::AddAfter:
                ConnectObjects(targetPtr, objPtr, false);
                break;
            case InsertionType::Replace:
                CopyCablesFromObj(objPtr, targetPtr);
                (targetPtr)->CopyStatusTo(objPtr);
                ParkObject(targetPtr);
                break;
            case InsertionType::NoInsertion:
                break;
        }
        currentContainerProgram->CollectCableUpdates();
    }

    myHost->SetSolverUpdateNeeded();
    Updated();
}


void Container::UserParkObject(QSharedPointer<Object> objPtr,
                               RemoveType::Enum removeType,
                               QList< QPair<ConnectionInfo,ConnectionInfo> > *listOfAddedCables,
                               QList< QPair<ConnectionInfo,ConnectionInfo> > *listOfRemovedCables)
{
    currentContainerProgram->CollectCableUpdates( listOfAddedCables, listOfRemovedCables );

    if(removeType==RemoveType::BridgeCables)
        currentContainerProgram->CreateBridgeOverObj(objPtr->GetIndex());

    ParkObject(objPtr);

    currentContainerProgram->CollectCableUpdates();

    myHost->SetSolverUpdateNeeded();
    Updated();
}

/*!
  Remove an object from the panel (delete it or park it)
  \param objPtr shared pointer to the object
  */
void Container::ParkObject(QSharedPointer<Object> objPtr)
{
    objPtr->ToggleEditor(false);

    //this container is not programmable : delete the object from the saved program too
    if(!listenProgramChanges) {
        foreach(ContainerProgram *prg, listContainerPrograms) {
            prg->RemoveObject(objPtr);
        }
    }

    if(currentContainerProgram)
        currentContainerProgram->RemoveObject(objPtr);

    if(objPtr==bridgeIn)
        bridgeIn.clear();
    if(objPtr==bridgeOut)
        bridgeOut.clear();

    listStaticObjects.removeAll(objPtr);
}

/*!
  Copy cables from an object
  \param newObjPtr the new object
  \param ObjPtr the object to copy
  */
void Container::CopyCablesFromObj(QSharedPointer<Object> newObjPtr, QSharedPointer<Object> ObjPtr)
{
    if(!currentContainerProgram)
        return;
    currentContainerProgram->CopyCablesFromObj( newObjPtr->GetIndex(), ObjPtr->GetIndex() );
}

void Container::MoveOutputCablesFromObj(QSharedPointer<Object> newObjPtr, QSharedPointer<Object> ObjPtr)
{
    if(!currentContainerProgram)
        return;
    currentContainerProgram->MoveOutputCablesFromObj( newObjPtr->GetIndex(), ObjPtr->GetIndex() );
}

void Container::MoveInputCablesFromObj(QSharedPointer<Object> newObjPtr, QSharedPointer<Object> ObjPtr)
{
    if(!currentContainerProgram)
        return;
    currentContainerProgram->MoveInputCablesFromObj( newObjPtr->GetIndex(), ObjPtr->GetIndex() );
}

void Container::GetListOfConnectedPinsTo(const ConnectionInfo &pin, QList<ConnectionInfo> &list)
{
    if(!currentContainerProgram)
        return;
    currentContainerProgram->GetListOfConnectedPinsTo(pin,list);
}

/*!
  Called by ContainerProgam, move a parked object to the panel
  \param objPtr shared pointer to the object
  */
void Container::AddChildObject(QSharedPointer<Object> objPtr)
{
    if(!modelIndex.isValid()) {
        debug2(<<"Container::AddChildObject index not valid")
        return;
    }

    if(objPtr->modelIndex.isValid() && objPtr->modelIndex.model()==&parkModel)
        parkModel.removeRow(objPtr->modelIndex.row());

    QStandardItem *item = objPtr->GetFullItem();

    myHost->GetModel()->itemFromIndex( modelIndex )->appendRow(item);
    objPtr->modelIndex=item->index();
    objPtr->parked=false;

//    myHost->SetSolverUpdateNeeded();
}

/*!
  Called by ContainerProgram, remove an object from the panel
  \param objPtr shared pointer to the object
  */
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

//    myHost->SetSolverUpdateNeeded();
}

/*!
  Called by the Object destructor, remove the object
  \param obj pointer to the object
  */
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

/*!
  User removed a cable from the program
  \param index model index of the cable
  */
//void Container::RemoveCable(QModelIndex & index)
//{
//    currentProgram->RemoveCable(index);
//}

void Container::UserAddCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
{
    AddCable(outputPin,inputPin,false);
    myHost->SetSolverUpdateNeeded();

    Updated();
}

void Container::UserAddCable(const QPair<ConnectionInfo,ConnectionInfo>&pair)
{
    UserAddCable(pair.first,pair.second);
}

void Container::UserRemoveCableFromPin(const ConnectionInfo &pin)
{
    RemoveCableFromPin(pin);
    myHost->SetSolverUpdateNeeded();

    Updated();
}

void  Container::UserRemoveCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
{
    RemoveCable(outputPin,inputPin);
    myHost->SetSolverUpdateNeeded();
    Updated();
}

void Container::UserRemoveCable(const QPair<ConnectionInfo,ConnectionInfo>&pair)
{
    UserRemoveCable(pair.first,pair.second);
}

/*!
  Add a cable in this container
  \param outputPin the output pin (messages sender)
  \param inputPin the input pin (messages receiver)
  \param hidden true if the cable is hidden (the cables between containers are hidden)
  */
void Container::AddCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin, bool hidden)
{
    if(!currentContainerProgram)
        return;
    currentContainerProgram->AddCable(outputPin,inputPin, hidden);
}

/*!
  Remove a cable
  \param outputPin the output pin (messages sender)
  \param inputPin the input pin (messages receiver)
  */
void Container::RemoveCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
{
    if(!currentContainerProgram)
        return;
    currentContainerProgram->RemoveCable(outputPin,inputPin);
}

/*!
  Remove all cables from a Pin
  \param pin the pin to disconnect
  */
void Container::RemoveCableFromPin(const ConnectionInfo &pin)
{
    if(!currentContainerProgram)
        return;
    currentContainerProgram->RemoveCableFromPin(pin);
}

/*!
  Remove all cables from an Object
  \param objId the object index to disconnect
  */
//void Container::RemoveCableFromObj(int objId)
//{
//    if(!currentProgram)
//        return;
//    currentProgram->RemoveCableFromObj(objId);
//}

/*!
  Put all the ContainerProgram and children Objects in a data stream
  */
QDataStream & Container::toStream (QDataStream& out) const
{

    {
        QByteArray tmpBa;
        QDataStream tmpStream( &tmpBa, QIODevice::ReadWrite);

        //save container header
        tmpStream << (qint16)index;
        tmpStream << objectName();
        tmpStream << sleep;
        tmpStream << (quint32)currentProgId;
        ProjectFile::SaveChunk( "CntHead", tmpBa, out);
    }

    //save all loaded objects
    foreach(Object* obj, listLoadedObjects) {
        QByteArray tmpBa;
        QDataStream tmpStream( &tmpBa, QIODevice::ReadWrite);
        tmpStream << obj->info();
        obj->toStream( tmpStream );
        ProjectFile::SaveChunk( "CntObj", tmpBa, out);
    }

    //save all programs
    QHash<int,ContainerProgram*>::const_iterator i = listContainerPrograms.constBegin();
    while(i!=listContainerPrograms.constEnd()) {
        QByteArray tmpBa;
        QDataStream tmpStream( &tmpBa, QIODevice::ReadWrite);
        tmpStream << (quint32)i.key();
        tmpStream << *(i.value());
        ProjectFile::SaveChunk( "CntProg", tmpBa, out);
        ++i;
    }

    return out;
}

/*!
  Read the container from a data stream, creates all the ContainerProgram and children Objects
  */
bool Container::fromStream (QDataStream& in)
{
    //clear programs
    foreach(ContainerProgram *prog, listContainerPrograms) {
        delete prog;
    }
    listContainerPrograms.clear();

    LoadProgram(TEMP_PROGRAM);

    int savedProgId=0;

    QString chunkName;
    QByteArray tmpBa;

    while( !in.atEnd() ) {
        QDataStream tmpStream( &tmpBa , QIODevice::ReadWrite);
        ProjectFile::LoadNextChunk( chunkName, tmpBa, in );

        if(chunkName=="CntHead")
            savedProgId=loadHeaderStream(tmpStream);

        else if(chunkName=="CntObj")
            loadObjectFromStream(tmpStream);

        else if(chunkName=="CntProg")
            loadProgramFromStream(tmpStream);

        //unknown chunk
        else {
            in.setStatus(QDataStream::ReadCorruptData);
            debug2(<<"Container::fromStream unknown section"<<chunkName<<tmpBa.size())
        }

        if(!tmpStream.atEnd()) {
            debug2(<<"Container::fromStream stream not at end, drop remaining data :")
            while(!tmpStream.atEnd()) {
                char c[1000];
                int nb=tmpStream.readRawData(c,1000);
                debug2(<<nb << QByteArray::fromRawData(c,nb).toHex())
            }
            in.setStatus(QDataStream::ReadCorruptData);
        }

        if(tmpStream.status()==QDataStream::ReadPastEnd) {
            debug2(<<"Container::fromStream ReadPastEnd"<<tmpStream.status())
            return false;
        }
    }

    //load the saved program
    LoadProgram(savedProgId);

    //clear the loading list : delete unused objects
    listLoadingObjects.clear();
    delete listContainerPrograms.take(TEMP_PROGRAM);

    return true;
}

quint32 Container::loadHeaderStream (QDataStream &in)
{
    //load header
    qint16 id;
    in >> id;
    savedIndex = id;

    QString name;
    in >> name;
    setObjectName(name);

    in >> sleep;

    quint32 savedProgId=0;
    in >> savedProgId;
    return savedProgId;
}

bool Container::loadObjectFromStream (QDataStream &in)
{
    ObjectInfo info;
    in >> info;
    info.forcedObjId=0;

    QSharedPointer<Object> objPtr = myHost->objFactory->NewObject(info);

    //error while creating the object, build a dummy object with the same saved id
    if(objPtr.isNull()) {
        objPtr = myHost->objFactory->NewObject(info);
    }

    //can't even create a dummy object ?
    if(objPtr.isNull()) {
        debug("Container::fromStream dummy object not created")
    }

    if(!objPtr.isNull()) {
        AddParkedObject(objPtr);
        if(objPtr->fromStream( in ))
            listLoadingObjects << objPtr; //keep the object alive while loading
    }

    return true;
}

bool Container::loadProgramFromStream (QDataStream &in)
{
    quint32 progId;
    in >> progId;

    ContainerProgram *prog = new ContainerProgram(myHost,this);
    in >> *prog;
    listContainerPrograms.insert(progId,prog);

    return true;
}

void Container::ProgramToStream (int progId, QDataStream &out)
{
    if(progId == currentProgId)
        SaveProgram();

    ContainerProgram *prog = listContainerPrograms.value(progId);
    if(!prog) {
        out << (quint8)0;
        return;
    }
    out << (quint8)1;

    quint16 nbObj = prog->listObjects.size();
    out << nbObj;
    foreach(QSharedPointer<Object>obj, prog->listObjects) {
        QByteArray tmpBa;
        QDataStream tmpStream( &tmpBa , QIODevice::ReadWrite);
        if(obj) {
            tmpStream << obj->info();
            obj->ProgramToStream( progId, tmpStream );
        }
        out << tmpBa;
    }

    out << *prog;
    out << (quint32)currentProgId;
}

void Container::ProgramFromStream (int progId, QDataStream &in)
{
    quint8 valid=0;
    in >> valid;
    if(valid!=1)
        return;

    QList<QSharedPointer<Object> >tmpListObj;

    quint16 nbObj;
    in >> nbObj;
    for(int i=0; i<nbObj; i++) {
        QByteArray tmpBa;
        QDataStream tmpStream( &tmpBa , QIODevice::ReadWrite);
        in >> tmpBa;
        ObjectInfo info;
        tmpStream >> info;
        QSharedPointer<Object>obj = myHost->objFactory->GetObjectFromId( info.forcedObjId );
        if(!obj) {
            obj = myHost->objFactory->NewObject(info);
            AddParkedObject(obj);
            tmpListObj << obj;
        } else {
            obj->SetContainerId(index);
            obj->ResetSavedIndex(info.forcedObjId);
        }
        if(!obj) {
            debug2(<<"Container::ProgramFromStream can't create obj"<<info.id<<info.name)
        }
        if(obj) {
            obj->ProgramFromStream(progId, tmpStream);
        }
    }

    if(!listContainerPrograms.contains(progId))
        listContainerPrograms.insert(progId, new ContainerProgram(myHost,this));

    in >> *listContainerPrograms.value(progId);

    quint32 savedProgId;
    in >> savedProgId;

    myHost->objFactory->ResetSavedId();
}
