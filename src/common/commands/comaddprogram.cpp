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
    QDataStream stream(&data, QIODevice::WriteOnly);
    model->ProgramToStream(stream, model->index(groupNum,0).child(row,0) );

    model->fromCom=true;
    model->removeRow(row, model->index(groupNum,0) );
    model->fromCom=false;
}

void ComAddProgram::redo()
{
    if(!done) {
        done=true;
        return;
    }

    QModelIndex prgIndex;
    if(!model->AddProgram(groupNum,prgIndex,row))
        return;

    QDataStream stream(&data, QIODevice::ReadOnly);
    model->ProgramFromStream(stream,prgIndex);
}
