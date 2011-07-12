#include "comchangegroup.h"
#include "mainhost.h"

ComChangeGroup::ComChangeGroup(MainHost *myHost,
                               const QModelIndex &newGroupIndex,
                               QUndoCommand  *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    oldGroup(-1),
    oldProg(-1),
    newGroup(-1),
    newProg(-1)
{
    if(!newGroupIndex.isValid())
        return;

    setText( QObject::tr("Group:%1").arg( newGroupIndex.data().toString() ));
    oldGroup = myHost->programList->GetCurrentMidiGroup();
    oldProg = myHost->programList->GetCurrentMidiProg();
    newGroup = newGroupIndex.row();
    newProg = 0;
}

void ComChangeGroup::undo()
{
    if(oldProg==-1)
        return;
    myHost->programList->ChangeProgNow(oldGroup,oldProg);
}

void ComChangeGroup::redo()
{
    if(newProg==-1)
        return;
    myHost->programList->ChangeProgNow(newGroup,newProg);
}
