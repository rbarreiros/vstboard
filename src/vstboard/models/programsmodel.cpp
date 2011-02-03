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

ProgramsModel::ProgramsModel(MainHost *parent) :
    QStandardItemModel(parent),
    movingItems(0),
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
            //note that we're moving items : remove items but don't delete the associated programs
            QStandardItemModel mod;
            mod.dropMimeData(data,action,0,0,QModelIndex());
            movingItems=mod.rowCount();
            return QStandardItemModel::dropMimeData(data, action,row, column, parent);
            break;
        }
        default : {
            return QStandardItemModel::dropMimeData(data, action,row, column, parent);
        }
    }
    return false;
}

bool ProgramsModel::removeRows ( int row, int count, const QModelIndex & parent )
{
    if(movingItems>0) {
        movingItems--;
    } else {
        QModelIndex idx = index(row,0,parent);
        if(!myHost->programList->RemoveIndex(idx))
            return false;
    }
    return QStandardItemModel::removeRows(row,count,parent);
}

