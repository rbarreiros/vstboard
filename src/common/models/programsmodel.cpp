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

#include "programsmodel.h"
#include "globals.h"
#include "mainhost.h"
#include "commands/comchangeprogramitem.h"
#include "commands/comremoveprogram.h"
#include "commands/comaddprogram.h"
#include "commands/comremovegroup.h"
#include "commands/comaddgroup.h"
#include "mainwindow.h"

ProgramsModel::ProgramsModel(MainHost *parent) :
    QStandardItemModel(parent),
    droppedItemsCount(0),
    myHost(parent),
    fromCom(false),
    currentCommand(0)
{
}

bool ProgramsModel::GroupFromStream( QDataStream &stream, int row)
{
    int groupId = myHost->programList->GetNextGroupId();

    QString name;
    stream >> name;

    //create the group
    QStandardItem *groupItem = new QStandardItem( name );
    groupItem->setData(NodeType::programGroup,UserRoles::nodeType);
    groupItem->setData(groupId,UserRoles::value);
    groupItem->setData(0,UserRoles::type);
    groupItem->setDragEnabled(true);
    groupItem->setDropEnabled(false);
    groupItem->setEditable(true);

    //create the program list
    QStandardItem *prgList = new QStandardItem();
    prgList->setDragEnabled(false);
    prgList->setDropEnabled(true);
    prgList->setEditable(false);
    groupItem->appendRow(prgList);

    if(row==-1)
        row=rowCount();
    insertRow( row, groupItem );

    //send the group data to the host
    myHost->groupContainer->ProgramFromStream( groupId, stream );
    myHost->mainWindow->mySceneView->viewGroup->ProgramFromStream( groupId, stream );

    //add the programs
    QByteArray programs;
    stream >> programs;
    QDataStream streamProgs( &programs, QIODevice::ReadOnly);
    int i=0;
    while(!streamProgs.atEnd()) {
        if(!ProgramFromStream(streamProgs, i, groupItem->row()))
            return false;
        ++i;
    }

    return true;
}

bool ProgramsModel::ProgramFromStream( QDataStream &stream, int row, int groupNum)
{
    QStandardItem *groupItem = item(groupNum);
    QStandardItem *prgList = groupItem->child(0);

    if(!prgList) {
        //create the program list
        prgList = new QStandardItem();
        prgList->setDragEnabled(false);
        prgList->setDropEnabled(true);
        prgList->setEditable(false);
        groupItem->appendRow(prgList);
    }


    int progId = myHost->programList->GetNextProgId();

    QString name;
    stream >> name;

    //create the program item
    QStandardItem *prgItem = new QStandardItem( name );
    prgItem->setData(NodeType::program,UserRoles::nodeType);
    prgItem->setData(progId,UserRoles::value);
    prgItem->setData(0,UserRoles::type);
    prgItem->setDragEnabled(true);
    prgItem->setDropEnabled(false);
    prgItem->setEditable(true);

    if(row==-1)
        row=prgList->rowCount();
    prgList->insertRow(row, prgItem);

    //send the data to the host
    myHost->programContainer->ProgramFromStream( progId, stream );
    myHost->mainWindow->mySceneView->viewProgram->ProgramFromStream( progId, stream );

    return true;
}

bool ProgramsModel::GroupToStream( QDataStream &stream, int row) const
{
    return GroupToStream(stream, index(row,0));
}

bool ProgramsModel::GroupToStream( QDataStream &stream, const QModelIndex &groupIndex) const
{
    stream << groupIndex.data().toString();

    int groupId = groupIndex.data(UserRoles::value).toInt();
    myHost->groupContainer->ProgramToStream( groupId, stream );
    myHost->mainWindow->mySceneView->viewGroup->ProgramToStream( groupId, stream );

    QStandardItem *progList = itemFromIndex(groupIndex)->child(0);
    if(!progList)
        return false;

    QByteArray programs;
    QDataStream streamProgs( &programs, QIODevice::WriteOnly);
    for(int i=0; i<progList->rowCount(); i++) {
        if(!ProgramToStream( streamProgs, progList->child(i)->index() ))
            return false;
    }
    stream << programs;
    return true;
}

bool ProgramsModel::ProgramToStream( QDataStream &stream, int row, int groupNum) const
{
    return ProgramToStream(stream, item(groupNum)->child(0)->child(row)->index() );
}

bool ProgramsModel::ProgramToStream( QDataStream &stream, const QModelIndex &progIndex) const
{
    stream << progIndex.data().toString();

    int progId = progIndex.data(UserRoles::value).toInt();
    myHost->programContainer->ProgramToStream( progId, stream );
    myHost->mainWindow->mySceneView->viewProgram->ProgramToStream( progId, stream );

    return true;
}

bool ProgramsModel::RemoveProgram( int row, int groupNum )
{
    if(!item(groupNum) || !item(groupNum)->child(0)) {
        debug2(<<"ProgramsModel::RemoveProgram can't remove"<<groupNum<<row)
        return false;
    }
    return removeRow(row, item(groupNum)->child(0)->index() );
}

