#include "comchangeprogram.h"
#include "mainhost.h"

ComChangeProgram::ComChangeProgram(MainHost *myHost,
                                   const QModelIndex &newProgIndex,
                                   QUndoCommand *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    oldGroup(-1),
    oldProg(-1),
    newGroup(-1),
    newProg(-1)
{
    if(!newProgIndex.isValid())
        return;

    setText( QObject::tr("Prog:%1").arg( newProgIndex.data().toString() ));
    oldGroup = myHost->programList->GetCurrentMidiGroup();
    oldProg = myHost->programList->GetCurrentMidiProg();
    newGroup = newProgIndex.parent().parent().row();
    newProg = newProgIndex.row();
}

void ComChangeProgram::undo()
{
    if(oldProg==-1)
        return;
    myHost->programList->ChangeProgNow(oldGroup,oldProg);
}

void ComChangeProgram::redo()
{
    if(newProg==-1)
        return;
    myHost->programList->ChangeProgNow(newGroup,newProg);
}
