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

#ifndef UPDATEDELAYS_H
#define UPDATEDELAYS_H

#include "connectables/cable.h"
#include "solvernode.h"

class MainHost;
class UpdateDelays
{
public:
    UpdateDelays(MainHost *myHost, hashCables &listCables, const QList<SolverNode*> &listNodes);

private:
    void ResetDelays();
    bool AddDelays();
    bool SynchronizeParentNodes(SolverNode *node, long targetDelay);
    bool SynchronizeAudioOutputs();
//    void CreateDelayNode(SolverNode *node, SolverNode *childNode, long delay);

    void GetListCablesConnectedTo(quint16 objId, QList<Connectables::Cable*> &list);

    hashCables listCables;
    const QList<SolverNode*> listNodes;
    long globalDelay;
    MainHost *myHost;
};

#endif // UPDATEDELAYS_H
