#ifndef COMREMOVEOBJECT_H
#define COMREMOVEOBJECT_H

#include "precomp.h"
#include <QUndoCommand>
#include "connectables/container.h"


class MainHost;
class ComRemoveObject : public QUndoCommand
{
public:
    ComRemoveObject( MainHost *myHost,
                     const QModelIndex &objIndex,
                     RemoveType::Enum removeType,
                     QUndoCommand  *parent=0);
    void undo ();
    void redo ();

private:
    MainHost *myHost;

    ObjectInfo objectInfo;
    QWeakPointer<Connectables::Object>ObjectPtr;
    QWeakPointer<Connectables::Container>ContainerPtr;
    RemoveType::Enum removeType;
    ObjectContainerAttribs attr;

    QList< QPair<ConnectionInfo,ConnectionInfo> >listAddedCables;
    QList< QPair<ConnectionInfo,ConnectionInfo> >listRemovedCables;

    //    QByteArray objState;

    int currentGroup;
    int currentProg;
};

#endif // COMREMOVEOBJECT_H
