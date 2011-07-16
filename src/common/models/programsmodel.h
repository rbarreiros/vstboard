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

#ifndef PROGRAMSMODEL_H
#define PROGRAMSMODEL_H
#include "precomp.h"

class MainHost;
class ProgramsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit ProgramsModel(MainHost *parent = 0);
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
    void removeRows ( QModelIndexList &listToRemove, const QModelIndex & parent = QModelIndex() );

    QStringList mimeTypes () const;
    QMimeData * mimeData ( const QModelIndexList & indexes ) const;

    bool GroupFromStream( QDataStream &stream, int row);
    bool ProgramFromStream( QDataStream &stream, int row, int groupNum);
    bool GroupToStream( QDataStream &stream, const QModelIndex &groupIndex) const;
    bool ProgramToStream( QDataStream &stream, const QModelIndex &progIndex) const;
    bool GroupToStream( QDataStream &stream, int row) const;
    bool ProgramToStream( QDataStream &stream, int row, int groupNum) const;

private:
    bool RemoveProgram( int row, int groupNum );
    bool RemoveGroup( int row );

    bool fromCom;

    QUndoCommand *currentCommand;
    MainHost *myHost;
    int droppedItemsCount;
    int movingItemLeft;
    int movingDestRow;
    QModelIndex movingToParent;
    QList< QPair<QPersistentModelIndex,QPersistentModelIndex> >listMovedIndex;

    friend class ComAddProgram;
    friend class ComAddGroup;
    friend class ComRemoveProgram;
    friend class ComRemoveGroup;
};

#endif // PROGRAMSMODEL_H
