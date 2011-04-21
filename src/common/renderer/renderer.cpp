/**************************************************************************
#    Copyright 2010-2011 Rapha�l Fran�ois
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
#include "renderernode.h"

Renderer::Renderer(MainHost *myHost)
    : QObject(),
      numberOfThreads(0),
      numberOfSteps(0),
      stop(false),
      newNodes(false),
      needOptimize(false),
      nextOptimize(0),
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
        optimizer.BuildModel( &model );
        optimizer.UpdateView( &model );
    }



//    foreach(RenderThread *th, listOfThreads) {
//        th->UpdateView( optimizer.GetModel() );
//    }
    mutex.unlock();
}

QStandardItemModel * Renderer::GetModel()
{
    return &model;
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
    if(maxNumberOfThreads<=0) maxNumberOfThreads = 2;
    InitThreads();
    stop=false;
    mutex.unlock();
}

//void Renderer::ClearNodes()
//{
//    foreach(RendererNode *node, listOfNodes) {
//        delete node;
//    }
//    listOfNodes.clear();
//}

void Renderer::LoadNodes(const QList<RendererNode*> & listNodes)
{
    if(listNodes.isEmpty()) {
        myHost->SetSolverUpdateNeeded(true);
        return;
    }

    mutexNodes.lock();
    //copy nodes
    QList<RendererNode*> tmpListOfNodes;
    foreach(RendererNode *n, listNodes) {
        tmpListOfNodes << new RendererNode(*n);
    }
    optimizer.NewListOfNodes( tmpListOfNodes );
    newNodes=true;
    mutexNodes.unlock();

    mutexOptimize.lock();
    nextOptimize=0;
    needOptimize=true;
    mutexOptimize.unlock();
}

QList<RendererNode*> Renderer::SaveNodes()
{
    return optimizer.GetListOfNodes();
}

void Renderer::OnNewRenderingOrder(const QList<SolverNode*> & listNodes)
{
    mutexNodes.lock();

    //copy nodes
    QList<RendererNode*> tmpListOfNodes;
    foreach(SolverNode *n, listNodes) {
        tmpListOfNodes << new RendererNode(*n);
    }

    optimizer.SetNbThreads(maxNumberOfThreads);
    optimizer.NewListOfNodes( tmpListOfNodes );

    newNodes=true;
    mutexNodes.unlock();

    if(maxNumberOfThreads>1) {
        mutexOptimize.lock();
        nextOptimize=50;
        QTimer::singleShot(nextOptimize, this, SLOT(Optimize()));
        mutexOptimize.unlock();
    }
}

void Renderer::StartRender()
{
//    mutex.lockForRead();
    if(!mutex.tryLockForRead(5)) {
        debug2(<<"Renderer::StartRender can't lock")
        return;
    }

    if(stop) {
        mutex.unlock();
        return;
    }

    mutexNodes.lock();
    if(newNodes) {
        newNodes=false;
        mutexNodes.unlock();
        GetStepsFromOptimizer();
        needBuildModel=true;
    } else {
        mutexNodes.unlock();
    }

    mutexOptimize.lock();
    if(needOptimize) {
        needOptimize=false;
        mutexOptimize.unlock();

        QList<RendererNode*> tmpListOfNodes;
        foreach(RenderThread *th, listOfThreads) {
            tmpListOfNodes << th->GetListOfNodes();
        }
        optimizer.NewListOfNodes( tmpListOfNodes );
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

    sem.release(maxNumberOfThreads);
    for(int currentStep=-1; currentStep<numberOfSteps; currentStep++) {

        if( sem.tryAcquire(maxNumberOfThreads,5000) ) {

            foreach( RenderThread *th, listOfThreads) {
                th->StartRenderStep( currentStep );
            }

        } else {
            debug2(<<"Renderer::StartRender timeout, step:"<< currentStep << sem.available() << "/" << maxNumberOfThreads )
        }

    }

    if( !sem.tryAcquire(maxNumberOfThreads,5000) ) {
        debug2(<<"Renderer::StartRender timeout last step" << sem.available() << "/" << maxNumberOfThreads )
        sem.acquire( sem.available() );
    }
    mutex.unlock();
}

void Renderer::Optimize()
{
    mutexOptimize.lock();
    needOptimize=true;

    switch(nextOptimize) {
        case 50:
            nextOptimize=500;
            break;
        case 500:
            nextOptimize=5000;
            break;
        default:
            nextOptimize=0;
            break;
    }

    if(nextOptimize!=0)
        QTimer::singleShot(nextOptimize, this, SLOT(Optimize()));

    mutexOptimize.unlock();
}

void Renderer::GetStepsFromOptimizer()
{
    numberOfSteps = -1;
    numberOfThreads = maxNumberOfThreads;

//    foreach(RenderThread *th, listOfThreads) {
//        th->ResetSteps();
//    }

    for(int th=0; th<maxNumberOfThreads; th++) {
        QMap<int, RendererNode* >lst = optimizer.GetThreadNodes(th);

        RenderThread *thread = listOfThreads.value(th);
        thread->SetListOfSteps( lst );

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
