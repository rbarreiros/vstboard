#ifndef OPTIMIZESTEPTHREAD_H
#define OPTIMIZESTEPTHREAD_H

#include "solvernode.h"

class OptimizeStepThread
{
public:
    OptimizeStepThread();
    OptimizeStepThread(const OptimizeStepThread &th);
    SolverNode *GetMergedNode();

    QList<SolverNode*>listOfNodes;
    unsigned long cpuTime;
    SolverNode* spanFor;
};

#endif // OPTIMIZESTEPTHREAD_H
