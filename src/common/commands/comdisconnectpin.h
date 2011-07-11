#ifndef COMDISCONNECTPIN_H
#define COMDISCONNECTPIN_H

#include <QUndoCommand>
#include "connectables/connectioninfo.h"

class MainHost;
class ComDisconnectPin : public QUndoCommand
{
public:
    ComDisconnectPin(MainHost *myHost,
                     const ConnectionInfo &pinInfo,
                     QUndoCommand  *parent=0);
    void undo ();
    void redo ();

private:
    MainHost *myHost;
    ConnectionInfo pinInfo;
    QList<ConnectionInfo>listConnectedPins;
};

#endif // COMDISCONNECTPIN_H
