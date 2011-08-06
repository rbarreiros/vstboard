#ifndef COMADDOBJECT_H
#define COMADDOBJECT_H

#include "precomp.h"
#include <QUndoCommand>
#include "connectables/container.h"

class MainHost;
class ComAddObject : public QUndoCommand
{
public:
    ComAddObject(MainHost *myHost,
                 const ObjectInfo & objInfo,
                 const QSharedPointer<Connectables::Container> &container,
                 const QSharedPointer<Connectables::Object> &targetObj,
                 InsertionType::Enum insertType = InsertionType::NoInsertion,
                 QUndoCommand  *parent=0);
    void undo ();
    void redo ();

    void ReloadObject(const ObjectInfo &info);

private:
    MainHost *myHost;

    ObjectInfo objectInfo;
    ObjectInfo targetInfo;

    QWeakPointer<Connectables::Container>ContainerPtr;
    InsertionType::Enum insertType;

    QList< QPair<ConnectionInfo,ConnectionInfo> >listAddedCables;
    QList< QPair<ConnectionInfo,ConnectionInfo> >listRemovedCables;

    ObjectContainerAttribs attr;
    QByteArray objState;

    ObjectContainerAttribs targetAttr;
    QByteArray targetState;

    int currentGroup;
    int currentProg;
};

#endif // COMADDOBJECT_H
