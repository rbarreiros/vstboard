#include "optimizerstep.h"
#include "optimizer.h"

OptimizerStep::OptimizerStep(int step, int nbThreads, Optimizer *parent) :
    step(step),
    nbThreads(nbThreads),
    parent(parent)
{
}

void OptimizerStep::AddNode(SolverNode *node)
{
    listOfNodes << node;
    node->minRenderOrder = node->maxRenderOrder = step;
}

void OptimizerStep::SetNbThreads(int nb)
{
    nbThreads = nb;
}

void OptimizerStep::MapOnThreadsRandomly()
{
    threadsMap.clear();
    QList<SolverNode*>ln = listOfNodes;
    int currentThread = 0;

    while(!ln.isEmpty()) {
        threadsMap.insert(currentThread, ln.takeFirst());

        currentThread++;
        if(currentThread==nbThreads)
            currentThread=0;
    }
}

void OptimizerStep::Optimize()
{
    qSort(listOfNodes.begin(), listOfNodes.end(), CompareNodeSpeed);

    threadsMap.clear();
    foreach(SolverNode *node, listOfNodes) {
        node->ClearMergedNodes();
    }

    QList<SolverNode*>newLstNodes = listOfNodes;

    //put biggest nodes in threads
    for(int th=0; th<nbThreads; th++) {
        threadsMap.insert(th, newLstNodes.takeLast());
    }

    //add remaining nodes
    int th=nbThreads-1;
    while(!newLstNodes.isEmpty()) {
        threadsMap.insert(th, newLstNodes.takeLast());

        th--;
        if(th<0)
            th=nbThreads-1;
    }

}

bool OptimizerStep::CompareNodeSpeed(SolverNode *n1, SolverNode *n2)
{
    return n1->cpuTime < n2->cpuTime;
}
