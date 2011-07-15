#ifndef COMADDPROGRAM_H
#define COMADDPROGRAM_H

#include <QUndoCommand>

class MainHost;
class ComAddProgram : public QUndoCommand
{
public:
    ComAddProgram(MainHost *myHost,
                  int row,
                  int count,
                  const QModelIndex & parentIndex,
                  QUndoCommand  *parent=0);
    void undo ();
    void redo ();

private:
    MainHost *myHost;
    int row;
    int count;
    int groupNum;
    int progNum;

    QList<int> progIds;
    QStringList progNames;
    QByteArray progData;
};

#endif // COMADDPROGRAM_H
