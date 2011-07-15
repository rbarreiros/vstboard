#ifndef COMREMOVEPROGRAM_H
#define COMREMOVEPROGRAM_H

#include <QUndoCommand>

class MainHost;
class ComRemoveProgram : public QUndoCommand
{
public:
    ComRemoveProgram(MainHost *myHost,
                     const QModelIndexList &listToRemove,
                     const QModelIndex &parentIndex,
                     QUndoCommand  *parent=0);
    void undo();
    void redo();

private:
    MainHost *myHost;
    QList<int> rows;
    QList<int> progIds;
    QStringList progNames;
    QByteArray progData;

    int currentGroup;
};

#endif // COMREMOVEPROGRAM_H
