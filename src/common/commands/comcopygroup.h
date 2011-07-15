#ifndef COMCOPYGROUP_H
#define COMCOPYGROUP_H

#include <QUndoCommand>

class MainHost;
class ComCopyGroup : public QUndoCommand
{
public:
    ComCopyGroup(MainHost *myHost,
                 const QList<int> &grpIds,
                  int destRow,
                 QUndoCommand  *parent=0);

  void undo ();
  void redo ();

private:
  MainHost *myHost;
  QList<int> grpIds;
  QList<int> newGrpIds;
  int destRow;
  QByteArray grpData;
};

#endif // COMCOPYGROUP_H
