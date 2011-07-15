#ifndef COMREMOVEGROUP_H
#define COMREMOVEGROUP_H

#include <QUndoCommand>

class MainHost;
class ComRemoveGroup : public QUndoCommand
{
public:
    ComRemoveGroup(MainHost *myHost,
                   const QModelIndexList &listToRemove,
                   QUndoCommand  *parent=0);
  void undo();
  void redo();

private:
  MainHost *myHost;
  QList<int> rows;
  QList<int> progIds;
  QStringList progNames;
  QByteArray progData;

};

#endif // COMREMOVEGROUP_H
