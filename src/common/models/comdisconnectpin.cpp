#include "comdisconnectpin.h"
#include "connectables/objectfactory.h"
#include "connectables/container.h"
#include "mainhost.h"

ComDisconnectPin::ComDisconnectPin(MainHost *myHost,const ConnectionInfo &pinInfo) :
    myHost(myHost),
    pinInfo(pinInfo)
{
    setText(QObject::tr("Disconnect pin"));
}

void ComDisconnectPin::undo ()
{
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
    QSharedPointer<Connectables::Container>cntPtr = myHost->objFactory->GetObjectFromId( pinInfo.container ).staticCast<Connectables::Container>();
    if(!cntPtr)
        return;

    listConnectedPins.clear();
    cntPtr->GetListOfConnectedPinsTo(pinInfo, listConnectedPins);
    cntPtr->UserRemoveCableFromPin(pinInfo);
}
