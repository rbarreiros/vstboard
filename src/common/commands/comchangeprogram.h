#ifndef COMCHANGEPROGRAM_H
#define COMCHANGEPROGRAM_H

#include "precomp.h"
#include <QUndoCommand>

class MainHost;
class ComChangeProgram : public QUndoCommand
{
public:
    ComChangeProgram(MainHost *myHost,
                     const QModelIndex &oldProg,
                     const QModelIndex &newProg,
                     QUndoCommand  *parent=0);
    void undo ();
    void redo ();
private:
    MainHost *myHost;

    QPersistentModelIndex oldProg;
    QPersistentModelIndex newProg;
};

#endif // COMCHANGEPROGRAM_H
