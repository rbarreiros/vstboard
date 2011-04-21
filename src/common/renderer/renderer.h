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

#ifndef RENDERER_H
#define RENDERER_H

#include "precomp.h"
#include "connectables/object.h"
#include "renderthread.h"
#include "optimizer.h"

#include "renderernode.h"

typedef QMultiHash<int, SolverNode*> orderedNodes;

class MainHost;
class Renderer : public QObject
{
    Q_OBJECT

public:
    Renderer(MainHost *myHost);
    ~Renderer();
    void SetNbThreads(int nbThreads);
    void SetEnabled(bool enabled) {stop=!enabled;}
    void OnNewRenderingOrder(const QList<SolverNode*> & listNodes);

    QStandardItemModel model;

protected:
    void InitThreads();
    void BuildModel();
    void GetStepsFromOptimizer();
    void Clear();
    void ClearNodes();
    void ProcessNewNodes();

    int maxNumberOfThreads;
    int numberOfThreads;
    int numberOfSteps;
    bool stop;

    QList<RendererNode*>listOfNodes;
    QList<RendererNode*>tmpListOfNodes;
    QMutex mutexNodes;
    bool newNodes;
    QMutex mutexOptimize;
    bool needOptimize;
    int countOptimize;
    bool needBuildModel;
    QList<RenderThread*>listOfThreads;
    QReadWriteLock mutex;
    QSemaphore sem;
    MainHost *myHost;
    QTimer updateViewTimer;
    Optimizer optimizer;

public slots:

    void StartRender();
    void UpdateView();
    void Optimize();

friend class RenderThread;
};

#endif // RENDERER_H
