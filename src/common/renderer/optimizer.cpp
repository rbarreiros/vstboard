#include "optimizer.h"

Optimizer::Optimizer()
{
}

Optimizer::~Optimizer()
{
    Clear();
}

void Optimizer::SetNbThreads(int nb)
{
    nbThreads=nb;
    foreach(OptimizerStep *st, listOfSteps) {
        st->SetNbThreads(nb);
    }
}

void Optimizer::Clear()
{
    foreach(OptimizerStep *st, listOfSteps) {
        delete st;
    }
    listOfSteps.clear();
}

void Optimizer::NewListOfNodes(const QList<RendererNode*> & listNodes)
{
    Clear();

    foreach(RendererNode *node, listNodes) {
        node->minRenderOrder = node->minRenderOrderOri;
        node->maxRenderOrder = node->maxRenderOrderOri;

        int step = node->minRenderOrder;
        OptimizerStep *oStep = listOfSteps.value(step,0);
        if(!oStep) {
            oStep = new OptimizerStep(step,nbThreads,this);
            listOfSteps.insert(step,oStep);
        }

        oStep->AddNode(node);
    }

    foreach(OptimizerStep *step, listOfSteps) {
        step->MapOnThreadsRandomly();
    }
}

void Optimizer::Optimize()
{
    foreach(OptimizerStep *step, listOfSteps) {
        if(step)
            step->Optimize();
    }

    QMap<int,OptimizerStep*>::iterator i = listOfSteps.begin();
    while(i!=listOfSteps.end()) {
        OptimizerStep* s = i.value();
        s->OptimizeSpannedNodes();
        ++i;
    }
}

QMap<int, RendererNode* > Optimizer::GetListOfNode(int thread)
{
    QMap<int, RendererNode* >lstSteps;

    QMap<int,OptimizerStep*>::iterator i = listOfSteps.begin();
    while(i!=listOfSteps.end()) {
        OptimizerStep* step = i.value();
        RendererNode *node=0;
        if( step->GetNode(thread, &node) )
            lstSteps.insert(i.key(), node );
        ++i;
    }
    return lstSteps;
}
