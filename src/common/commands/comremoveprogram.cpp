#include "comremoveprogram.h"
#include "mainhost.h"
#include "mainwindow.h"

ComRemoveProgram::ComRemoveProgram(MainHost *myHost,
                                   const QModelIndexList &listToRemove,
                                   const QModelIndex &parentIndex,
                                   QUndoCommand *parent) :
    QUndoCommand(parent),
    myHost(myHost)
{
    setText(QObject::tr("Remove program"));

    currentGroup = parentIndex.parent().row();

    foreach(QModelIndex index, listToRemove) {
        rows << index.row();
    }

    qSort(rows);
}

void ComRemoveProgram::undo()
{
    ProgramsModel *model = myHost->programList->GetModel();

    QStandardItem *progList = model->item(currentGroup)->child(0);
    if(!progList) {
        debug2(<<"ComRemoveProgram::undo progList==0")
        return;
    }
    QDataStream stream(&progData, QIODevice::ReadWrite);

    for(int i=0; i<rows.size(); i++) {
        int progId = progIds.at(i);

        QStandardItem *prgItem = new QStandardItem( progNames.at(i) );
        prgItem->setData(progId, UserRoles::value);
        prgItem->setData(NodeType::program, UserRoles::nodeType);
        prgItem->setData(0,UserRoles::type);
        prgItem->setDragEnabled(true);
        prgItem->setDropEnabled(false);
        prgItem->setEditable(true);

        model->fromCom=true;
        progList->insertRow( rows.at(i), prgItem );
        model->fromCom=false;

        myHost->programContainer->ProgramFromStream(progId,stream);
        myHost->mainWindow->mySceneView->viewProgram->ProgramFromStream(progId, stream);
    }

    progData.resize(0);
    progIds.clear();
    progNames.clear();
}

void ComRemoveProgram::redo()
{
    ProgramsModel *model = myHost->programList->GetModel();
    QStandardItem *progList = model->item(currentGroup)->child(0,0);

    if(myHost->programList->GetCurrentMidiGroup()==currentGroup) {
        if(rows.contains( myHost->programList->GetCurrentMidiProg() )) {
            //go to another program
            if(rows.count() == progList->rowCount()) {
                //find another group
                int i=0;
                while(i<model->rowCount() && i==currentGroup) {
                    i++;
                }
                if(i==model->rowCount()) {
                    rows.clear();
                    return;
                }
                myHost->programList->ChangeGroup(i);
            } else {
                int i=0;
                while(i<progList->rowCount() && !rows.contains(i)) {
                    i++;
                }
                if(i!=progList->rowCount())
                    myHost->programList->ChangeProg(i);
            }
        }
    }

    QModelIndex index;
    QDataStream stream(&progData, QIODevice::ReadWrite);

    for(int i=0; i<rows.size(); i++) {
        if(myHost->programList->GetIndexFromProgNum(currentGroup,rows.at(i), index) ) {
            progNames << index.data(Qt::DisplayRole).toString();
            int progId = index.data(UserRoles::value).toInt();
            progIds << progId;
            myHost->programContainer->ProgramToStream(progId, stream);
            myHost->mainWindow->mySceneView->viewProgram->ProgramToStream(progId, stream);

            if(!myHost->programList->RemoveIndex(index)) {
                rows.removeAt(i);
                i++;
            }
        } else {
            rows.removeAt(i);
            i--;
        }
    }

    model->fromCom=true;
    for(int i=rows.size()-1; i>=0; i--) {
        model->removeRows( rows.at(i), 1, progList->index() );
    }
    model->fromCom=false;
}
