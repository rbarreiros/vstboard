#include "comaddcable.h"
#include "connectables/objectfactory.h"
#include "connectables/container.h"
#include "mainhost.h"

ComAddCable::ComAddCable(MainHost *myHost,
                         const ConnectionInfo &outInfo,
                         const ConnectionInfo &inInfo,
                         QUndoCommand  *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    outInfo(outInfo),
    inInfo(inInfo),
    currentGroup(0),
    currentProg(0)
{
    if(outInfo.direction==PinDirection::Input) {
        ConnectionInfo tmp(outInfo);
        this->outInfo=ConnectionInfo(inInfo);
        this->inInfo=ConnectionInfo(tmp);
    }
    setText(QObject::tr("Add cable"));

    currentGroup = myHost->programList->GetCurrentMidiGroup();
    currentProg =  myHost->programList->GetCurrentMidiProg();
}

void ComAddCable::undo ()
{
    myHost->programList->ChangeProgNow(currentGroup,currentProg);

    QSharedPointer<Connectables::Container>cntPtr = myHost->objFactory->GetObjectFromId( inInfo.container ).staticCast<Connectables::Container>();
    if(!cntPtr)
        return;
    static_cast<Connectables::Container*>(cntPtr.data())->UserRemoveCable(outInfo,inInfo);
}

void ComAddCable::redo ()
{
    myHost->programList->ChangeProgNow(currentGroup,currentProg);

    QSharedPointer<Connectables::Container>cntPtr = myHost->objFactory->GetObjectFromId( inInfo.container ).staticCast<Connectables::Container>();
    if(!cntPtr)
        return;
    static_cast<Connectables::Container*>(cntPtr.data())->UserAddCable(outInfo,inInfo);
}
