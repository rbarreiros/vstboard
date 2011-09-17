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

#ifndef PATHSOLVER_H
#define PATHSOLVER_H

//#include "precomp.h"
#include "connectables/object.h"
#include "connectables/cable.h"
#include "solvernode.h"
#include "globals.h"
#include "renderer.h"


class MainHost;

class PathSolver : public QObject
{
Q_OBJECT

public:
    explicit PathSolver(MainHost *parent);
    ~PathSolver();
    void Resolve(hashCables cables, Renderer *renderer);
    void UpdateDelays();

protected:
    void Clear();
    void CreateNodes();
    void PutParentsInNodes();
    bool ChainNodes();
    void UnwrapLoops();
    void SetMinAndMaxStep();
    void RemoveUnusedNodes();

    void ResetDelays();
    bool AddDelays();
    bool SynchronizeParentNodes(SolverNode *node, long targetDelay);
    bool SynchronizeAudioOutputs();
    void CreateDelayNode(SolverNode *node, SolverNode *childNode, long delay);
    void GetListCablesConnectedTo(quint16 objId, QList<Connectables::Cable*> &list);
    void GetListPinsConnectedTo(ConnectionInfo out, QList<ConnectionInfo> &list);

    QList<SolverNode*> ListOfGoodStarts(const QList<SolverNode*>&loop);
    QList<SolverNode*> BestStartsInAList(const QList<SolverNode*>&loop, const QList<SolverNode*>&possibleStarts);
    QList<SolverNode*> CopyNodesChain(const QList<SolverNode*>&chain);
    QList<SolverNode*> OpenLoop(const QList<SolverNode*>&loop, SolverNode* startingNode);

    QList< QSharedPointer<Connectables::Object> >GetListParents(QSharedPointer<Connectables::Object> objPtr);
//    void GetListParentsOfBridgePin(const ConnectionInfo &info, QList< QSharedPointer<Connectables::Object> > &listParents);
    QList< QSharedPointer<Connectables::Object> >GetListChildren(QSharedPointer<Connectables::Object> objPtr);
//    void GetListChildrenOfBridgePin(const ConnectionInfo &info, QList< QSharedPointer<Connectables::Object> > &listChildren);

    hashCables listCables;
    MainHost *myHost;

    QList<SolverNode*>listNodes;
    QMutex mutex;
    long globalDelay;
signals:
    void GlobalDelayChanged(long samples);
};

#endif // PATHSOLVER_H
