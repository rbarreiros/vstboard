#include "comcopyprogram.h"
#include "mainhost.h"

ComCopyProgram::ComCopyProgram(MainHost *myHost,
                               const QList<int> &progIds,
                               const QModelIndex &destIndex,
                               int destRow,
                               QUndoCommand  *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    progIds(progIds),
    destRow(destRow)
{
    setText(QObject::tr("Copy porgram"));

    destGroup = destIndex.row();
    qSort(this->progIds);
}

void ComCopyProgram::undo()
{
    ProgramsModel *model = myHost->programList->GetModel();
    QStandardItem *progList = model->item(destGroup)->child(0);

    QModelIndex index;
    for(int i=0; i<progIds.size(); i++) {
        myHost->programList->GetIndexFromProgNum(destGroup,destRow+i, index);
        if(index.isValid())
            myHost->programList->RemoveIndex(index);
    }

    model->fromCom=true;
    for(int i=progIds.size()-1; i>=0; i--) {
        model->removeRows( destRow+i, 1, progList->index() );
    }
    model->fromCom=false;
}

void ComCopyProgram::redo()
{
    ProgramsModel *model = myHost->programList->GetModel();
    QStandardItem *progList = model->item(destGroup)->child(0);

    if(destRow==-1)
        destRow=progList->rowCount();

    QDataStream stream(&progData, QIODevice::ReadWrite);
    for(int i=0; i<progIds.size(); i++) {
        myHost->programContainer->ProgramToStream(progIds.at(i), stream);
    }

    for(int i=0; i<progIds.size(); i++) {
        QStandardItem *prgItem = new QStandardItem( "cpy" );
        prgItem->setData(progIds.at(i), UserRoles::value);
        prgItem->setData(NodeType::program, UserRoles::nodeType);
        prgItem->setData(0,UserRoles::type);
        prgItem->setDragEnabled(true);
        prgItem->setDropEnabled(false);
        prgItem->setEditable(true);

        model->fromCom=true;
        progList->insertRow( destRow+i , prgItem );
        model->fromCom=false;

        myHost->programContainer->ProgramFromStream(progIds.at(i),stream);
    }

    progData.resize(0);
}
