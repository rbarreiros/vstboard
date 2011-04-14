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

void RenderThread::run()
{
    forever {
        sem.acquire();

        mutex.lock();
        if(stop)
            return;
        mutex.unlock();

        RenderStep(step);

    }
}

void RenderThread::Stop()
{
    mutex.lock();
    stop=true;
    mutex.unlock();
    sem.release(1);
}

void RenderThread::RenderStep(int step)
{
    if(step==-1) {
        mutex.lock();
        //reset counters
        QMap<int, SolverNode* >::iterator i = listOfSteps.begin();
        while (i != listOfSteps.end()) {
            SolverNode *node = i.value();
            if(node && !node->objectPtr.isNull() && !node->objectPtr->GetSleep()) {
                node->objectPtr->NewRenderLoop();
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
        if(!n->objectPtr.isNull() && !n->objectPtr->GetSleep())
            n->objectPtr->Render();

        if(lastStepForRendering == step)
            renderer->sem.release();
        lastStepForRendering=-1;
    }
}

bool RenderThread::SetStep(SolverNode *node, bool strict)
{
    mutex.lock();

    if(strict) {
        for(int i=node->minRenderOrder; i<=node->maxRenderOrder; i++) {
            if(listOfSteps.contains(i)) {
                mutex.unlock();
                return false;
            }
        }
    } else {
        bool found=false;

        //find an empty step
        for(int i=node->minRenderOrder; i<=node->maxRenderOrder; i++) {
            if(listOfSteps.value(i,0)==0) {

                //correct the node before
                int j=i;
                bool f2=false;
                while(!f2 && j>=0) {
                    SolverNode *oldNode = listOfSteps.value(j,0);
                    if(oldNode!=0) {
                        oldNode->maxRenderOrder=node->minRenderOrder-1;
                        f2=true;
                    }
                    j--;
                }

                found=true;
            }
        }
        if(!found) {
            mutex.unlock();
            return false;
        }
    }

    SolverNode *n = new SolverNode();
    n->objectPtr = node->objectPtr;
    n->minRenderOrder = node->minRenderOrder;
    n->maxRenderOrder = node->maxRenderOrder;


    listOfSteps.insert(node->minRenderOrder, n);
    for(int i=node->minRenderOrder+1; i<=node->maxRenderOrder; i++) {
        listOfSteps.insert(i,0);
    }
    mutex.unlock();
    return true;
}

void RenderThread::ResetSteps()
{
    mutex.lock();
    foreach(SolverNode *node, listOfSteps) {
        if(node)
            delete node;
    }
    listOfSteps.clear();
    mutex.unlock();
}

