#include "comremoveobject.h"
#include "mainhost.h"
#include "connectables/container.h"

ComRemoveObject::ComRemoveObject( MainHost *myHost,
                                  const QModelIndex &objIndex,
                                  RemoveType::Enum removeType,
                                  QUndoCommand  *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    removeType(removeType),
    currentGroup(0),
    currentProg(0)
{
    QSharedPointer<Connectables::Container>container = myHost->objFactory->GetObj( objIndex.parent() ).staticCast<Connectables::Container>();
    if(!container)
        return;

    QSharedPointer<Connectables::Object>object = myHost->objFactory->GetObj( objIndex );
    if(!object)
        return;

    setText(QObject::tr("Remove %1").arg(object->objectName()));

    //save references
    ContainerPtr = container;
    objectInfo = object->info();

    currentGroup = myHost->programList->GetCurrentMidiGroup();
    currentProg =  myHost->programList->GetCurrentMidiProg();
}

void ComRemoveObject::undo ()
{
    myHost->programList->ChangeProgNow(currentGroup,currentProg);

    //get the object
    QSharedPointer<Connectables::Object> obj = myHost->objFactory->GetObjectFromId( objectInfo.forcedObjId );
    if(!obj) {
        //object was deleted, create a new one
        obj = myHost->objFactory->NewObject( objectInfo );
        ObjectPtr = obj;
    }
    if(!obj)
        return;

    objectInfo = obj->info();

    //get the container
    QSharedPointer<Connectables::Container> container = ContainerPtr.toStrongRef();
    if(!container)
        return;

    QDataStream stream(&objState, QIODevice::ReadWrite);
    obj->fromStream( stream );
    objState.resize(0);

    container->UserAddObject( obj );

    obj->SetContainerAttribs(attr);

    //remove cables added at creation
    QPair<ConnectionInfo,ConnectionInfo>pair;
    foreach( pair, listAddedCables) {
        container->UserRemoveCable(pair);
    }
    //add cables removed at creation
    foreach( pair, listRemovedCables) {
        container->UserAddCable(pair);
    }

    listAddedCables.clear();
    listRemovedCables.clear();
}

void ComRemoveObject::redo()
{
    myHost->programList->ChangeProgNow(currentGroup,currentProg);

    //get the object
    QSharedPointer<Connectables::Object> obj = myHost->objFactory->GetObjectFromId( objectInfo.forcedObjId );
    if(!obj)
        return;

    //get the container
    QSharedPointer<Connectables::Container> container = ContainerPtr.toStrongRef();
    if(!container)
        return;

    QDataStream stream(&objState, QIODevice::ReadWrite);
    obj->SaveProgram();
    obj->toStream( stream );
    obj->GetContainerAttribs(attr);

    //remove the object
    container->UserParkObject(obj,removeType,&listAddedCables,&listRemovedCables);

}
