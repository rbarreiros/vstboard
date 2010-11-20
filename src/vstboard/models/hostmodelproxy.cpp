/******************************************************************************
#    Copyright 2010 Raphaël François
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
******************************************************************************/

#include "hostmodelproxy.h"
#include "globals.h"

HostModelProxy::HostModelProxy(QStandardItemModel* model) :
    QObject(model),
    model(model)
{
    connect(this,SIGNAL(_add(int,QStandardItem*,int)),
            this,SLOT(__addObject(int,QStandardItem*,int)),
            Qt::QueuedConnection);
    connect(this,SIGNAL(_remove(int)),
            this,SLOT(__removeObject(int)),
            Qt::QueuedConnection);
    connect(this,SIGNAL(_update(QVariant,int,int)),
            this,SLOT(__updateObject(QVariant,int,int)),
            Qt::QueuedConnection);
}

void HostModelProxy::Add(const int objId, QStandardItem *item, const int parentId)
{
    emit _add(parentId,item,objId);
}

void HostModelProxy::Remove(const int objId)
{
    emit _remove(objId);
}

//void HostModelProxy::Remove(int row, const QModelIndex & parent)
//{
//    int objId = parent.child(row,0).data(UserRoles::value).toInt();
//    emit _remove(objId);
//}

void HostModelProxy::Update(const int objId, const int role, const QVariant & value)
{
    emit _update(value,role,objId);
}

void HostModelProxy::__addObject (int parentId, QStandardItem* item, const int objId)
{
    QStandardItem *parent = mapObjects.value(parentId,0);
    if(!parent)
        return;
    mapObjects.insert(objId,item);
    parent->appendRow(item);
}

void HostModelProxy::__removeObject (const int objId)
{
    QStandardItem *item = mapObjects.value(objId,0);
    if(!item)
        return;
    if(item->parent())
        item->parent()->removeRow(item->row());

    mapObjects.remove(objId);
}

void HostModelProxy::__updateObject( const QVariant & value,const  int role, const int objId)
{
    QStandardItem *item = mapObjects.value(objId,0);
    if(!item)
        return;
    item->setData(value,role);
}
