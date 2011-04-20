#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "optimizerstep.h"

class Optimizer
{
public:
    Optimizer();
    ~Optimizer();
    void SetNbThreads(int nb);
    void NewListOfNodes(QList<SolverNode*> & listNodes);
    void Optimize();
    OptimizerStep* GetStep(int step) {return listOfSteps.value(step,0); }
    void SetStep(int step, OptimizerStep* s) {listOfSteps.insert(step,s); }
    QMap<int, SolverNode* > GetListOfNode(int thread);

protected:
    void Clear();
    QMap<int,OptimizerStep*>listOfSteps;
    int nbThreads;

};

#endif // OPTIMIZER_H
