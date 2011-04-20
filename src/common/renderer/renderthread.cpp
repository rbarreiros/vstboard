#include "renderthread.h"
#include "renderer.h"

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
    sem.release(1);
}

void RenderThread::RenderStep(int step)
{
    if(step==-1) {
        mutex.lockForRead();

        //reset counters
        QMap<int, SolverNode* >::iterator i = listOfSteps.begin();
        while (i != listOfSteps.end()) {
            SolverNode *node = i.value();
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

    SolverNode *n = listOfSteps.value(step,0);
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
        n->RenderNode();

        if(lastStepForRendering == step)
            renderer->sem.release();
        lastStepForRendering=-1;
    }
}

void RenderThread::ResetSteps()
{
    mutex.lockForWrite();
//    foreach(SolverNode *node, listOfSteps) {
//        delete node;
//    }
    listOfSteps.clear();
    mutex.unlock();
}

