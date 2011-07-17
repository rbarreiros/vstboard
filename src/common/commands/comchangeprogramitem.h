#ifndef COMRENAMEPROGRAM_H
#define COMRENAMEPROGRAM_H

#include <QUndoCommand>

class ProgramsModel;
class ComChangeProgramItem : public QUndoCommand
{
public:
    ComChangeProgramItem(ProgramsModel *model,
                     int row,
                     int groupNum,
                     const QVariant &oldValue,
                     const QVariant &newValue,
                     int role,
                     QUndoCommand  *parent=0);
    void undo ();
    void redo ();

private:
    ProgramsModel *model;
    QVariant oldValue;
    QVariant newValue;
    int role;
    int groupNum;
    int row;
};

#endif // COMRENAMEPROGRAM_H
