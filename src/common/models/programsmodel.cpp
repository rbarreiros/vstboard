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
#include "commands/commoveprogram.h"
#include "commands/comrenameprogram.h"
#include "commands/comremoveprogram.h"
#include "commands/comremovegroup.h"
#include "commands/comcopyprogram.h"
#include "commands/comcopygroup.h"

ProgramsModel::ProgramsModel(MainHost *parent) :
    QStandardItemModel(parent),
    movingItemCount(0),
    movingItemLeft(0),
    movingDestRow(0),
    myHost(parent),
    fromCom(false)
{
}

bool ProgramsModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    QStandardItemModel tmpModel;
    tmpModel.dropMimeData(data,action,0,0,QModelIndex());

    switch(action) {
        case Qt::CopyAction : {

            //copying programs
            if(tmpModel.item(0)->data(UserRoles::nodeType).toInt() == NodeType::program) {
                QList<int>progIds;
                for(int i=0; i<tmpModel.rowCount(); i++) {
                    progIds<<tmpModel.item(i)->data(UserRoles::value).toInt();
                }
                myHost->undoStack.push( new ComCopyProgram(myHost,progIds,parent.parent().row(),row) );
            }
            //copying groups
            if(tmpModel.item(0)->data(UserRoles::nodeType).toInt() == NodeType::programGroup) {

                QUndoCommand *comm = new QUndoCommand( tr("Copy group") );
                int destGroupRow = row;
                if(destGroupRow==-1)
                    destGroupRow=rowCount();

                //copy the group
                QList<int>grpIds;
                for(int i=0; i<tmpModel.rowCount(); i++) {
                    grpIds<<tmpModel.item(i)->data(UserRoles::value).toInt();
                }
                new ComCopyGroup(myHost,grpIds,destGroupRow,comm);

                //copy the childen
                for(int i=0; i<tmpModel.rowCount(); i++) {
                    QStandardItem *listProgs = tmpModel.item(i)->child(0,0);
                    QList<int> tmpLst;
                    for(int j=0; j<listProgs->rowCount(); j++) {
                        tmpLst << listProgs->child(j,0)->data(UserRoles::value).toInt();
                    }
                    new ComCopyProgram(myHost,tmpLst, destGroupRow+i, 0, comm);
                }

                myHost->undoStack.push( comm );
            }
            break;
        }
        case Qt::MoveAction : {
            //drag&drop to reorder items : the model will delete and recreate new indexes, losing track of persistentmodelindex
            //we need to track the "currently moving items" to prevent their deletion

            //count the number or moved items
            movingItemCount+=tmpModel.rowCount();
            movingItemLeft+=tmpModel.rowCount();

            //get the target row
            movingDestRow=row;

            if(movingDestRow==-1)
                //-1 => items are moved to the end
                if(parent.isValid())
                    movingDestRow=itemFromIndex(parent)->rowCount();
                else
                    //we're moving a group
                    movingDestRow=rowCount();

            //get the targeted parent
            movingToParent=parent;

            QStandardItemModel::dropMimeData(data, action,row, column, parent);
            return true;
        }
        default : {
            return QStandardItemModel::dropMimeData(data, action,row, column, parent);
        }
    }
    return false;
}

bool ProgramsModel::removeRows ( int row, int count, const QModelIndex & parent )
{
    if(fromCom) {
        return QStandardItemModel::removeRows ( row, count, parent );
    }

    //don't delete if the user is reordering with drag&drop
    if(movingItemLeft>0) {
        for(int i=0;i<count;i++) {

            //get the index about to be removed
            QModelIndex oriIdx = index(row+i, 0, parent);
            if( oriIdx.isValid() ) {

                //find the corresponding index in the destination list
                for(int j=0; j<movingItemCount; j++) {
                    QModelIndex destIdx = index(movingDestRow+j, 0, movingToParent);

                    //found it, update persistent index
                    if(destIdx.isValid()
                        && destIdx.data(UserRoles::value) == oriIdx.data(UserRoles::value)
                        && itemFromIndex(oriIdx) != itemFromIndex(destIdx)) {

                        changePersistentIndex( oriIdx , destIdx );
                        listMovedIndex << QPair<QPersistentModelIndex,QPersistentModelIndex>(oriIdx,destIdx);
                        break;
                    }
                }
            } else {
                debug2(<<"ProgramsModel::removeRows invalid oriIdx"<<row<<i<<parent)
            }

            movingItemLeft--;
        }

        //we're done moving items for now
        if(movingItemLeft==0) {
            movingItemCount=0;
            myHost->undoStack.push( new ComMoveProgram(myHost, listMovedIndex) );
            listMovedIndex.clear();
        }
        return true;
    }

    return QStandardItemModel::removeRows(row,count,parent);
}

void ProgramsModel::removeRows ( QModelIndexList &listToRemove, const QModelIndex & parent )
{
    if(parent.isValid()) {
        //keep one program
        if(listToRemove.size() == itemFromIndex(parent)->rowCount())
            listToRemove.takeFirst();
        if(listToRemove.isEmpty())
            return;

        myHost->undoStack.push( new ComRemoveProgram(myHost, listToRemove, parent) );
    } else {
        //keep one group
        if(listToRemove.size() == rowCount())
            listToRemove.takeFirst();
        if(listToRemove.isEmpty())
            return;

        QUndoCommand *comm = new QUndoCommand( tr("Remove group") );
        foreach(QModelIndex index, listToRemove) {
            QStandardItem *listProgs = itemFromIndex(index)->child(0,0);
            QModelIndexList tmpLst;
            for(int i=0; i<listProgs->rowCount(); i++) {
                tmpLst << listProgs->child(i,0)->index();
            }
            new ComRemoveProgram(myHost, tmpLst, listProgs->index(), comm);
        }
        new ComRemoveGroup(myHost, listToRemove, comm);
        myHost->undoStack.push( comm );
    }
}

bool ProgramsModel::insertRows ( int row, int count, const QModelIndex & parent )
{
    if(fromCom || movingItemLeft>0) {
        return QStandardItemModel::insertRows ( row, count, parent );
    }

//    myHost->undoStack.push( new ComAddProgram(myHost,row,count,parent) );
    return true;
}
