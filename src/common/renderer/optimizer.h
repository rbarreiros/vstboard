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

protected:
    void Clear();

    QMap<int,OptimizerStep*>listOfSteps;
    int nbThreads;

};

#endif // OPTIMIZER_H
