#include "comcopyprogram.h"
#include "mainhost.h"
#include "mainwindow.h"

ComCopyProgram::ComCopyProgram(MainHost *myHost,
                               const QList<int> &progIds,
                               int destGroupRow,
                               int destRow,
                               QUndoCommand  *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    progIds(progIds),
    destGroupRow(destGroupRow),
    destRow(destRow)
{
    setText(QObject::tr("Copy porgram"));

    qSort(this->progIds);

    //generate a list of new program ids
    for(int i=0; i<progIds.size(); i++) {
        newProgIds << myHost->programList->GetNextProgId();
    }
}

void ComCopyProgram::undo()
{
    ProgramsModel *model = myHost->programList->GetModel();
    QStandardItem *progList = model->item(destGroupRow)->child(0);

    if(myHost->programList->GetCurrentMidiGroup()==destGroupRow) {
        if(  myHost->programList->GetCurrentMidiProg() >= destRow ||  myHost->programList->GetCurrentMidiProg() < destRow+progIds.size()) {
            //go to another program
            if(progIds.size() == progList->rowCount()) {
                //find another group
                int i=0;
                while(i<model->rowCount() && i==destGroupRow) {
                    i++;
                }
                if(i==model->rowCount()) {
                    progIds.clear();
                    return;
                }
                myHost->programList->ChangeGroup(i);
            } else {
                int i=0;
                while(i<progList->rowCount() && i>=destRow && i<destRow+progIds.size()) {
                    i++;
                }
                if(i!=progList->rowCount())
                    myHost->programList->ChangeProg(i);
            }
        }
    }

    QModelIndex index;
    for(int i=0; i<progIds.size(); i++) {
        myHost->programList->GetIndexFromProgNum(destGroupRow,destRow+i, index);
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
    QStandardItem *progList = model->item(destGroupRow)->child(0);

    if(destRow==-1)
        destRow=progList->rowCount();

    {
        QDataStream stream(&progData, QIODevice::WriteOnly);
        for(int i=0; i<progIds.size(); i++) {
            myHost->programContainer->ProgramToStream(progIds.at(i), stream);
            myHost->mainWindow->mySceneView->viewProgram->ProgramToStream(progIds.at(i), stream);
        }

    }

    {
        QDataStream stream(&progData, QIODevice::ReadOnly);
        for(int i=0; i<progIds.size(); i++) {
            QStandardItem *prgItem = new QStandardItem( "cpy" );
            prgItem->setData(newProgIds.at(i), UserRoles::value);
            prgItem->setData(NodeType::program, UserRoles::nodeType);
            prgItem->setData(0,UserRoles::type);
            prgItem->setDragEnabled(true);
            prgItem->setDropEnabled(false);
            prgItem->setEditable(true);

            model->fromCom=true;
            progList->insertRow( destRow+i , prgItem );
            model->fromCom=false;

            myHost->programContainer->ProgramFromStream(newProgIds.at(i),stream);
            myHost->mainWindow->mySceneView->viewProgram->ProgramFromStream(newProgIds.at(i), stream);
        }
    }

    progData.resize(0);
}
