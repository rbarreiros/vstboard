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

#ifndef SOLVERNODE_H
#define SOLVERNODE_H

#include "precomp.h"
#include "connectables/object.h"
#include <QReadWriteLock>

class SolverNode
{
public:
    SolverNode();
    SolverNode(const SolverNode &c);
    ~SolverNode();

    void ReconnectChildsTo(SolverNode *newParent);
    void ReconnectParentsTo(SolverNode *newChild);
    bool DetectLoopback(QList<SolverNode*>&listLoop);
    bool DistanceFromRoot(int &steps);
    bool DistanceFromTail(int &steps);
    bool IsRoot();
    bool IsTail();

    void AddChild(SolverNode *child);
    void AddParent(SolverNode *parent);
    void RemoveChild(SolverNode *child);
    void RemoveParent(SolverNode *parent);
    int SetMinRenderOrder(int order);
    void SetMaxRenderOrder(int order);

    bool MergeWithParentNode();

    void NewRenderLoop();
    void RenderNode();
    void AddMergedNode(SolverNode *merged);
    void RemoveMergedNode(SolverNode *merged);
    void ClearMergedNodes();
    QList<SolverNode*> GetListOfMergedNodes() const {return listOfMergedNodes;}
    void UpdateModel(QStandardItemModel *model);
    unsigned long GetCpuUsage();

    int minRenderOrder;
    int maxRenderOrder;

    int minRenderOrderOri;
    int maxRenderOrderOri;

    bool loopFlag;
    int countSteps;

//    QSharedPointer<Connectables::Object> objectPtr;
    QList<SolverNode*>listParents;
    QList<SolverNode*>listChilds;

    QList< QSharedPointer<Connectables::Object> >listOfObj;

    void ResetLoopFlags();
//    static QList<SolverNode*>listNodes;
//    int index;

    unsigned long cpuTime;
    int benchCount;
    QPersistentModelIndex modelIndex;
    bool modelNeedUpdate;

protected:
     QList<SolverNode*>listOfMergedNodes;
     QReadWriteLock mutex;
};

#endif // SOLVERNODE_H
