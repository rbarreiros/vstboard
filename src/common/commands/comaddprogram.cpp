#include "comaddprogram.h"
#include "models/programsmodel.h"

ComAddProgram::ComAddProgram(ProgramsModel *model,
                             int groupNum,
                             int row,
                             QByteArray *data,
                             QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    row(row),
    groupNum(groupNum),
    done(false)

{
    setText(QObject::tr("Add program"));

    if(data)
        this->data=*data;

    done=true;
    redo();
    done=false;
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
    if(!done) {
        done=true;
        return;
    }

    model->fromCom=true;
    QDataStream stream(&data, QIODevice::ReadOnly);
    model->ProgramFromStream(stream,row,groupNum);
    model->fromCom=false;
}
