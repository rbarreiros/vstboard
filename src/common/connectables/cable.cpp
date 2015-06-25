/**************************************************************************
#    Copyright 2010-2011 Rapha�l Fran�ois
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


#include "cable.h"
#include "mainhost.h"
#include "circularbuffer.h"
#include "pin.h"

using namespace Connectables;

/*!
  \class Connectables::Cable
  \brief cable are created in Container to connect Objects
  */

/*!
  \param myHost pointer to the MainHost
  \param pinOut the output pin (the one sending the messages)
  \param pinIn the input pin (the receiver)
  */
Cable::Cable(MainHost *myHost, const ConnectionInfo &pinOut, const ConnectionInfo &pinIn) :
    pinOut(pinOut),
    pinIn(pinIn),
    modelIndex(QModelIndex()),
    myHost(myHost),
    buffer(0),
    delay(0),
    tmpBuf(0)
{
}

/*!
  Copy a cable
  \param c the model
  */
Cable::Cable(const Cable & c) :
    pinOut(c.pinOut),
    pinIn(c.pinIn),
    modelIndex(c.modelIndex),
    myHost(c.myHost),
    buffer(0),
    delay(c.delay),
    tmpBuf(0)
{
}

Cable::~Cable()
{
    if(buffer)
        delete buffer;
}

/*!
  Add the cable in the model
  \param parentIndex the Container index
  \return true if cable is created
  */
void Cable::AddToParentNode(const QModelIndex &parentIndex)
{
    SetDelay(delay);

    QStandardItem *item = new QStandardItem(QString("cable %1:%2 %3").arg(pinOut.objId).arg(pinIn.objId).arg(delay));
    item->setData(QVariant::fromValue(ObjectInfo(NodeType::cable)),UserRoles::objInfo);
    item->setData(QVariant::fromValue(pinOut),UserRoles::value);
    item->setData(QVariant::fromValue(pinIn),UserRoles::connectionInfo);
    item->setData(delay,UserRoles::position);

    QStandardItem *parentItem = myHost->GetModel()->itemFromIndex(parentIndex);
    if(!parentItem) {
        LOG("parent item not found");
        return;
    }
    parentItem->appendRow(item);
    modelIndex = item->index();
}

/*!
  Remove the cable from the model
  \param parentIndex the Container index
  */
void Cable::RemoveFromParentNode(const QModelIndex &parentIndex)
{
    delete buffer;
    buffer=0;
    delete tmpBuf;
    tmpBuf=0;

    if(modelIndex.isValid() && parentIndex.isValid())
        myHost->GetModel()->removeRow(modelIndex.row(), parentIndex);

    modelIndex=QModelIndex();
}

bool Cable::SetDelay(quint32 d)
{
    delay=d;
    if(modelIndex.isValid()) {
        QStandardItem *item = myHost->GetModel()->itemFromIndex(modelIndex);
        item->setText( QString("cable %1:%2 %3").arg(pinOut.objId).arg(pinIn.objId).arg(delay) );
        item->setData(d,UserRoles::position);
    }

    if(delay==0) {
        if(buffer) {
            delete buffer;
            buffer=0;
            delete tmpBuf;
            tmpBuf=0;
        }
        return true;
    }

    if(!buffer)
        buffer = new CircularBuffer(myHost->GetBufferSize()*2 + delay);
    else
        buffer->SetSize(myHost->GetBufferSize()*2 + delay);

    if(!tmpBuf)
        tmpBuf = new AudioBuffer(false,false);

    tmpBuf->SetSize(myHost->GetBufferSize());
    return true;
}

void Cable::Render(const PinMessage::Enum msgType,void *data)
{
    Pin *pin = myHost->objFactory->GetPin(pinIn);
    if(!pin)
        return;

    if(buffer==0) {
        pin->ReceiveMsg(msgType,data);
        return;
    }

    //fill buffer
    if(msgType==PinMessage::AudioBuffer) {
        AudioBuffer *buf = static_cast<AudioBuffer*>(data);
        buffer->Put( (float*)buf->GetPointer(), buf->GetSize());
    }

    //buffer is full, send data
    if(buffer->filledSize >= (long)(delay + myHost->GetBufferSize())) {
        buffer->Get( (float*)tmpBuf->GetPointerWillBeFilled(),tmpBuf->GetSize() );
        tmpBuf->ConsumeStack();
        pin->ReceiveMsg(msgType,(void*)tmpBuf);
//        static_cast<AudioPin*>(pin)->GetBuffer()->AddToStack(tmpBuf);
    }

}
