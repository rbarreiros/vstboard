/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "cable.h"
#include "../mainhost.h"

using namespace Connectables;

Cable::Cable(const ConnectionInfo &pinOut, const ConnectionInfo &pinIn) :
        pinOut(pinOut),
        pinIn(pinIn),
        modelItem(0)
{

}

Cable::Cable(const Cable & c) :
        pinOut(c.pinOut),
        pinIn(c.pinIn),
        modelItem(c.modelItem)
{

}

void Cable::AddToParentNode(QStandardItem *parent)
{
    modelItem = new QStandardItem(QString("cable %1:%2").arg(pinOut.objId).arg(pinIn.objId));
    modelItem->setData(QVariant::fromValue(ObjectInfo(NodeType::cable)),UserRoles::objInfo);
    modelItem->setData(QVariant::fromValue(pinOut),UserRoles::value);
    modelItem->setData(QVariant::fromValue(pinIn),UserRoles::connectionInfo);
    parent->appendRow(modelItem);
}

void Cable::RemoveFromParentNode(QStandardItem *parent)
{
    if(modelItem && parent)
        parent->removeRow(modelItem->row());
    modelItem=0;
}
