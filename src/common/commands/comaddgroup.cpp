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
    model->fromCom=true;
    QDataStream stream(&data, QIODevice::WriteOnly);
    model->GroupToStream(stream,row);
    model->removeRow(row);
    model->fromCom=false;
}

void ComAddGroup::redo()
{
    if(!done) {
        done=true;
        return;
    }

    model->fromCom=true;
    QDataStream stream(&data, QIODevice::ReadOnly);
    model->GroupFromStream(stream,row);
    model->fromCom=false;
}
