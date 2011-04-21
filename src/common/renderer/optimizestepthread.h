#ifndef OPTIMIZESTEPTHREAD_H
#define OPTIMIZESTEPTHREAD_H

#include "renderernode.h"

class OptimizeStepThread
{
public:
    OptimizeStepThread();
    OptimizeStepThread(const OptimizeStepThread &th);
    bool GetMergedNode(RendererNode **node);

    QList<RendererNode*>listOfNodes;
    long cpuTime;
    RendererNode* spanFor;
};

#endif // OPTIMIZESTEPTHREAD_H
