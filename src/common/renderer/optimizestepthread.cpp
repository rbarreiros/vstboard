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

bool OptimizeStepThread::GetMergedNode(RendererNode **node)
{
    if(listOfNodes.isEmpty()) {
        return false;
    }

    QList<RendererNode*>newLst = listOfNodes;
    RendererNode *n = newLst.takeFirst();
    n->ClearMergedNodes();

    while(!newLst.isEmpty()) {
        RendererNode *merged = newLst.takeFirst();
        merged->ClearMergedNodes();
        n->AddMergedNode( merged );
    }

    *node=n;

    return true;
}
