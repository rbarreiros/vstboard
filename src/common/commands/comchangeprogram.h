#ifndef COMCHANGEPROGRAM_H
#define COMCHANGEPROGRAM_H

#include "precomp.h"
#include <QUndoCommand>

class MainHost;
class ComChangeProgram : public QUndoCommand
{
public:
    ComChangeProgram(MainHost *myHost,
                     const QModelIndex &newProgIndex,
                     QUndoCommand  *parent=0);
    void undo ();
    void redo ();
private:
    MainHost *myHost;

    int oldGroup;
    int oldProg;
    int newGroup;
    int newProg;
};

#endif // COMCHANGEPROGRAM_H
