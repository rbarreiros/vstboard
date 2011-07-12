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

ProgramsModel::ProgramsModel(MainHost *parent) :
    QStandardItemModel(parent),
    movingItemCount(0),
    movingItemLeft(0),
    movingDestRow(0),
    myHost(parent)
{
}

bool ProgramsModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    switch(action) {
        case Qt::CopyAction : {
            QStandardItemModel mod;
            mod.dropMimeData(data,action,0,0,QModelIndex());

            for(int i=0; i<mod.rowCount(); i++) {
                QStandardItem *it = mod.invisibleRootItem()->child(i);

                if(it->data(UserRoles::nodeType).toInt() == NodeType::program) {
                    QStandardItem *cpy = myHost->programList->CopyProgram(it);
                    QStandardItem *par = itemFromIndex(parent);
                    if(row==-1)
                        par->appendRow(cpy);
                    else
                        par->insertRow(row,cpy);
                }

                if(it->data(UserRoles::nodeType).toInt() == NodeType::programGroup) {
                    QStandardItem *cpy = myHost->programList->CopyGroup(it);
                    if(row==-1)
                        appendRow(cpy);
                    else
                        invisibleRootItem()->insertRow(row,cpy);
                }
            }
            break;
        }
        case Qt::MoveAction : {
            //drag&drop to reorder items : the model will delete and recreate new indexes, losing track of persistentmodelindex
            //we need to track the "currently moving items" to prevent their deletion

            //count the number or moved items
            QStandardItemModel *item = new QStandardItemModel(this);
            item->dropMimeData(data,action,0,0,QModelIndex());
            movingItemCount+=item->rowCount();
            movingItemLeft+=item->rowCount();

            //get the target row
            movingDestRow=row;

            if(movingDestRow==-1)
                //-1 => items are moved to the end
                if(parent.isValid())
                    movingDestRow=itemFromIndex(parent)->rowCount();
                else
                    //we're moving a group, it's parent is not valid
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
    } else {
        QModelIndex idx = index(row,0,parent);
        if(!myHost->programList->RemoveIndex(idx))
            return false;
    }
    return QStandardItemModel::removeRows(row,count,parent);
}
