/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "pathsolver.h"
#include "globals.h"
#include "projectfile/projectfile.h"
#include "projectfile/program.h"
#include "connectables/objectfactory.h"

PathSolver::PathSolver(QObject *parent) :
    QObject(parent)
{
}

PathSolver::~PathSolver()
{
    Clear();
}

void PathSolver::Clear()
{
    model.clear();
   /* const Connectables::hashObjects listObjects = Connectables::ObjectFactory::Get()->GetListObjects();
    Connectables::hashObjects::const_iterator i = listObjects.constBegin();
    while(i!=listObjects.constEnd()) {
        i.value()->solverNode = 0;
        ++i;
    }*/

    renderingOrder.clear();
    foreach(SolverNode* line, SolverNode::listNodes) {
        delete line;
    }
    SolverNode::listNodes.clear();

}

void PathSolver::Resolve(hashCables cables)
{
//    debug("solver resolve");

    Clear();

    listCables = cables;

    //encapsulate objects in solvernodes
//    debug(QString("PathSolver::Resolve nbobj1 %1").arg( Connectables::ObjectFactory::Get()->GetListObjects().size() ).toAscii())
    const Connectables::hashObjects listObjects = Connectables::ObjectFactory::Get()->GetListObjects();
    Connectables::hashObjects::const_iterator i = listObjects.constBegin();
    while(i!=listObjects.constEnd()) {
        QSharedPointer<Connectables::Object> objPtr = i.value();
        if(!objPtr.isNull()) {
            if(objPtr->info().nodeType!=NodeType::bridge) {
                SolverNode *node = new SolverNode();
                node->objectPtr = objPtr;
                objPtr->SetSolverNode(node);
            }
        }
        ++i;
    }

    //list childs and parents of all nodes
    foreach(SolverNode *node,SolverNode::listNodes) {
        foreach(QSharedPointer<Connectables::Object> parent,GetListParents(node->objectPtr)) {
            if(!parent.isNull())
                node->listParents << parent->GetSolverNode();
        }
        foreach(QSharedPointer<Connectables::Object> child,GetListChildren(node->objectPtr)) {
            if(!child.isNull())
                node->listChilds << child->GetSolverNode();
        }

        //no parents, no childs : don't render it
        if(node->listChilds.isEmpty() && node->listParents.isEmpty()) {
            SolverNode::listNodes.removeAll(node);
            delete node;
        }
    }

    //unwrap loops
    foreach(SolverNode *node,SolverNode::listNodes) {
        SolverNode::ResetLoopFlags();

        //find loop
        QList<SolverNode*>loop;
        if(node->DetectLoopback(loop)) {

            //find the node close to a root
            QList<SolverNode*>listStarts = ListOfGoodStarts(loop);

            SolverNode* bestStart=0;

            //we have a winner
            if(listStarts.size()==1)
                bestStart = listStarts.first();

            //it's a draw... try to find the best one
            if(listStarts.size()>1)
                listStarts = BestStartsInAList(loop,listStarts);

            //now we have a winner
            if(listStarts.size()==1)
                bestStart = listStarts.first();

            //a draw, again. i don't know what else to check : take the first in the list
            if(listStarts.size()>1)
                bestStart = listStarts.first();

            if(bestStart) {
                //opened the loop : no more feedback
                QList<SolverNode*>openedLoop = OpenLoop(loop, bestStart);

                //create a copy of the loop
                QList<SolverNode*>copiedLoop = CopyNodesChain(openedLoop);

                //insert the copied loop before the original one
                openedLoop.first()->ReconnectParentsTo(copiedLoop.first());
                copiedLoop.last()->AddChild(openedLoop.first());

            } else {
                Q_ASSERT(false);
                //bestEnd not found : wtf ?
            }
        }
    }

    //start from each root and set the rendering order of each child nodes
    int maxStep=0;
    foreach(SolverNode *node, SolverNode::listNodes) {
        if(!node->IsRoot())
            continue;
        maxStep = std::max(maxStep, node->SetMinRenderOrder(0));
    }

//    debug(QString("PathSolver::Resolve nbobj2 %1").arg( Connectables::ObjectFactory::Get()->GetListObjects().size() ).toAscii())

    foreach(SolverNode *node, SolverNode::listNodes) {
        if(!node->IsTail())
            continue;
        node->SetMaxRenderOrder(maxStep);
    }

    //put the nodes in an ordered map
    foreach(SolverNode* node, SolverNode::listNodes) {
        renderingOrder.insert(node->maxRenderOrder,node);
    }

    UpdateModel();

    emit NewRenderingOrder(&renderingOrder);
}

void PathSolver::UpdateModel()
{

    int currentStep=0;
    QList<QStandardItem*>listItems;

    orderedNodes::iterator i = renderingOrder.end();
    while (i != renderingOrder.begin()) {
        --i;
        if(i.key() != currentStep) {
            if(listItems.size()!=0) {
                model.insertRow(0,listItems);
                listItems.clear();
            }
            currentStep = i.key();
        }
        listItems << new QStandardItem(QString("[%1:%2] %3:%4")
                                       .arg(i.value()->minRenderOrder)
                                       .arg(i.value()->maxRenderOrder)
                                       .arg(i.value()->objectPtr->GetIndex())
                                       .arg(i.value()->objectPtr->objectName())
                                       );


    }
    if(listItems.size()>0)
        model.insertRow(0,listItems);
}

