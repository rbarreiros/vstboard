#ifndef COMMOVEPROGRAM_H
#define COMMOVEPROGRAM_H

#include <QUndoCommand>

class MainHost;
class ComMoveProgram : public QUndoCommand
{
public:
    ComMoveProgram(MainHost *myHost,
                   const QList< QPair<QPersistentModelIndex,QPersistentModelIndex> > &listMovedIndex,
                   QUndoCommand  *parent=0);

    void undo ();
    void redo ();

private:
    MainHost *myHost;
    bool firstMove;

    int groupOri;
    int groupDest;
    QList< QPair<int,int> >listMoves;
    QList<int>listOri;
    QList<int>listDest;

};

#endif // COMMOVEPROGRAM_H
