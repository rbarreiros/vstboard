#include "comaddgroup.h"
#include "models/programsmodel.h"

ComAddGroup::ComAddGroup(ProgramsModel *model,
                         QByteArray *data,
                         int row,
                         QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    row(row),
    data(*data),
    done(false)

{
    setText(QObject::tr("Add group"));

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