//return a list of good starts by looking at the nodes close to a root
QList<SolverNode*> PathSolver::ListOfGoodStarts(const QList<SolverNode*>&loop)
{
    QList<SolverNode*> bestStarts;
    int minStep=99999;

    foreach(SolverNode *node, loop) {
        SolverNode::ResetLoopFlags();
        int steps=0;

        if(node->DistanceFromRoot(steps)) {
            //detect equality
            if(steps==minStep) {
                bestStarts << node;
            }
            //this is a new best
            if(steps<minStep) {
                bestStarts.clear();
                minStep=steps;
                bestStarts << node;
            }
        }
    }

    return bestStarts;
}

//return the best start of a loop by finding the ending node close to a tail
QList<SolverNode*> PathSolver::BestStartsInAList(const QList<SolverNode*>&loop, const QList<SolverNode*>&possibleStarts)
{
//    SolverNode* bestStart=0;
    QList<SolverNode*> bestStarts;
    int minStep=99999;

    foreach(SolverNode *triedStart, possibleStarts) {
        //which node would be the end one in this case
        SolverNode *triedEnd;
        if(triedStart == loop.first())
            triedEnd = loop.last();
        else {
            triedEnd = loop.at( loop.indexOf(triedStart)-1 );
        }

        SolverNode::ResetLoopFlags();
        int steps=0;

        if(triedEnd->DistanceFromTail(steps)) {
            if(steps==minStep) {
                bestStarts << triedStart;
            }
            if(steps<minStep) {
                bestStarts.clear();
                bestStarts << triedStart;
                minStep=steps;
            }
        }
    }

    return bestStarts;
}

//copy a chain of nodes
QList<SolverNode*> PathSolver::CopyNodesChain(const QList<SolverNode*>&chain)
{
    QList<SolverNode*>copiedChain;
    SolverNode *parentNode=0;
    foreach(SolverNode *node, chain) {
        SolverNode *copiedNode = new SolverNode();
        copiedNode->objectPtr = node->objectPtr;

        if(parentNode)
            parentNode->AddChild(copiedNode);

        parentNode = copiedNode;
        copiedChain << copiedNode;
    }
    return copiedChain;
}

//cut a loop before the selected start
QList<SolverNode*> PathSolver::OpenLoop(const QList<SolverNode*>&loop, SolverNode* startingNode)
{
    QList<SolverNode*>destLoop;

    //rotate the loop, it must start with startingNode
    int pos=loop.indexOf(startingNode);
    for(int i=0;i<loop.size();i++) {
        if(pos==loop.size())
            pos=0;
        destLoop << loop.at(pos);
        ++pos;
    }

    //remove connections between last and first to break the feedback
    destLoop.first()->RemoveParent(destLoop.last());

    return destLoop;
}


//get the objects connected to the input pins
QList< QSharedPointer<Connectables::Object> >PathSolver::GetListParents( QSharedPointer<Connectables::Object> objPtr)
{
    QList< QSharedPointer<Connectables::Object> >listParents;

    hashCables::iterator i = listCables.begin();
    while (i != listCables.end()) {
        QSharedPointer<Connectables::Object> parentPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(i.key().objId);
        if(!parentPtr.isNull()) {
            if(i.value().objId == objPtr->GetIndex()) {
                if(parentPtr->info().nodeType == NodeType::bridge) {
                    GetListParentsOfBridgePin(i.key(),listParents);
                } else {
                    if(!listParents.contains(parentPtr))
                        listParents << parentPtr;
                }
            }
        }
        ++i;
    }

    return listParents;
}

void PathSolver::GetListParentsOfBridgePin(const ConnectionInfo &info, QList< QSharedPointer<Connectables::Object> > &listParents)
{
    hashCables::iterator i = listCables.begin();
    while (i != listCables.end()) {
        QSharedPointer<Connectables::Object> parentPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(i.key().objId);
        if(!parentPtr.isNull()) {
            if(i.value().objId == info.objId && i.value().pinNumber == info.pinNumber && i.value().type == info.type) {
                if(parentPtr->info().nodeType == NodeType::bridge) {
                    GetListParentsOfBridgePin(i.key(),listParents);
                } else {
                    if(!listParents.contains(parentPtr))
                        listParents << parentPtr;
                }
            }
        }
        ++i;
    }
}

//get the objects connected to the output pins
QList< QSharedPointer<Connectables::Object> >PathSolver::GetListChildren( QSharedPointer<Connectables::Object> objPtr)
{
    QList< QSharedPointer<Connectables::Object> >listChildren;

    hashCables::iterator i = listCables.begin();
    while (i != listCables.end()) {
        QSharedPointer<Connectables::Object> childPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(i.value().objId);
        if(!childPtr.isNull()) {
            if(i.key().objId == objPtr->GetIndex()) {
                if(childPtr->info().nodeType == NodeType::bridge) {
                    GetListChildrenOfBridgePin(i.value(),listChildren);
                } else {
                    if(!listChildren.contains(childPtr))
                        listChildren << childPtr;
                }
            }
        }
        ++i;
    }

    return listChildren;
}

void PathSolver::GetListChildrenOfBridgePin(const ConnectionInfo &info, QList< QSharedPointer<Connectables::Object> > &listChildren)
{
    hashCables::iterator i = listCables.begin();
    while (i != listCables.end()) {
        QSharedPointer<Connectables::Object> childPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(i.value().objId);
        if(!childPtr.isNull()) {
            if(i.key().objId == info.objId && i.key().pinNumber == info.pinNumber && i.key().type == info.type) {
                if(childPtr->info().nodeType == NodeType::bridge) {
                    GetListChildrenOfBridgePin(i.value(),listChildren);
                } else {
                    if(!listChildren.contains(childPtr))
                        listChildren << childPtr;
                }
            }
        }
        ++i;
    }
}
