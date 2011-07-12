#include "comrenameprogram.h"
#include "mainhost.h"

ComRenameProgram::ComRenameProgram(MainHost *myHost,
                                   const QModelIndex &progIndex,
                                   const QString &newName,
                                   QUndoCommand  *parent):
    QUndoCommand(parent),
    myHost(myHost),
    newName(newName)
{
    setText(QObject::tr("Rename program"));

    if(progIndex.parent().isValid()) {
        //renaming a program
        groupNum = progIndex.parent().parent().row();
        progNum = progIndex.row();
    } else {
        //renaming a group
        progNum = -1;
        groupNum = progIndex.row();
    }
    oldName = progIndex.data().toString();

}

void ComRenameProgram::undo()
{
    if(progNum==-1)
        myHost->programList->SetGroupName(groupNum,oldName);
    else
        myHost->programList->SetProgName(groupNum,progNum,oldName);
}

void ComRenameProgram::redo()
{
    if(progNum==-1)
        myHost->programList->SetGroupName(groupNum,newName);
    else
        myHost->programList->SetProgName(groupNum,progNum,newName);
}
