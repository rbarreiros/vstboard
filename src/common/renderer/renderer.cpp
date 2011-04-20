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

#include "renderer.h"
#include "pathsolver.h"
#include "connectables/object.h"
#include "solvernode.h"
#include "mainhost.h"
#include "views/configdialog.h"

Renderer::Renderer(MainHost *myHost)
    : QObject(),
      numberOfThreads(0),
      numberOfSteps(0),
      stop(false),
      sem(0),
      myHost(myHost)
{
    maxNumberOfThreads = ConfigDialog::defaultNumberOfThreads(myHost);
    InitThreads();

    connect(&updateViewTimer, SIGNAL(timeout()),
            this, SLOT(UpdateView()));
    updateViewTimer.start(500);
}

Renderer::~Renderer()
{
    updateViewTimer.stop();
    Clear();
}

void Renderer::InitThreads()
{
    for(int i=0; i<maxNumberOfThreads; i++) {
        RenderThread *th = new RenderThread(this, QString::number(i));
        listOfThreads << th;
        th->start(QThread::TimeCriticalPriority);
    }
}

void Renderer::BuildModel()
{
    mutex.lockForRead();

    model.clear();
    for(int i=0; i<maxNumberOfThreads; i++) {
        model.setHorizontalHeaderItem(i, new QStandardItem( QString::number(i+1) ) );
    }

    //insert steps
    for(int step=0; step<numberOfSteps; step++) {
        int col=0;
        foreach(RenderThread *th, listOfThreads) {
            if(th->listOfSteps.contains(step)) {
                SolverNode *node = th->listOfSteps.value(step,0);
                QStandardItem *item = new QStandardItem("0");
                model.setItem(step,col,item);
                if(node) {
                    node->modelIndex=item->index();
                    for(int st=node->minRenderOrder+1; st<=node->maxRenderOrder; st++) {
                        model.setItem(st,col, new QStandardItem("+"));
                    }
                }
            }
            col++;
        }
    }

    //insert merged nodes
    for(int step=0; step<numberOfSteps; step++) {
        int mergedOnThisStep=0;
        int col=0;
        foreach(RenderThread *th, listOfThreads) {
            SolverNode *node = th->listOfSteps.value(step,0);
            if(node) {
                int cpt=1;
                foreach(SolverNode *merged, node->GetListOfMergedNodes()) {
                    if(cpt>mergedOnThisStep) {
                        mergedOnThisStep=cpt;
                        model.insertRow(step+mergedOnThisStep);
                    }
                    QStandardItem *item = new QStandardItem();
                    model.setItem(step+cpt,col,item);
                    merged->modelIndex=item->index();
                    cpt++;
                }
                col++;
            }
        }
    }
    mutex.unlock();
}

void Renderer::UpdateView()
{
    if(!mutex.tryLockForRead())
        return;

    int cpt=0;
    foreach(RenderThread *th, listOfThreads) {
        foreach(SolverNode *node, th->listOfSteps) {
            if(node) {
                node->UpdateModel(&model);
            }
        }
        cpt++;
    }
    mutex.unlock();
}

void Renderer::Clear()
{
    mutex.lockForWrite();
    stop=true;
    numberOfThreads=0;
    numberOfSteps=0;
    foreach(RenderThread *th, listOfThreads) {
        delete th;
    }
    listOfThreads.clear();
    mutex.unlock();
}

void Renderer::SetNbThreads(int nbThreads)
{
    Clear();

    mutex.lockForWrite();
    maxNumberOfThreads = nbThreads;
    if(maxNumberOfThreads<=0) maxNumberOfThreads = 4;
    InitThreads();
    stop=false;
    mutex.unlock();
}

void Renderer::OnNewRenderingOrder(QList<SolverNode*> & listNodes)
{
    QList<SolverNode*>newLstNodes;
    foreach(SolverNode *node, listNodes) {
        SolverNode *newNode = new SolverNode(*node);
        newLstNodes << newNode;
    }

    optimizer.SetNbThreads(maxNumberOfThreads);
    optimizer.NewListOfNodes(newLstNodes);

    GetStepsFromOptimizer();

    QTimer::singleShot(500, this, SLOT(Optimize()));
}

void Renderer::StartRender()
{

    if(!mutex.tryLockForRead()) {
        debug2(<<"Renderer::StartRender can't lock")
        return;
    }

    if(stop) {
        mutex.unlock();
        return;
    }

    if(numberOfThreads==0) {
        mutex.unlock();
        return;
    }

    sem.release(numberOfThreads);
    for(int currentStep=-1; currentStep<numberOfSteps; currentStep++) {
        sem.acquire(numberOfThreads);
        for(int i=0;i<numberOfThreads; i++) {
            RenderThread *th = listOfThreads.value(i);
            th->step = currentStep;
            th->sem.release();
        }
    }
    sem.acquire(numberOfThreads);
    mutex.unlock();
}

void Renderer::Optimize()
{
    mutex.lockForWrite();
    optimizer.Optimize();
    mutex.unlock();
    GetStepsFromOptimizer();
    BuildModel();
}

void Renderer::GetStepsFromOptimizer()
{
    mutex.lockForWrite();

    numberOfSteps = -1;
    numberOfThreads = maxNumberOfThreads;

    foreach(RenderThread *th, listOfThreads) {
        th->ResetSteps();
    }

    for(int th=0; th<maxNumberOfThreads; th++) {
        QMap<int, SolverNode* >lst = optimizer.GetListOfNode(th);
        listOfThreads.value(th)->listOfSteps = lst;
        if(lst.size() > numberOfSteps) {
            numberOfSteps = lst.size();
        }
    }
    mutex.unlock();
}
