#ifndef COMCHANGEGROUP_H
#define COMCHANGEGROUP_H

#include <QUndoCommand>

class MainHost;
class ComChangeGroup : public QUndoCommand
{
public:
    ComChangeGroup(MainHost *myHost,
                   const QModelIndex &newGroup,
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

#endif // COMCHANGEGROUP_H
