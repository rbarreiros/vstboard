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

private:
    MainHost *myHost;

    ObjectInfo objectInfo;
    ObjectInfo targetInfo;
    QWeakPointer<Connectables::Container>ContainerPtr;
    InsertionType::Enum insertType;
    ObjectContainerAttribs attr;

    QList< QPair<ConnectionInfo,ConnectionInfo> >listAddedCables;
    QList< QPair<ConnectionInfo,ConnectionInfo> >listRemovedCables;

//    QByteArray objState;
};

#endif // COMADDOBJECT_H
