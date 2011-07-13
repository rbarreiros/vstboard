#include "comremoveprogram.h"
#include "mainhost.h"

ComRemoveProgram::ComRemoveProgram(MainHost *myHost,
                                   int row,
                                   int count,
                                   const QModelIndex &parentIndex,
                                   QUndoCommand *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    row(row),
    count(count)
{
    setText(QObject::tr("Remove program"));

    if(parentIndex.isValid()) {
        //removing a program
        groupNum = parentIndex.parent().row();
        progNum = row;
    } else {
        //removing a group
        progNum = -1;
        groupNum = row;
    }
}

void ComRemoveProgram::undo()
{
    ProgramsModel *model = myHost->programList->GetModel();
    model->fromCom=true;
    if(progNum==-1)
        model->insertRows( groupNum, count );
    else {
        QStandardItem *parentItem = model->item(groupNum)->child(0);
        QDataStream stream(&progData, QIODevice::ReadWrite);

        for(int i=0; i<count; i++) {
            QStandardItem *newItem = new QStandardItem( progNames.at(i) );
            int progId = progIds.at(i);
            newItem->setData(progId, UserRoles::value);
            parentItem->insertRow( progNum+i, newItem );
            myHost->programContainer->ProgramFromStream(progId,stream);
        }

    }
    model->fromCom=false;

    progData.resize(0);
    progIds.clear();
    progNames.clear();
}

void ComRemoveProgram::redo()
{
    ProgramsModel *model = myHost->programList->GetModel();

    if(progNum==-1)
        model->removeRows( groupNum, count );
    else {

        bool currentProgRemoved=false;
        int currentProg = myHost->programList->GetCurrentMidiProg();

        //check if the current prog will be removed
        if( myHost->programList->GetCurrentMidiGroup() == groupNum ) {
            if(currentProg>=row && currentProg<=row+count) {
                currentProgRemoved=true;
            }
        }

        //save the program
        QModelIndex index;
        QDataStream stream(&progData, QIODevice::ReadWrite);
        for(int i=0; i<count; i++) {
            if(! myHost->programList->GetIndexFromProgNum(groupNum,progNum+i, index) )
                    continue;
            progNames << index.data(Qt::DisplayRole).toString();
            int progId = index.data(UserRoles::value).toInt();
            progIds << progId;
            myHost->programContainer->ProgramToStream(progId, stream);
        }

        //remove rows
        model->fromCom=true;
        model->removeRows( progNum, count, index.parent() );
        model->fromCom=false;

        //the current prog has been removed, change to the next one
        if(currentProgRemoved)
            myHost->programList->ChangeProg( currentProg );
    }

}
