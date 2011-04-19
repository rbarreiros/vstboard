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


bool RenderThread::PostponeNode(SolverNode *node, int minStep)
{
    if(minStep>node->maxRenderOrder)
        return false;

    mutex.lockForWrite();

    for(int i=node->minRenderOrder; i<=minStep; i++) {
        listOfSteps.remove(i);
    }

    node->minRenderOrder=minStep;
    listOfSteps.insert(minStep,node);

    mutex.unlock();
    return true;
}

bool RenderThread::ShortenNode(SolverNode *node, int maxStep)
{
    if(maxStep<node->minRenderOrder)
        return false;

    mutex.lockForWrite();

    for(int i=maxStep+1; i<=node->maxRenderOrder; i++) {
        listOfSteps.remove(i);
    }

    node->maxRenderOrder=maxStep;
    foreach(SolverNode *n, node->GetListOfMergedNodes()) {
        n->maxRenderOrder=maxStep;
    }
    mutex.unlock();
    return true;
}

int RenderThread::NeededModificationsToInsertNode(SolverNode *node, bool apply)
{
    int minStep=node->minRenderOrder;
    int maxStep=node->maxRenderOrder;

    for(int i=minStep; i<=maxStep; i++) {
        SolverNode* nodeInPlace=0;
        int modifiedSteps = 999;
        int bestStep=0;
        insertType preferred = ND;

        mutex.lockForRead();
        if(listOfSteps.contains(i)) {
            nodeInPlace=listOfSteps.value(i,0);
            if(nodeInPlace!=0) {

                //the node in place can be postponed, shorten the new node
                if(nodeInPlace->maxRenderOrder>i) {
                    int newInPlaceMinStep = i+1;
                    int newNodeMaxStep = i;

                    int tmpMods = newInPlaceMinStep - nodeInPlace->minRenderOrder;
                    tmpMods += maxStep - newNodeMaxStep;
                    if(tmpMods < modifiedSteps) {
                        modifiedSteps = tmpMods;
                        preferred = postponeNodeInPlace;
                        bestStep = i;
                    }
                }

            } else {

                //the node in place can be shortened, postpone and shorten the new node
                int newInPlaceMaxStep = i-1;
                int newNodeMinStep = i;
                int newNodeMaxStep = FirstUsedStepInRange(newNodeMinStep, maxStep) - 1;

                nodeInPlace = FindNodeUsingStep(i);
                int tmpMods = nodeInPlace->maxRenderOrder - newInPlaceMaxStep;
                tmpMods += maxStep - newNodeMaxStep;
                tmpMods += newNodeMinStep - minStep;
                if(tmpMods < modifiedSteps) {
                    modifiedSteps = tmpMods;
                    preferred = shortenNodeInPlace;
                    bestStep = i;
                }

            }
        } else {

            //no node in place, postpone and shorten the new node
            int newNodeMinStep = i;
            int newNodeMaxStep = FirstUsedStepInRange(newNodeMinStep, maxStep) - 1;

            int tmpMods = maxStep - newNodeMaxStep;
            tmpMods += newNodeMinStep - minStep;
            if(tmpMods < modifiedSteps) {
                modifiedSteps = tmpMods;
                preferred = postponeNewNode;
                bestStep = i;
            }
        }
        mutex.unlock();

        if(preferred!=ND) {

            if( apply ) {
                switch(preferred) {
                    case postponeNodeInPlace:
                        //the node in place can be postponed, shorten the new node
                        PostponeNode(nodeInPlace, bestStep+1);
                        node->maxRenderOrder = bestStep;
                        break;

                    case shortenNodeInPlace:
                        //the node in place can be shortened, postpone and shorten the new node
                        ShortenNode(nodeInPlace, bestStep-1);
                        node->minRenderOrder = bestStep;
                        node->maxRenderOrder = FirstUsedStepInRange(bestStep, maxStep) - 1;
                        break;

                    case postponeNewNode:
                        //no node in place, postpone and shorten the new node
                        node->minRenderOrder = bestStep;
                        node->maxRenderOrder = FirstUsedStepInRange(bestStep, maxStep) - 1;
                        break;
                }
                SetStep(node);
            }

            return modifiedSteps;
        }

    }
    return -1;
}

int RenderThread::FirstUsedStepInRange(int startStep, int endStep)
{
    for(int i=startStep; i<=endStep; i++) {
        if(listOfSteps.value(i,0) != 0) {
            return i;
        }
    }
    //nothing found, return next step
    return endStep+1;
}

SolverNode* RenderThread::FindNodeUsingStep(int step)
{
    for(int i=step; i>=0; i--) {
        SolverNode *node = listOfSteps.value(i,0);
        if(node != 0) {
            return node;
        }
    }
    return 0;
}

bool RenderThread::SetStep(SolverNode *node)
{
    mutex.lockForRead();
    for(int i=node->minRenderOrder; i<=node->maxRenderOrder; i++) {
        if(listOfSteps.contains(i)) {
            mutex.unlock();
            return false;
        }
    }
    mutex.unlock();

    mutex.lockForWrite();
//    SolverNode *n = new SolverNode(*node);

    listOfSteps.insert(node->minRenderOrder, node);
    for(int i=node->minRenderOrder+1; i<=node->maxRenderOrder; i++) {
        listOfSteps.insert(i,0);
    }
    mutex.unlock();
    return true;
}

void RenderThread::ResetSteps()
{
    mutex.lockForWrite();
    listOfSteps.clear();
    mutex.unlock();
}

