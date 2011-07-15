#ifndef COMCOPYPROGRAM_H
#define COMCOPYPROGRAM_H

#include <QUndoCommand>

class MainHost;
class ComCopyProgram : public QUndoCommand
{
public:
    ComCopyProgram(MainHost *myHost,
                   const QList<int> &progIds,
                   const QModelIndex &destIndex,
                   int destRow,
                   QUndoCommand  *parent=0);

    void undo ();
    void redo ();

private:
    MainHost *myHost;
    QList<int> progIds;
    int destRow;
    int destGroup;
    QByteArray progData;
};

#endif // COMCOPYPROGRAM_H
