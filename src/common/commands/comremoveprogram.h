#ifndef COMREMOVEPROGRAM_H
#define COMREMOVEPROGRAM_H

#include <QUndoCommand>

class MainHost;
class ComRemoveProgram : public QUndoCommand
{
public:
    ComRemoveProgram(MainHost *myHost,
                     int row,
                     int count,
                     const QModelIndex &parentIndex,
                     QUndoCommand  *parent=0);
    void undo();
    void redo();

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

#endif // COMREMOVEPROGRAM_H
