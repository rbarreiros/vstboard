#ifndef COMREMOVEPROGRAM_H
#define COMREMOVEPROGRAM_H

#include <QUndoCommand>

class ProgramsModel;
class ComRemoveProgram : public QUndoCommand
{
public:
    ComRemoveProgram(ProgramsModel *model,
                     int row,
                     int groupNum,
                     QUndoCommand  *parent=0);
    void undo();
    void redo();

private:
    ProgramsModel *model;
    int row;
    int groupNum;
    QByteArray data;
    bool done;
};

#endif // COMREMOVEPROGRAM_H
