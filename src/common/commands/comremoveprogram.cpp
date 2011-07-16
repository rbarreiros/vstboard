#include "comremoveprogram.h"
#include "models/programsmodel.h"

ComRemoveProgram::ComRemoveProgram(ProgramsModel *model,
                                   int row,
                                   int groupNum,
                                   QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    row(row),
    groupNum(groupNum)

{
    setText(QObject::tr("Remove program"));
}

void ComRemoveProgram::undo()
{
    model->fromCom=true;

    QDataStream stream(&data, QIODevice::ReadOnly);
    model->ProgramFromStream(stream,row,groupNum);

    model->fromCom=false;
}

void ComRemoveProgram::redo()
{
    model->fromCom=true;

    QDataStream stream(&data, QIODevice::WriteOnly);
    model->ProgramToStream(stream,row,groupNum);
    model->RemoveProgram(row,groupNum);

    model->fromCom=false;
}
