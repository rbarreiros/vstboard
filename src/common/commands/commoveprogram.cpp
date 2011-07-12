#include "commoveprogram.h"
#include "mainhost.h"

ComMoveProgram::ComMoveProgram(MainHost *myHost,
                                const QList< QPair<QPersistentModelIndex,QPersistentModelIndex> > &listMovedIndex,
                               QUndoCommand  *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    firstMove(true)
{
    setText(QObject::tr("Move programs"));

    QPair<QPersistentModelIndex,QPersistentModelIndex>pair;
    foreach(pair,listMovedIndex) {
        int ori = pair.first.row();
        int dest = pair.second.row();
        listMoves << QPair<int,int>(ori, dest);
        listOri << ori;
        listDest << dest;
    }

    groupOri = listMovedIndex.first().first.parent().parent().row();
    groupDest = listMovedIndex.first().second.parent().parent().row();
}

void ComMoveProgram::undo()
{
    qSort(listOri);
    qSort(listDest.begin(), listDest.end(), qGreater<int>());

    QStandardItemModel *model = myHost->programList->GetModel();

    QStandardItem *lstProgOri = model->invisibleRootItem();
    if(groupOri!=-1)
        lstProgOri = model->item(groupOri)->child(0,0);


    QStandardItem *lstProgDest = model->invisibleRootItem();
    if(groupDest!=-1)
        lstProgDest = model->item(groupDest)->child(0,0);

    foreach(int i, listOri) {
        lstProgOri->insertRows(i,1);
    }

    QPair<int,int>pair;
    foreach(pair, listMoves) {
        lstProgOri->setChild( pair.first, lstProgDest->takeChild( pair.second ) );
    }

    foreach(int i, listDest) {
        lstProgDest->removeRow(i);
    }
}

void ComMoveProgram::redo()
{
    qSort(listDest);
    qSort(listOri.begin(), listOri.end(), qGreater<int>());

    QStandardItemModel *model = myHost->programList->GetModel();

    QStandardItem *lstProgOri = model->invisibleRootItem();
    if(groupOri!=-1)
        lstProgOri = model->item(groupOri)->child(0,0);


    QStandardItem *lstProgDest = model->invisibleRootItem();
    if(groupDest!=-1)
        lstProgDest = model->item(groupDest)->child(0,0);

    if(!firstMove) {
        foreach(int i, listDest) {
            lstProgDest->insertRows(i,1);
        }

        QPair<int,int>pair;
        foreach(pair, listMoves) {
            lstProgDest->setChild( pair.second, lstProgOri->takeChild( pair.first ) );
        }
    }

    foreach(int i, listOri) {
        lstProgOri->removeRow(i);
    }

    firstMove=false;
}



