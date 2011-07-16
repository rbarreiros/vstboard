#include "comremoveprogram.h"
#include "models/programsmodel.h"

ComRemoveProgram::ComRemoveProgram(ProgramsModel *model,
                                   int row,
                                   int groupNum,
                                   QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    row(row),
    groupNum(groupNum),
    done(false)

{
    setText(QObject::tr("Remove program"));

    done=true;
    redo();
    done=false;
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
    if(!done) {
        done=true;
        return;
    }

    model->fromCom=true;
    QDataStream stream(&data, QIODevice::WriteOnly);
    model->ProgramToStream(stream,row,groupNum);
    model->RemoveProgram(row,groupNum);
    model->fromCom=false;
}
