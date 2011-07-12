#include "comaddobject.h"
#include "mainhost.h"
#include "connectables/container.h"
#include "comremoveobject.h"

ComAddObject::ComAddObject(MainHost *myHost,
                           const ObjectInfo & objInfo,
                           const QSharedPointer<Connectables::Container> &container,
                           const QSharedPointer<Connectables::Object> &targetObj,
                           InsertionType::Enum insertType,
                           QUndoCommand  *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    objectInfo(objInfo),
    targetInfo(ObjectInfo()),
    ContainerPtr(container),
    insertType(insertType),
    currentGroup(0),
    currentProg(0)
{
    setText(QObject::tr("Add object"));

    if(targetObj)
        targetInfo = targetObj->info();

    currentGroup = myHost->programList->GetCurrentMidiGroup();
    currentProg =  myHost->programList->GetCurrentMidiProg();
}

void ComAddObject::undo ()
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
    container->UserParkObject(obj);

    //add the replaced object
    if(insertType==InsertionType::Replace) {
        QSharedPointer<Connectables::Object> target = myHost->objFactory->GetObjectFromId( targetInfo.forcedObjId );
        if(!target) {
            //the target has been deleted, create it back
            target = myHost->objFactory->NewObject( targetInfo );
        }
        if(target) {
            container->UserAddObject(target);
        }
    }

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

void ComAddObject::redo ()
{
    myHost->programList->ChangeProgNow(currentGroup,currentProg);

    //get the object
    QSharedPointer<Connectables::Object> obj = myHost->objFactory->GetObjectFromId( objectInfo.forcedObjId );
    if(!obj) {
        //object was deleted, create a new one
        obj = myHost->objFactory->NewObject( objectInfo );
    }
    if(!obj)
        return;

    setText(QObject::tr("Add %1").arg(obj->objectName()));
    objectInfo = obj->info();

    //get the container
    QSharedPointer<Connectables::Container> container = ContainerPtr.toStrongRef();
    if(!container)
        return;

    //get the target
    QSharedPointer<Connectables::Object> target = myHost->objFactory->GetObjectFromId( targetInfo.forcedObjId );
    if(target)
        targetInfo = target->info();

    if(objState.size()!=0) {
        QDataStream stream(&objState, QIODevice::ReadWrite);
        obj->fromStream( stream );
        objState.clear();
    }

    //add the object to the container
    container->UserAddObject( obj, insertType, &listAddedCables, &listRemovedCables, target );

    if(attr.position!=QPointF(0.0f,0.0f)) {
        obj->SetContainerAttribs(attr);
    }



}
