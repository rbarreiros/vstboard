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
      newNodes(false),
      needOptimize(false),
      countOptimize(0),
      needBuildModel(false),
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

void Renderer::UpdateView()
{
    if(!mutex.tryLockForRead())
        return;

    if(needBuildModel) {
        needBuildModel=false;
        BuildModel();
    }

    int cpt=0;
    foreach(RenderThread *th, listOfThreads) {
        th->mutex.lockForRead();
        foreach(RendererNode *node, th->listOfSteps) {
            if(node) {
                node->UpdateModel(&model);
            }
        }
        th->mutex.unlock();
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
    ClearNodes();
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

void Renderer::ClearNodes()
{
    foreach(RendererNode *node, listOfNodes) {
        delete node;
    }
    listOfNodes.clear();
}

void Renderer::OnNewRenderingOrder(const QList<SolverNode*> & listNodes)
{
    mutexNodes.lock();

    //copy nodes
    tmpListOfNodes.clear();
    foreach(SolverNode *n, listNodes) {
        tmpListOfNodes << new RendererNode(*n);
    }
    newNodes=true;
    mutexNodes.unlock();

    mutexOptimize.lock();
    if(countOptimize<3) {
        countOptimize++;
        QTimer::singleShot(5000, this, SLOT(Optimize()));
    }
    countOptimize++;
    QTimer::singleShot(50, this, SLOT(Optimize()));
    QTimer::singleShot(500, this, SLOT(Optimize()));
     mutexOptimize.unlock();
}

void Renderer::ProcessNewNodes()
{
    ClearNodes();

    listOfNodes = tmpListOfNodes;
    tmpListOfNodes.clear();

    optimizer.SetNbThreads(maxNumberOfThreads);
    optimizer.NewListOfNodes(listOfNodes);
    GetStepsFromOptimizer();


}

void Renderer::StartRender()
{

    if(!mutex.tryLockForRead(5)) {
        debug2(<<"Renderer::StartRender can't lock")
        return;
    }
//    mutex.lockForRead();

    if(stop) {
        mutex.unlock();
        return;
    }

    mutexNodes.lock();
    if(newNodes) {
        newNodes=false;
        mutexNodes.unlock();
        ProcessNewNodes();
    } else {
        mutexNodes.unlock();
    }

    mutexOptimize.lock();
    if(needOptimize) {
        needOptimize=false;
        mutexOptimize.unlock();
        optimizer.NewListOfNodes(listOfNodes);
        optimizer.Optimize();
        GetStepsFromOptimizer();
        needBuildModel=true;
    } else {
        mutexOptimize.unlock();
    }

    if(numberOfThreads==0) {
        mutex.unlock();
        return;
    }

    sem.release(numberOfThreads);
    for(int currentStep=-1; currentStep<numberOfSteps; currentStep++) {

        if( sem.tryAcquire(numberOfThreads,1000) ) {

            for(int i=0;i<numberOfThreads; i++) {
                RenderThread *th = listOfThreads.value(i);
                th->step = currentStep;
                th->sem.release();
            }

        } else {
            debug2(<<"Renderer::StartRender timeout, step:"<< currentStep << sem.available() << "/" << numberOfThreads )
        }

    }

    if( !sem.tryAcquire(numberOfThreads,5000) ) {
        debug2(<<"Renderer::StartRender timeout last step" << sem.available() << "/" << numberOfThreads )
        sem.acquire( sem.available() );
    }

    mutex.unlock();
}

void Renderer::Optimize()
{
    mutexOptimize.lock();
    needOptimize=true;
    countOptimize++;
    mutexOptimize.unlock();
}

void Renderer::GetStepsFromOptimizer()
{
    numberOfSteps = -1;
    numberOfThreads = maxNumberOfThreads;

    foreach(RenderThread *th, listOfThreads) {
        th->ResetSteps();
    }

    for(int th=0; th<maxNumberOfThreads; th++) {
        QMap<int, RendererNode* >lst = optimizer.GetListOfNode(th);
        RenderThread *thread = listOfThreads.value(th);
        thread->mutex.lockForWrite();
        thread->listOfSteps = lst;
        thread->mutex.unlock();

        if(!lst.isEmpty()) {
            int lastStep = lst.uniqueKeys().last();
            if( lastStep >= numberOfSteps) {
                numberOfSteps = lastStep+1;
            }
        }
    }
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
    model.clear();

    for(int i=0; i<maxNumberOfThreads; i++) {
        model.setHorizontalHeaderItem(i, new QStandardItem( QString::number(i+1) ) );
    }

    //insert steps
    for(int step=0; step<numberOfSteps; step++) {
        int col=0;
        foreach(RenderThread *th, listOfThreads) {
            th->mutex.lockForRead();
            if(th->listOfSteps.contains(step)) {
                RendererNode *node = th->listOfSteps.value(step,0);
                QStandardItem *item = new QStandardItem("0");
                model.setItem(step,col,item);
                if(node) {
                    node->modelIndex=item->index();
                    for(int st=node->minRenderOrder+1; st<=node->maxRenderOrder; st++) {
                        model.setItem(st,col, new QStandardItem("+"));
                    }
                }
            }
            th->mutex.unlock();
            col++;
        }
    }

    //insert merged nodes
    for(int step=0; step<numberOfSteps; step++) {
        int mergedOnThisStep=0;
        int col=0;
        foreach(RenderThread *th, listOfThreads) {
            th->mutex.lockForRead();
            RendererNode *node = th->listOfSteps.value(step,0);
            th->mutex.unlock();
            if(node) {
                int cpt=1;
                foreach(RendererNode *merged, node->GetListOfMergedNodes()) {
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
}
