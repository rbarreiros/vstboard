#include "comremovegroup.h"
#include "models/programsmodel.h"

ComRemoveGroup::ComRemoveGroup(ProgramsModel *model,
                               int row,
                               QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    row(row)
{
    setText(QObject::tr("Remove group"));
}

void ComRemoveGroup::undo()
{
    model->fromCom=true;

    QDataStream stream(&data, QIODevice::ReadOnly);
    model->GroupFromStream(stream,row);

    model->fromCom=false;
}

void ComRemoveGroup::redo()
{
    model->fromCom=true;

    QDataStream stream(&data, QIODevice::WriteOnly);
    model->GroupToStream(stream,row);
    model->RemoveGroup(row);

    model->fromCom=false;
}
