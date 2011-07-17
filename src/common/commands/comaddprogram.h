#ifndef COMADDPROGRAM_H
#define COMADDPROGRAM_H

#include <QUndoCommand>

class ProgramsModel;
class ComAddProgram : public QUndoCommand
{
public:
    ComAddProgram(ProgramsModel *model,
                  int groupNum,
                  int row=0,
                  QByteArray *data=0,
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

#endif // COMADDPROGRAM_H
