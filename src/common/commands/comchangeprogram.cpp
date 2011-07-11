#include "comchangeprogram.h"
#include "mainhost.h"

ComChangeProgram::ComChangeProgram(MainHost *myHost,
                                   const QModelIndex &oldProg,
                                   const QModelIndex &newProg,
                                   QUndoCommand *parent) :
    QUndoCommand(parent),
    myHost(myHost),
    oldProg(oldProg),
    newProg(newProg)
{
    setText(QObject::tr("Load program"));
}

void ComChangeProgram::undo()
{
    myHost->programList->ChangeProg(oldProg);
}

void ComChangeProgram::redo()
{
    myHost->programList->ChangeProg(newProg);
}
