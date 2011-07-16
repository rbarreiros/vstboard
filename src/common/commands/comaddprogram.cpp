#include "comaddprogram.h"
#include "models/programsmodel.h"

ComAddProgram::ComAddProgram(ProgramsModel *model,
                             QByteArray *data,
                             int row,
                             int groupNum,
                             QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    row(row),
    groupNum(groupNum),
    data(*data)

{
    setText(QObject::tr("Add program"));
}

void ComAddProgram::undo()
{
    model->fromCom=true;

    QDataStream stream(&data, QIODevice::WriteOnly);

    model->ProgramToStream(stream,row,groupNum);
    model->RemoveProgram(row,groupNum);
    
    model->fromCom=false;
}

void ComAddProgram::redo()
{
    model->fromCom=true;

    QDataStream stream(&data, QIODevice::ReadOnly);
    model->ProgramFromStream(stream,row,groupNum);

    model->fromCom=false;
}
