#ifndef COMADDGROUP_H
#define COMADDGROUP_H

#include <QUndoCommand>

class ProgramsModel;
class ComAddGroup : public QUndoCommand
{
public:
    ComAddGroup(ProgramsModel *model,
                QByteArray *data,
                int row,
                QUndoCommand *parent=0);
    void undo();
    void redo();

private:
    ProgramsModel *model;
    int row;
    QByteArray data;
    bool done;
};

#endif // COMADDGROUP_H
