#include "comdisconnectpin.h"
#include "connectables/objectfactory.h"
#include "connectables/container.h"
#include "mainhost.h"

ComDisconnectPin::ComDisconnectPin(MainHost *myHost,
                                   const ConnectionInfo &pinInfo,
                                   QUndoCommand  *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    pinInfo(pinInfo),
    currentGroup(0),
    currentProg(0)
{
    setText(QObject::tr("Disconnect pin"));

    currentGroup = myHost->programList->GetCurrentMidiGroup();
    currentProg =  myHost->programList->GetCurrentMidiProg();
}

void ComDisconnectPin::undo ()
{
    myHost->programList->ChangeProgNow(currentGroup,currentProg);

    QSharedPointer<Connectables::Container>cntPtr = myHost->objFactory->GetObjectFromId( pinInfo.container ).staticCast<Connectables::Container>();
    if(!cntPtr)
        return;

    foreach(ConnectionInfo info, listConnectedPins) {
        if(pinInfo.direction==PinDirection::Output) {
            cntPtr->UserAddCable(pinInfo, info);
        } else {
            cntPtr->UserAddCable(info, pinInfo);
        }
    }
}

void ComDisconnectPin::redo ()
{
    myHost->programList->ChangeProgNow(currentGroup,currentProg);

    QSharedPointer<Connectables::Container>cntPtr = myHost->objFactory->GetObjectFromId( pinInfo.container ).staticCast<Connectables::Container>();
    if(!cntPtr)
        return;

    listConnectedPins.clear();
    cntPtr->GetListOfConnectedPinsTo(pinInfo, listConnectedPins);
    cntPtr->UserRemoveCableFromPin(pinInfo);
}
