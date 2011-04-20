#ifndef OPTIMIZERSTEP_H
#define OPTIMIZERSTEP_H

#include "optimizestepthread.h"

class Optimizer;
class OptimizerStep
{
public:
    OptimizerStep(int step, int nbThreads, Optimizer *parent);
    OptimizerStep(const OptimizerStep &s);
    ~OptimizerStep();
    void ClearThreads();
    void CreateThreads();
    void AddNode(SolverNode *node);
    void MapOnThreadsRandomly();
    void Optimize();
    void OptimizeSpannedNodes();
    void SetNbThreads(int nb);
    SolverNode *GetNode(int thread);

    static bool CompareNodeSpeed(SolverNode *n1, SolverNode *n2);
    static bool CompareThreadSpeed(OptimizeStepThread *t1, OptimizeStepThread *t2);

protected:
    bool NextStepCanAcceptSpannedNode(SolverNode *node, unsigned long cpuGainForPreviousStep);
    bool NextStepCanAcceptPostponedNode(SolverNode *node, unsigned long cpuGainForPreviousStep);
    QList<SolverNode*>listOfNodes;
    QList<OptimizeStepThread*>listOfThreads;
    int step;
    int nbThreads;
    Optimizer *parent;
    OptimizerStep *nextStep;
};

#endif // OPTIMIZERSTEP_H
