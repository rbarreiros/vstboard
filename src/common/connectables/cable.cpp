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
#include "heap.h"
#ifndef QT_NO_DEBUG
#define new DEBUG_CLIENTBLOCK
#endif


#include "cable.h"
#include "../mainhost.h"

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
        myHost(myHost)
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
        myHost(c.myHost)
{

}

/*!
  Add the cable in the model
  \param parentIndex the Container index
  \return true if cable is created
  */
void Cable::AddToParentNode(const QModelIndex &parentIndex)
{
    QStandardItem *item = new QStandardItem(QString("cable %1:%2").arg(pinOut.objId).arg(pinIn.objId));
    item->setData(QVariant::fromValue(ObjectInfo(NodeType::cable)),UserRoles::objInfo);
    item->setData(QVariant::fromValue(pinOut),UserRoles::value);
    item->setData(QVariant::fromValue(pinIn),UserRoles::connectionInfo);

    QStandardItem *parentItem = myHost->GetModel()->itemFromIndex(parentIndex);
    if(!parentItem) {
        debug("Cable::AddToParentNode parent item not found")
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
    if(modelIndex.isValid() && parentIndex.isValid())
        myHost->GetModel()->removeRow(modelIndex.row(), parentIndex);

    modelIndex=QModelIndex();
}
