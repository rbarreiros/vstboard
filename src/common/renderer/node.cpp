#include "node.h"

Node::Node() :
    minRenderOrder(0),
    maxRenderOrder(9999)
{
}

Node::Node(const Node &c) :
    minRenderOrder(c.minRenderOrder),
    maxRenderOrder(c.maxRenderOrder),
    listOfObj(c.listOfObj)
{

}

