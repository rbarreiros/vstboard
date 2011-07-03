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

#include "renderthread.h"
#include "renderer.h"
#include "renderernode.h"

RenderThread::RenderThread(Renderer *renderer, const QString &name)
    : QThread(renderer),
      renderer(renderer),
      sem(0),
      stop(false),
      lastStepForRendering(-1)
{
    setObjectName(name);
}

RenderThread::~RenderThread()
{
    ResetSteps();
    Stop();
    while(isRunning()) {
        usleep(100);
    }
}

void RenderThread::run()
{
    forever {
        sem.acquire();

        mutex.lockForRead();
        if(stop)
            return;
        mutex.unlock();

        RenderStep(step);
    }
}

void RenderThread::Stop()
{
    mutex.lockForWrite();
    stop=true;
    mutex.unlock();
    sem.release();
}

void RenderThread::RenderStep(int step)
{
    if(step==-1) {
        mutex.lockForRead();

        //reset counters
        QMap<int, RendererNode* >::iterator i = listOfSteps.begin();
        while (i != listOfSteps.end()) {
            RendererNode *node = i.value();
            if(node) {
                node->NewRenderLoop();
            }
            ++i;
        }
        lastStepForRendering = -1;
        mutex.unlock();
        renderer->sem.release();
        return;
    }

    RendererNode *n = listOfSteps.value(step,0);
    if(n!=0) {
        lastStepForRendering = n->maxRenderOrder;
    } else {
        //nothing to do
        if(lastStepForRendering==-1) {
            renderer->sem.release();
            return;
        }

    }

    //we have more time to render, release this step now
    if(lastStepForRendering > step) {
        renderer->sem.release();
    }

    //even if we have more time, we can start rendering now
    if(n!=0) {
//        debug2(<< "start" << currentThreadId() << n->listOfObj.first()->objectName())
        n->Render();
//        debug2(<< "end  " << currentThreadId() << n->listOfObj.first()->objectName())

        if(lastStepForRendering == step)
            renderer->sem.release();
        lastStepForRendering=-1;


    }
}

void RenderThread::ResetSteps()
{
    foreach( RendererNode *node, listOfSteps ) {
        delete node;
    }
    listOfSteps.clear();
}

void RenderThread::SetListOfSteps( const QMap<int, RendererNode* > &lst )
{
    mutex.lockForWrite();
    ResetSteps();
    listOfSteps = lst;
    mutex.unlock();
}

void RenderThread::StartRenderStep( int s )
{
    step=s;
    sem.release();
}

QList<RendererNode*> RenderThread::GetListOfNodes()
{
    mutex.lockForRead();
    QList<RendererNode*> tmpList;

    foreach(RendererNode *node, listOfSteps) {
        if(node) {
            RendererNode *newNode = new RendererNode(*node);
            tmpList << newNode;
            tmpList << newNode->GetListOfMergedNodes();
            newNode->ClearMergedNodes();
        }
    }
    mutex.unlock();
    return tmpList;
}