bool ProgramsModel::RemoveGroup( int row )
{
    if(!item(row)) {
        debug2(<<"ProgramsModel::RemoveGroup can't remove"<<row)
        return false;
    }
    return removeRow(row);
}

bool ProgramsModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    switch(action) {
    case Qt::CopyAction:
        currentCommand = new QUndoCommand(tr("Copy programs"));
        break;
    case Qt::MoveAction:
        currentCommand = new QUndoCommand(tr("Move programs"));
        break;
    default:
        debug2(<<"ProgramsModel::dropMimeData unsupported action"<<action)
        return false;
    }

    int countRows=0;

    if(parent.isValid()) {
        if(data->hasFormat("application/x-programsdata")) {
            if(row==-1)
                row=itemFromIndex(parent)->rowCount();
            QDataStream stream( &data->data( "application/x-programsdata" ), QIODevice::ReadOnly);
            while(!stream.atEnd()) {
                QByteArray tmpBa;
                stream >> tmpBa;
                new ComAddProgram( this, &tmpBa, row+countRows, parent.parent().row() , currentCommand );
                ++countRows;
            }
        }
    } else {
        if(data->hasFormat("application/x-groupsdata")) {
            if(row==-1)
                row=rowCount();
            QDataStream stream( &data->data( "application/x-groupsdata" ), QIODevice::ReadOnly);
            while(!stream.atEnd()) {
                QByteArray tmpBa;
                stream >> tmpBa;
                new ComAddGroup( this, &tmpBa, row+countRows, currentCommand );
                ++countRows;
            }
        }
    }

    switch(action) {
    case Qt::CopyAction :
        myHost->undoStack.push( currentCommand );
        currentCommand = 0;
        droppedItemsCount = 0;
        break;
    case Qt::MoveAction :
        droppedItemsCount = countRows;
        break;
    }

    return true;
}

bool ProgramsModel::removeRows ( int row, int count, const QModelIndex & parent )
{
    if(fromCom)
        return QStandardItemModel::removeRows(row,count,parent);

    if(currentCommand) {
        droppedItemsCount-=count;

        while(count>0) {
            if(parent.isValid()) {
                if( myHost->programList->RemoveIndex( parent.child(row+count-1,0) ) )
                    new ComRemoveProgram( this, row+count-1, parent.parent().row(), currentCommand);
            } else {
                if( myHost->programList->RemoveIndex(index(row+count-1,0)) )
                    new ComRemoveGroup( this, row+count-1, currentCommand);
            }
            --count;
        }

        if(droppedItemsCount==0) {
            myHost->undoStack.push( currentCommand );
            currentCommand=0;
        }
        return true;
    }

    debug2(<<"ProgramsModel::removeRows remove row with no undoCommand")
    return QStandardItemModel::removeRows(row,count,parent);
}

void ProgramsModel::removeRows ( QModelIndexList &listToRemove, const QModelIndex & parent )
{
    currentCommand = new QUndoCommand(tr("Remove programs"));
    droppedItemsCount = listToRemove.size();

    qSort(listToRemove.begin(),listToRemove.end(),qGreater<QModelIndex>());

    foreach(QModelIndex index, listToRemove) {
        removeRow( index.row(), parent);
    }
}

bool ProgramsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!fromCom && role == Qt::EditRole) {
        int groupNum=-1;
        if(index.parent().isValid() && index.parent().parent().isValid())
            groupNum=index.parent().parent().row();

        if(index.data(role)==value)
            return true;

        myHost->undoStack.push( new ComChangeProgramItem( this, index.row(), groupNum, index.data(role), value, role ) );
        return true;
    }

    return QStandardItemModel::setData(index,value,role);
}

QStringList ProgramsModel::mimeTypes () const
{
    QStringList types;
    types << "application/x-programsdata";
    types << "application/x-groupsdata";
    return types;

}

QMimeData * ProgramsModel::mimeData ( const QModelIndexList & indexes ) const
{
    QByteArray groups;
    QDataStream streamGroup( &groups, QIODevice::WriteOnly);
    QByteArray programs;
    QDataStream streamProg( &programs, QIODevice::WriteOnly);

    foreach(QModelIndex index, indexes) {
        if(index.data(UserRoles::nodeType).toInt() == NodeType::programGroup) {
            QByteArray tmpBa;
            QDataStream tmpStream(&tmpBa, QIODevice::WriteOnly);
            GroupToStream(tmpStream,index);
            streamGroup << tmpBa;
        }
        if(index.data(UserRoles::nodeType).toInt() == NodeType::program) {
            QByteArray tmpBa;
            QDataStream tmpStream(&tmpBa, QIODevice::WriteOnly);
            ProgramToStream(tmpStream,index);
            streamProg << tmpBa;
        }
    }
    QMimeData *mime = new QMimeData();
    if(!groups.isEmpty())
        mime->setData("application/x-groupsdata", groups);
    if(!programs.isEmpty())
        mime->setData("application/x-programsdata", programs);
    return mime;
}


