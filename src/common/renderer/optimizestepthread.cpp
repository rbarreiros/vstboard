#include "optimizestepthread.h"

OptimizeStepThread::OptimizeStepThread() :
    cpuTime(0),
    spanFor(0)
{
}

OptimizeStepThread::OptimizeStepThread(const OptimizeStepThread &th)
{
    cpuTime = th.cpuTime;
    spanFor = th.spanFor;
    listOfNodes = th.listOfNodes;
}

SolverNode * OptimizeStepThread::GetMergedNode()
{
    if(listOfNodes.isEmpty()) {
        return 0;
    }

    QList<SolverNode*>newLst = listOfNodes;

    SolverNode *node(newLst.takeFirst());
    while(!newLst.isEmpty()) {
        node->AddMergedNode( new SolverNode( *(newLst.takeFirst()) ) );
    }
    return node;
}
