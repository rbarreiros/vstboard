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

#include "hostmodelproxy.h"
#include "globals.h"

HostModelProxy::HostModelProxy(QStandardItemModel* model) :
    QObject(model),
    model(model)
{
    connect(this,SIGNAL(_add(QStandardItem*,QStandardItem*,int)),
            this,SLOT(addObject(QStandardItem*,QStandardItem*,int)),
            Qt::QueuedConnection);
    connect(this,SIGNAL(_remove(int)),
            this,SLOT(removeObject(int)),
            Qt::QueuedConnection);
    connect(this,SIGNAL(_update(QVariant,int,int)),
            this,SLOT(updateObject(QVariant,int,int)),
            Qt::QueuedConnection);
}

void HostModelProxy::Add(int objId, QStandardItem *item, QStandardItem *parent)
{
    emit _add(parent,item,objId);
}

void HostModelProxy::Remove(int objId)
{
    emit _remove(objId);
}

void HostModelProxy::Remove(int row, const QModelIndex & parent)
{
    int objId = parent.child(row,0).data(UserRoles::value).toInt();
    emit _remove(objId);
}

void HostModelProxy::Update(int objId, int role, const QVariant & value)
{
    emit _update(value,role,objId);
}

void HostModelProxy::addObject (QStandardItem * parent, QStandardItem* item, int objId)
{
    parent->appendRow(item);
    if(objId!=-1)
        mapObjects.insert(objId,item);
}

void HostModelProxy::removeObject (int objId)
{
    QStandardItem *item = mapObjects.value(objId,0);
    if(!item)
        return;
    if(item->parent())
        item->parent()->removeRow(item->row());

    mapObjects.remove(objId);
}

void HostModelProxy::updateObject( const QVariant & value, int role, int objId)
{
    QStandardItem *item = mapObjects.value(objId,0);
    if(!item)
        return;
    item->setData(value,role);
}
