#include "comremovegroup.h"
#include "models/programsmodel.h"

ComRemoveGroup::ComRemoveGroup(ProgramsModel *model,
                               int row,
                               QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    row(row),
    done(false)
{
    setText(QObject::tr("Remove group"));

    done=true;
    redo();
    done=false;
}

void ComRemoveGroup::undo()
{
    QModelIndex grpIndex;
    if(!model->AddGroup(grpIndex, row))
        return;

    QDataStream stream(&data, QIODevice::ReadOnly);
    model->GroupFromStreamWithPrograms(stream, grpIndex );
}

void ComRemoveGroup::redo()
{
    if(!done) {
        done=true;
        return;
    }

    QDataStream stream(&data, QIODevice::WriteOnly);
    model->GroupToStreamWithPrograms(stream, model->index(row,0) );

    model->fromCom=true;
    model->removeRow(row);
    model->fromCom=false;
}
