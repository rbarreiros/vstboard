#ifndef COMRENAMEPROGRAM_H
#define COMRENAMEPROGRAM_H

#include <QUndoCommand>

class MainHost;
class ComRenameProgram : public QUndoCommand
{
public:
    ComRenameProgram(MainHost *myHost,
                     const QModelIndex &progIndex,
                     const QString &newName,
                     QUndoCommand  *parent=0);
    void undo ();
    void redo ();

private:
    void SetName( const QString &name);
    MainHost *myHost;
    QString oldName;
    QString newName;
    int groupNum;
    int progNum;
};

#endif // COMRENAMEPROGRAM_H
