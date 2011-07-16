#ifndef COMADDPROGRAM_H
#define COMADDPROGRAM_H

#include <QUndoCommand>

class ProgramsModel;
class ComAddProgram : public QUndoCommand
{
public:
    ComAddProgram(ProgramsModel *model,
                  QByteArray *data,
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
};

#endif // COMADDPROGRAM_H
