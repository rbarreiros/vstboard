#include "comaddgroup.h"
#include "models/programsmodel.h"

ComAddGroup::ComAddGroup(ProgramsModel *model,
                         int row,
                         QByteArray *data,
                         QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    row(row),
    done(false)

{
    setText(QObject::tr("Add group"));

    if(data)
        this->data=*data;

    done=true;
    redo();
    done=false;
}

void ComAddGroup::undo()
{
    QDataStream stream(&data, QIODevice::WriteOnly);
    model->GroupToStreamWithPrograms(stream, model->index(row,0) );

    model->fromCom=true;
    model->removeRow(row);
    model->fromCom=false;
}

void ComAddGroup::redo()
{
    if(!done) {
        done=true;
        return;
    }

    QModelIndex grpIndex;
    if(!model->AddGroup(grpIndex, row))
        return;

    QDataStream stream(&data, QIODevice::ReadOnly);
    model->GroupFromStreamWithPrograms(stream,grpIndex);
}
