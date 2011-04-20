#include "optimizerstep.h"
#include "optimizer.h"

OptimizerStep::OptimizerStep(int step, int nbThreads, Optimizer *parent) :
    step(step),
    nbThreads(nbThreads),
    parent(parent),
    nextStep(0)
{
}

OptimizerStep::OptimizerStep(const OptimizerStep &s) :
    listOfNodes(s.listOfNodes),
    step(s.step),
    nbThreads(s.nbThreads),
    parent(s.parent),
    nextStep(s.nextStep)
{
    foreach(OptimizeStepThread *th, s.listOfThreads) {
        listOfThreads << new OptimizeStepThread(*th);
    }
}

OptimizerStep::~OptimizerStep()
{
    ClearThreads();
}

void OptimizerStep::ClearThreads()
{
    foreach(OptimizeStepThread* th, listOfThreads) {
        delete th;
    }
    listOfThreads.clear();
}

void OptimizerStep::CreateThreads()
{
    ClearThreads();
    for(int t=0; t<nbThreads; t++) {
        listOfThreads << new OptimizeStepThread();
    }
}

void OptimizerStep::AddNode(SolverNode *node)
{
    listOfNodes << node;
    node->minRenderOrderOri = node->minRenderOrder;
    node->maxRenderOrderOri = node->maxRenderOrder;
    node->minRenderOrder = node->maxRenderOrder = step;
}

void OptimizerStep::SetNbThreads(int nb)
{
    nbThreads = nb;
}

void OptimizerStep::MapOnThreadsRandomly()
{
    CreateThreads();

    int currentThread = 0;
    QList<SolverNode*>ln = listOfNodes;

    while(!ln.isEmpty()) {
        OptimizeStepThread *th = listOfThreads.value(currentThread);
        SolverNode *node = ln.takeFirst();
        th->listOfNodes << node;

        currentThread++;
        if(currentThread==nbThreads)
            currentThread=0;
    }
}

void OptimizerStep::Optimize()
{
    //sort nodes by cpu usage
    qSort(listOfNodes.begin(), listOfNodes.end(), CompareNodeSpeed);

    CreateThreads();
    QList<SolverNode*>newLstNodes = listOfNodes;

    while(!newLstNodes.isEmpty()) {

        //add biggest node to the smallest thread
        OptimizeStepThread *th = listOfThreads.first();
        SolverNode *node = newLstNodes.takeLast();
        th->listOfNodes << node;
        th->cpuTime+=node->cpuTime;

        //sort threads by cpu usage
        qSort(listOfThreads.begin(), listOfThreads.end(), CompareThreadSpeed);
    }

}

void OptimizerStep::OptimizeSpannedNodes()
{
    if(!parent)
        return;

    nextStep = parent->GetStep(step+1);
    if(!nextStep)
        return;

    foreach(SolverNode *node, listOfNodes) {
        if(node->maxRenderOrderOri > step) {
            //this node can be postpone or spanned

            //the spanned node need one thread, create a temporary step with one less thread and add all other nodes
            OptimizerStep tmpStep(*this);
            tmpStep.SetNbThreads(nbThreads-1);
            tmpStep.listOfNodes.removeAll(node);
            tmpStep.Optimize();

            //the resulting cpu usage :
            unsigned long tmpCpuTime=tmpStep.listOfThreads.last()->cpuTime;

            if(tmpCpuTime < node->cpuTime) {
                //the spanned node is longer than the other threads, really need to be spanned

                unsigned long cpuGainForThisStep = node->cpuTime - tmpCpuTime;

                //does the next step accept this node ?
                if(NextStepCanAcceptPostponedNode(node, cpuGainForThisStep)) {
                    listOfNodes.removeAll(node);
                    ClearThreads();
                    foreach(OptimizeStepThread *th, tmpStep.listOfThreads) {
                        listOfThreads << new OptimizeStepThread(*th);
                    }

                } else {
                    //try to span this node
                    if(NextStepCanAcceptSpannedNode(node, cpuGainForThisStep)) {
                        listOfNodes.removeAll(node);
                        nbThreads--;
                        OptimizeStepThread *th = new OptimizeStepThread();
                        th->listOfNodes << node;
                        tmpStep.listOfThreads << th;
                        node->maxRenderOrder = step+1;
                        ClearThreads();
                        foreach(OptimizeStepThread *th, tmpStep.listOfThreads) {
                            listOfThreads << new OptimizeStepThread(*th);
                        }
                    }
                }
            }
        }
    }
}

bool OptimizerStep::NextStepCanAcceptSpannedNode(SolverNode *node, unsigned long cpuGainForPreviousStep)
{
    OptimizerStep *tmpStep = new OptimizerStep(*nextStep);
    tmpStep->SetNbThreads( nextStep->nbThreads-1 );
    tmpStep->Optimize();
    unsigned long newCpuTime = tmpStep->listOfThreads.last()->cpuTime;
    unsigned long oldCpuTime = nextStep->listOfThreads.last()->cpuTime;
    unsigned long cpuGain = cpuGainForPreviousStep - (newCpuTime - oldCpuTime);
    if(cpuGain>0) {
        tmpStep->SetNbThreads( tmpStep->nbThreads+1 );
        OptimizeStepThread *th = new OptimizeStepThread();
        th->spanFor = node;
        tmpStep->listOfThreads << th;
        parent->SetStep(step+1, tmpStep);
        delete nextStep;
        nextStep=tmpStep;
        return true;
    }
    delete tmpStep;
    return false;
}

bool OptimizerStep::NextStepCanAcceptPostponedNode(SolverNode *node, unsigned long cpuGainForPreviousStep)
{
    OptimizerStep *tmpStep = new OptimizerStep(*nextStep);
    tmpStep->listOfNodes << node;
    tmpStep->Optimize();
    unsigned long newCpuTime = tmpStep->listOfThreads.last()->cpuTime;
    unsigned long oldCpuTime = nextStep->listOfThreads.last()->cpuTime;
    unsigned long cpuGain = cpuGainForPreviousStep - (newCpuTime - oldCpuTime);
    if(cpuGain>0) {
        parent->SetStep(step+1, tmpStep);
        delete nextStep;
        nextStep=tmpStep;
        node->minRenderOrder = node->maxRenderOrder = step+1;
        return true;
    }
    delete tmpStep;
    return false;
}

SolverNode * OptimizerStep::GetNode(int thread)
{
    if(listOfThreads.size()<=thread) {
        return 0;
    }

    OptimizeStepThread *th = listOfThreads.at(thread);
    return th->GetMergedNode();
}

bool OptimizerStep::CompareNodeSpeed(SolverNode *n1, SolverNode *n2)
{
    return n1->cpuTime < n2->cpuTime;
}

bool OptimizerStep::CompareThreadSpeed(OptimizeStepThread *t1, OptimizeStepThread *t2)
{
    return t1->cpuTime < t2->cpuTime;
}
