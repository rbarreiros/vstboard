#ifndef NODE_H
#define NODE_H

#include "precomp.h"
#include "connectables/object.h"

class Node
{
public:
    Node();
    Node(const Node &c);
    int minRenderOrder;
    int maxRenderOrder;
    QList< QSharedPointer<Connectables::Object> >listOfObj;
};

#endif // NODE_H
