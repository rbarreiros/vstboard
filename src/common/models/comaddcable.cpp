#include "comaddcable.h"
#include "connectables/objectfactory.h"
#include "connectables/container.h"
#include "mainhost.h"

ComAddCable::ComAddCable(MainHost *myHost,const ConnectionInfo &outInfo, const ConnectionInfo &inInfo) :
    myHost(myHost),
    outInfo(outInfo),
    inInfo(inInfo)
{
    if(outInfo.direction==PinDirection::Input) {
        ConnectionInfo tmp(outInfo);
        this->outInfo=ConnectionInfo(inInfo);
        this->inInfo=ConnectionInfo(tmp);
    }
    setText(QObject::tr("Add cable"));
}

void ComAddCable::undo ()
{
    QSharedPointer<Connectables::Container>cntPtr = myHost->objFactory->GetObjectFromId( inInfo.container ).staticCast<Connectables::Container>();
    if(!cntPtr)
        return;
    static_cast<Connectables::Container*>(cntPtr.data())->UserRemoveCable(outInfo,inInfo);
}

void ComAddCable::redo ()
{
    QSharedPointer<Connectables::Container>cntPtr = myHost->objFactory->GetObjectFromId( inInfo.container ).staticCast<Connectables::Container>();
    if(!cntPtr)
        return;
    static_cast<Connectables::Container*>(cntPtr.data())->UserAddCable(outInfo,inInfo);
}
