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

Renderer::Renderer(MainHost *myHost)
    : QObject(),
      numberOfThreads(0),
      numberOfSteps(0),
      stop(false),
      sem(0),
      myHost(myHost)
{
    maxNumberOfThreads = myHost->GetSetting("MaxNbThreads", 4).toInt();
    if(maxNumberOfThreads<=0) maxNumberOfThreads = 4;
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
                QStandardItem *item = new QStandardItem();
                model.setItem(step,col,item);
                node->modelIndex=item->index();

                for(int st=node->minRenderOrder+1; st<=node->maxRenderOrder; st++) {
                    model.setItem(st,col, new QStandardItem("+"));
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

void Renderer::Optimize()
{
    UpdateView();

    mutex.lockForWrite();

    for(int step=0; step<numberOfSteps; step++) {

//        bool merged=false;

//        //check if there's more node than threads on this step
//        foreach(RenderThread *th, listOfThreads) {
//            SolverNode *n = th->listOfSteps.value(step,0);
//            if(n) {
//                if(!n->GetListOfMergedNodes().isEmpty()) {
//                    merged=true;
//                }
//            }
//        }

//        if(merged) {
            QList<SolverNode*>lstOfNodes;

            //get a list of nodes on this step
            foreach(RenderThread *th, listOfThreads) {
                if(th->listOfSteps.contains(step)) {
                    SolverNode *n = th->listOfSteps.take(step);
                    if(n) {
                        lstOfNodes << n;
                        lstOfNodes << n->GetListOfMergedNodes();
                    }
                }
            }

            //remove merged nodes
            foreach(SolverNode *node, lstOfNodes) {
                node->ClearMergedNodes();
            }

            //find the biggest nodes
            QList<SolverNode*>biggestNodes;
            while(biggestNodes.count()<numberOfThreads && lstOfNodes.count()!=0) {
                SolverNode *biggest=0;
                foreach(SolverNode *n, lstOfNodes) {
                    if(!biggest || n->cpuTime > biggest->cpuTime) {
                        biggest=n;
                    }
                }
                lstOfNodes.removeAll(biggest);
                biggestNodes << biggest;
            }

            //merge the smallest nodes
            int row=step+1;
            while(lstOfNodes.count()!=0) {
                model.insertRow(row);

                foreach(SolverNode *n, biggestNodes) {
                    if(lstOfNodes.count()!=0) {
                        SolverNode *merged = lstOfNodes.takeFirst();
                        n->AddMergedNode( merged );
                    }
                }
                row++;
            }

            //add nodes to threads
            int thCpt=0;
            foreach(RenderThread *th, listOfThreads) {
                if(biggestNodes.count()!=0) {

                    //don't touch empty steps, only create new steps or replace existing
                    if(!th->listOfSteps.contains(step) || th->listOfSteps.value(step)!=0) {

                        SolverNode *node = biggestNodes.takeFirst();
                        th->listOfSteps.insert(step, node);
                        node->modelIndex = model.index(step,thCpt);
                        int row=step+1;
                        int col=node->modelIndex.column();
                        foreach(SolverNode *merged, node->GetListOfMergedNodes()) {
                            merged->modelIndex = model.index( row, col);
                            row++;
                        }
                    }
                }
                thCpt++;
            }
//        }

    //shorten nodes if possible
        unsigned long maxcpu=0;

        //find the bigger thread on this step, not including nodes spanning on multiple steps
        foreach(RenderThread *th, listOfThreads) {
            SolverNode *node = th->listOfSteps.value(step,0);
            if(node && node->maxRenderOrder == step) {
                unsigned long cpu = node->GetCpuUsage();
                if(cpu>maxcpu)
                    maxcpu=cpu;
            }
        }

        //20% security
        maxcpu*=0.8;

        //shorten steps uselessely using multiple steps
        foreach(RenderThread *th, listOfThreads) {
            SolverNode *node = th->listOfSteps.value(step,0);
            if(node && node->maxRenderOrder > step) {
                if(maxcpu==0 || node->GetCpuUsage() < maxcpu) {
                    th->ShortenNode(node,step);
                }
            }
        }
    }

    mutex.unlock();

    BuildModel();
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

void Renderer::OnNewRenderingOrder(const QList<SolverNode*> & listNodes)
{
    mutex.lockForWrite();
    if(stop) {
        mutex.unlock();
        return;
    }

    int step = -1;
    int threadCount = -1;

    numberOfSteps = step;
    numberOfThreads = threadCount;

    foreach(RenderThread *th, listOfThreads) {
        th->ResetSteps();
    }

    listOfNodesToMerge.clear();

    foreach(SolverNode *node, orderedListOfNodes) {
        delete node;
    }
    orderedListOfNodes.clear();

    foreach(SolverNode *node, listNodes) {
        orderedListOfNodes.insert(node->minRenderOrder, new SolverNode(*node));
    }

    orderedNodes::iterator i = orderedListOfNodes.begin();
    while (i != orderedListOfNodes.end()) {
        step = i.key();
        if(step>numberOfSteps)
            numberOfSteps=step;

        SolverNode *node = i.value();
        node->minRenderOrderOri = node->minRenderOrder;
        node->maxRenderOrderOri = node->maxRenderOrder;

        bool found=false;

        //try to find a place in threads
        int j=0;
        while(!found && j<maxNumberOfThreads) {
            if(listOfThreads.value(j)->SetStep( node )) {
                if(j>numberOfThreads)
                    numberOfThreads=j;
                found=true;
            }
            j++;
        }

        if(!found) {
            //no place found, try again
            j=0;
            int bestThread = -1;
            int bestMod = -1;
            while(j<maxNumberOfThreads) {
                int nbMods = listOfThreads.value(j)->NeededModificationsToInsertNode( node );
                if(nbMods!=-1 && (bestMod==-1 ||  nbMods<bestMod) ) {
                    found=true;
                    bestMod = nbMods;
                    bestThread = j;
                }
                j++;
            }

            if(found) {
                listOfThreads.value(bestThread)->NeededModificationsToInsertNode( node, true );
            } else {
                listOfNodesToMerge << node;
            }
        }
        ++i;
    }

    //add nodes to merge
    foreach(SolverNode *node, listOfNodesToMerge) {
        int j=0;
        bool found=false;
        while(!found && j<maxNumberOfThreads) {
            SolverNode * parentNode = listOfThreads.value(j)->listOfSteps.value( node->minRenderOrder, 0 );
            if(parentNode) {
                parentNode->AddMergedNode(node);
                found = true;
            }
            j++;
        }
    }

    numberOfSteps++;
    numberOfThreads++;

    mutex.unlock();

//    BuildModel();

    QTimer::singleShot(300, this, SLOT(Optimize()));
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

