#include "comchangeprogram.h"
#include "models/programsmodel.h"

ComChangeProgram::ComChangeProgram(ProgramsModel *model,
                                   int oldGroup,
                                   int oldProg,
                                   int newGroup,
                                   int newProg,
                                   QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    oldGroup(oldGroup),
    oldProg(oldProg),
    newGroup(newGroup),
    newProg(newProg)
{
    setText(QObject::tr("Program change"));
}

void ComChangeProgram::undo()
{
    model->ChangeProgNow(oldGroup,oldProg);
}

void ComChangeProgram::redo()
{
    model->ChangeProgNow(newGroup,newProg);
}
