/**************************************************************************
#    Copyright 2010-2011 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    under the terms of the GNU Lesser General Public License for more details.
#
#    You should have received a copy of the under the terms of the GNU Lesser General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "solvernode.h"

//QList<SolverNode*>SolverNode::listNodes;

SolverNode::SolverNode() :
        minRenderOrder(0),
        maxRenderOrder(9999),
        minRenderOrderOri(-1),
        maxRenderOrderOri(-1),
        modelNeedUpdate(true),
        cpuTime(0),
        loopFlag(0),
        countSteps(0),
        benchCount(0)
{
//    index = listNodes.size();
//    listNodes << this;
}

SolverNode::SolverNode(const SolverNode &c) :
    minRenderOrder(c.minRenderOrder),
    maxRenderOrder(c.maxRenderOrder),
    minRenderOrderOri(c.minRenderOrderOri),
    maxRenderOrderOri(c.maxRenderOrderOri),
    listOfObj(c.listOfObj),
    modelNeedUpdate(true),
    cpuTime(0),
    loopFlag(0),
    countSteps(0),
    benchCount(0)
{

}

void SolverNode::NewRenderLoop()
{
    mutex.lockForRead();
    foreach( QSharedPointer<Connectables::Object> ObjPtr, listOfObj) {
        if(!ObjPtr.isNull()) {
            ObjPtr->NewRenderLoop();
        }
    }
    foreach(SolverNode *mergedNode, listOfMergedNodes) {
        mergedNode->NewRenderLoop();
    }
    mutex.unlock();
}

void SolverNode::RenderNode()
{
    mutex.lockForRead();

#ifdef _WIN32
    unsigned long timerStart=0;
    FILETIME creationTime, exitTime, kernelTime, userTime;

//    if( benchCount<10) {
        if( GetThreadTimes( GetCurrentThread(), &creationTime, &exitTime, &kernelTime, &userTime) !=0 ) {
            timerStart = kernelTime.dwLowDateTime + userTime.dwLowDateTime;
        }
//    }
#endif

    foreach( QSharedPointer<Connectables::Object> objPtr, listOfObj) {
        if(!objPtr.isNull() && !objPtr->GetSleep()) {
            objPtr->Render();
        }
    }


#ifdef _WIN32
//    if(benchCount<10) {
//        FILETIME creationTime, exitTime, kernelTime, userTime;
        if( GetThreadTimes( GetCurrentThread(), &creationTime, &exitTime, &kernelTime, &userTime) !=0 ) {
            cpuTime += ( (kernelTime.dwLowDateTime + userTime.dwLowDateTime) - timerStart );
            benchCount++;
            modelNeedUpdate=true;
        }
//    }
#endif

    foreach(SolverNode *mergedNode, listOfMergedNodes) {
        mergedNode->RenderNode();
    }
    mutex.unlock();
}

unsigned long SolverNode::GetTotalCpuUsage()
{
    unsigned long cpu = cpuTime;

    mutex.lockForRead();
    foreach(SolverNode *merged, listOfMergedNodes) {
        cpu+=merged->cpuTime;
    }
    mutex.unlock();

    return cpu;
}

void SolverNode::AddMergedNode(SolverNode *merged)
{
    mutex.lockForWrite();
    listOfMergedNodes << merged;
    minRenderOrder = merged->minRenderOrder = std::max(minRenderOrder, merged->minRenderOrder);
    maxRenderOrder = merged->maxRenderOrder = std::min(maxRenderOrder, merged->maxRenderOrder);
    mutex.unlock();
}

void SolverNode::RemoveMergedNode(SolverNode *merged)
{
    mutex.lockForWrite();
    listOfMergedNodes.removeAll(merged);
    mutex.unlock();
}

void SolverNode::ClearMergedNodes()
{
    mutex.lockForWrite();
    listOfMergedNodes.clear();
    mutex.unlock();
}

void SolverNode::UpdateModel(QStandardItemModel *model)
{
    if(!modelNeedUpdate || !modelIndex.isValid())
        return;

    mutex.lockForRead();

    modelNeedUpdate=false;

    QString str = QString("[%1:%2][%3:%4]%5")
                    .arg(minRenderOrder)
                    .arg(maxRenderOrder)
                    .arg(minRenderOrderOri)
                    .arg(maxRenderOrderOri)
                    .arg(cpuTime);

    foreach( QSharedPointer<Connectables::Object> objPtr, listOfObj) {
        if(!objPtr.isNull() && !objPtr->GetSleep()) {
            str.append("\n" + objPtr->objectName());
        }
    }

    model->setData(modelIndex, str);

    foreach(SolverNode *mergedNode, listOfMergedNodes) {
        mergedNode->UpdateModel(model);
    }

    mutex.unlock();
}

void SolverNode::AddChild(SolverNode *child)
{
    listChilds << child;
    child->listParents << this;
}

void SolverNode::AddParent(SolverNode *parent)
{
    listParents << parent;
    parent->listChilds << this;
}

void SolverNode::RemoveChild(SolverNode *child)
{
    listChilds.removeAll(child);
    child->listParents.removeAll(this);
}

void SolverNode::RemoveParent(SolverNode *parent)
{
    listParents.removeAll(parent);
    parent->listChilds.removeAll(this);
}

void SolverNode::ResetLoopFlags() {
    loopFlag=false;
    countSteps = 0;
}

bool SolverNode::IsRoot()
{
    if(listParents.isEmpty())
        return true;

    return false;
}

bool SolverNode::IsTail()
{
    if(listChilds.isEmpty())
        return true;

    return false;
}

bool SolverNode::DistanceFromRoot(int &steps)
{
    //already been there, continue only if this route is shorter
    if(loopFlag && steps >= countSteps) {
        return true;
    }

    countSteps = steps;
    loopFlag=true;

    if(IsRoot())
        return true;

    steps++;


    //find the best route in parents nodes
    int bestSteps=99999;
    foreach(SolverNode* parent, listParents) {
        int copySteps = steps;
        if(parent->DistanceFromRoot(copySteps)) {
            bestSteps=std::min(bestSteps, copySteps);
         }
    }
    steps=bestSteps;
    return true;
}

bool SolverNode::DistanceFromTail(int &steps)
{
    //already been there, continue only if this route is shorter
    if(loopFlag && steps >= countSteps) {
        return true;
    }

    countSteps = steps;
    loopFlag=true;

    if(IsTail())
        return true;

    steps++;

    //find the best route in parents nodes
    int bestSteps=99999;
    foreach(SolverNode* child, listChilds) {
        int copySteps = steps;
        if(child->DistanceFromTail(copySteps)) {
            bestSteps=std::min(bestSteps, copySteps);
        }
    }
    steps=bestSteps;
    return true;
}

bool SolverNode::DetectLoopback(QList<SolverNode*>&listLoop)
{
    //we're back : it's a loop
    if(!listLoop.isEmpty() && listLoop.first()==this) {
       return true;
    }

    //already solved this branch : return
    if(loopFlag)
        return false;

    loopFlag=true;
    listLoop << this;

    foreach(SolverNode* child,listChilds) {
        QList<SolverNode*>copyLoop = listLoop;
        if(child->DetectLoopback(copyLoop)) {
            listLoop = copyLoop;
            return true;
        }
    }
    return false;
}

int SolverNode::SetMinRenderOrder(int order)
{
    minRenderOrder = std::max(minRenderOrder,order);
    foreach(SolverNode *parent,listParents) {
        minRenderOrder = std::max(minRenderOrder,parent->minRenderOrder+1);
    }

    int maxstep=minRenderOrder;
    foreach(SolverNode *child, listChilds) {
        maxstep=std::max(maxstep,child->SetMinRenderOrder(minRenderOrder+1));
    }

    return maxstep;
}

void SolverNode::SetMaxRenderOrder(int order)
{
    maxRenderOrder = std::min(maxRenderOrder,order);

    foreach(SolverNode *child,listChilds) {
        maxRenderOrder = std::min(maxRenderOrder,child->maxRenderOrder-1);
    }

    foreach(SolverNode *parent, listParents) {
        parent->SetMaxRenderOrder(maxRenderOrder-1);
    }
}

void SolverNode::ReconnectChildsTo(SolverNode *newParent)
{
    //connect childs to outputline
    foreach(SolverNode *child, listChilds) {
        newParent->AddChild(child);
    }
    //and remove them
    foreach(SolverNode *child, listChilds) {
        RemoveChild(child);
    }
}

void SolverNode::ReconnectParentsTo(SolverNode *newChild)
{
    //connect childs to outputline
    foreach(SolverNode *parent, listParents) {
        newChild->AddParent(parent);
    }
    //and remove them
    foreach(SolverNode *parent, listParents) {
        RemoveParent(parent);
    }
}

bool SolverNode::MergeWithParentNode()
{
    if(listParents.count()!=1) {
        debug2(<< "SolverNode::MergeWithParentNode need only one parent")
        return false;
    }

    SolverNode *parent = listParents.first();
    if(parent->listChilds.count()!=1) {
        debug2(<< "SolverNode::MergeWithParentNode parent must have one child only")
        return false;
    }

    parent->listOfObj << listOfObj;
    parent->listChilds = listChilds;
    parent->maxRenderOrder = maxRenderOrder;

    foreach(SolverNode *child, listChilds) {
        child->listParents.removeAll(this);
        child->listParents << parent;
    }
    return true;
}

