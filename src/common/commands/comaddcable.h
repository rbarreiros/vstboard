#ifndef COMCONNECTPIN_H
#define COMCONNECTPIN_H

#include <QUndoCommand>
#include "connectables/connectioninfo.h"

class MainHost;
class ComAddCable : public QUndoCommand
{
public:
    ComAddCable(MainHost *myHost,
                const ConnectionInfo &outInfo,
                const ConnectionInfo &inInfo,
                QUndoCommand  *parent=0);
    void undo ();
    void redo ();

private:
    MainHost *myHost;
    ConnectionInfo outInfo;
    ConnectionInfo inInfo;
};

#endif // COMCONNECTPIN_H
