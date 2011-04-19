#ifndef OPTIMIZERSTEP_H
#define OPTIMIZERSTEP_H

#include "solvernode.h"

class Optimizer;
class OptimizerStep
{
public:
    OptimizerStep(int step, int nbThreads, Optimizer *parent);
    void AddNode(SolverNode *node);
    void MapOnThreadsRandomly();
    void Optimize();
    void SetNbThreads(int nb);
    static bool CompareNodeSpeed(SolverNode *n1, SolverNode *n2);

protected:
    QList<SolverNode*>listOfNodes;
    QMultiMap<int,SolverNode*>threadsMap;
    int step;
    int nbThreads;
    Optimizer *parent;
};

#endif // OPTIMIZERSTEP_H
