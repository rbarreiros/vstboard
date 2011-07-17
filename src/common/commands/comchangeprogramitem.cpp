#include "comchangeprogramitem.h"
#include "models/programsmodel.h"

ComChangeProgramItem::ComChangeProgramItem(ProgramsModel *model,
                                   int row,
                                   int groupNum,
                                   const QVariant &oldValue,
                                   const QVariant &newValue,
                                   int role,
                                   QUndoCommand  *parent):
    QUndoCommand(parent),
    model(model),
    row(row),
    oldValue(oldValue),
    newValue(newValue),
    role(role),
    groupNum(groupNum)
{
    if(role == Qt::DisplayRole || role == Qt::EditRole)
        setText(QObject::tr("Rename program"));
    else
        setText(QObject::tr("Modif program"));
}

void ComChangeProgramItem::undo()
{
    model->fromCom=true;

    if(groupNum==-1)
        model->item(row)->setData(oldValue,role);
    else
        model->item(groupNum)->child(0)->child(row)->setData(oldValue,role);

    model->fromCom=false;
}

void ComChangeProgramItem::redo()
{
    model->fromCom=true;

    if(groupNum==-1)
        model->item(row)->setData(newValue,role);
    else
        model->item(groupNum)->child(0)->child(row)->setData(newValue,role);

    model->fromCom=false;
}
