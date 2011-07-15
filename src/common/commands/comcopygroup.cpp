#include "comcopygroup.h"
#include "mainhost.h"
#include "mainwindow.h"

ComCopyGroup::ComCopyGroup(MainHost *myHost,
                           const QList<int> &grpIds,
                           int destRow,
                           QUndoCommand  *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    grpIds(grpIds),
    destRow(destRow)
{
    setText(QObject::tr("Copy group"));

    qSort(this->grpIds);

    //generate a list of new program ids
    for(int i=0; i<grpIds.size(); i++) {
        newGrpIds << myHost->programList->GetNextGroupId();
    }
}

void ComCopyGroup::undo()
{
    ProgramsModel *model = myHost->programList->GetModel();

    QModelIndex index;
    for(int i=0; i<grpIds.size(); i++) {
        myHost->programList->RemoveIndex( model->index(destRow+i,0) );
    }

    model->fromCom=true;
    for(int i=grpIds.size()-1; i>=0; i--) {
        model->removeRows( destRow+i, 1 );
    }
    model->fromCom=false;
}

void ComCopyGroup::redo()
{
    ProgramsModel *model = myHost->programList->GetModel();

    if(destRow==-1)
        destRow=model->rowCount();

    if( myHost->programList->GetCurrentMidiGroup() >= destRow && myHost->programList->GetCurrentMidiGroup() < destRow+grpIds.size() ) {
        //go to another program
        int i=0;
        while(i<model->rowCount() &&  i>=destRow && i<destRow+grpIds.size()) {
            i++;
        }
        myHost->programList->ChangeGroup(i);
    }

    {
        QDataStream stream(&grpData, QIODevice::WriteOnly);
        for(int i=0; i<grpIds.size(); i++) {
            myHost->groupContainer->ProgramToStream(grpIds.at(i), stream);
            myHost->mainWindow->mySceneView->viewGroup->ProgramToStream(grpIds.at(i), stream);
        }
    }

    {
        QDataStream stream(&grpData, QIODevice::ReadOnly);
        for(int i=0; i<grpIds.size(); i++) {
            QStandardItem *grpItem = new QStandardItem( "cpy" );
            grpItem->setData(NodeType::programGroup,UserRoles::nodeType);
            grpItem->setData(newGrpIds.at(i),UserRoles::value);
            grpItem->setData(0,UserRoles::type);
            grpItem->setDragEnabled(true);
            grpItem->setDropEnabled(false);
            grpItem->setEditable(true);

            QStandardItem *prgList = new QStandardItem();
            prgList->setDragEnabled(false);
            prgList->setDropEnabled(true);
            prgList->setEditable(false);

            grpItem->appendRow(prgList);

            model->fromCom=true;
            model->insertRow( destRow+i , grpItem );
            model->fromCom=false;

            myHost->groupContainer->ProgramFromStream(newGrpIds.at(i),stream);
            myHost->mainWindow->mySceneView->viewGroup->ProgramFromStream(newGrpIds.at(i), stream);
        }
    }

    grpData.resize(0);
}
