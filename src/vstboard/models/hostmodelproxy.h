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

#ifndef HOSTMODELPROXY_H
#define HOSTMODELPROXY_H

#include "../precomp.h"
#include "hostmodel.h"

class HostModelProxy : public QObject
{
    Q_OBJECT
public:
    explicit HostModelProxy(QStandardItemModel* model);

    void Add(const int objId, QStandardItem *item, const int parentId);
    void Remove(const int objId);
//    void Remove(int row, const QModelIndex & parent);
    void Update(const int objId, const int role, const QVariant & value);

private:
    QMap<int,QStandardItem*>mapObjects;
    QStandardItemModel *model;

signals:
    void _add(int parentId, QStandardItem* item, const int objId);
    void _remove(const int objId);
    void _update( const QVariant & value, const int role, const int objId);

public slots:

private slots:
    void __addObject(int parentId, QStandardItem* item, const int objId );
    void __removeObject (const int objId);
    void __updateObject( const QVariant & value, const int role, const int objId);
};

#endif // HOSTMODELPROXY_H
