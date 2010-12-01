#include "programsmodel.h"
#include "globals.h"
#include "mainhost.h"

ProgramsModel::ProgramsModel(QObject *parent) :
    QStandardItemModel(parent),
    movingItems(false)
{
}

bool ProgramsModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    switch(action) {
        case Qt::CopyAction : {
            QStandardItemModel mod;
            mod.dropMimeData(data,action,0,0,QModelIndex());
            QStandardItem *i = mod.invisibleRootItem()->child(0);

            if(i->data(UserRoles::nodeType).toInt() == NodeType::program) {
                QStandardItem *cpy = MainHost::Get()->programList->CopyProgram(i);
                QStandardItem *par = itemFromIndex(parent);
                if(row==-1)
                    par->appendRow(cpy);
                else
                    par->insertRow(row,cpy);
                return false;
            }

            if(i->data(UserRoles::nodeType).toInt() == NodeType::programGroup) {
                QStandardItem *cpy = MainHost::Get()->programList->CopyGroup(i);
                if(row==-1)
                    appendRow(cpy);
                else
                    invisibleRootItem()->insertRow(row,cpy);

                return false;
            }
            break;
        }
        case Qt::MoveAction : {
                //note that we're moving items : remove items but don't delete the associated programs
            movingItems=true;
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
    if(movingItems) {
        movingItems=false;
    } else {
        QModelIndex idx = index(row,0,parent);
        MainHost::Get()->programList->RemoveIndex(idx);
    }
    return QStandardItemModel::removeRows(row,count,parent);
}
