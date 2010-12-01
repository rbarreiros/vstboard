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

#ifndef HOSTMODEL_H
#define HOSTMODEL_H

#include "../precomp.h"

class HostModel : public QStandardItemModel
{
Q_OBJECT
public:
    HostModel(QObject *parent=0);

    QMimeData * mimeData ( const QModelIndexList & indexes ) const;

    QStringList mimeTypes () const;
    bool dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
//    bool setItemData ( const QModelIndex & index, const QMap<int, QVariant> & roles );
protected:
    QMap<int,QStandardItem*>mapObjects;
};

#endif // HOSTMODEL_H
