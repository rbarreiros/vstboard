#include "comaddprogram.h"
#include "mainhost.h"

ComAddProgram::ComAddProgram(MainHost *myHost,
                             int row,
                             int count,
                             const QModelIndex & parentIndex,
                             QUndoCommand  *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    row(row),
    count(count)
{
    setText(QObject::tr("Add program"));

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

void ComAddProgram::undo ()
{
    ProgramsModel *model = myHost->programList->GetModel();
    QStandardItem *lstProg = model->item(groupNum)->child(0,0);
    model->fromCom=true;
    lstProg->removeRows(row,count);
    model->fromCom=false;
}

void ComAddProgram::redo ()
{
    ProgramsModel *model = myHost->programList->GetModel();
    QStandardItem *lstProg = model->item(groupNum)->child(0,0);
    model->fromCom=true;
    lstProg->insertRows(row,count);
    model->fromCom=false;
}
