#include "optimizer.h"
#include "renderernode.h"

Optimizer::Optimizer() :
    nbThreads(0)
{
}

Optimizer::~Optimizer()
{
    Clear();
}

void Optimizer::SetNbThreads(int nb)
{
    mutex.lock();
    nbThreads=nb;
    foreach(OptimizerStep *st, listOfSteps) {
        st->SetNbThreads(nb);
    }
    mutex.unlock();
}

void Optimizer::Clear()
{
    foreach(OptimizerStep *st, listOfSteps) {
        st->ClearNodes();
        delete st;
    }
    listOfSteps.clear();
}

void Optimizer::NewListOfNodes(const QList<RendererNode*> & listNodes)
{
    mutex.lock();
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
    mutex.unlock();
}

OptimizerStep* Optimizer::GetStep(int step)
{
    return listOfSteps.value(step,0);
}

void Optimizer::SetStep(int step, OptimizerStep* s)
{
    listOfSteps.insert(step,s);
}

void Optimizer::Optimize()
{
    mutex.lock();
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
    mutex.unlock();
}

QMap<int, RendererNode* > Optimizer::GetThreadNodes(int thread)
{
    mutex.lock();
    QMap<int, RendererNode* >lstSteps;

    QMap<int,OptimizerStep*>::iterator i = listOfSteps.begin();
    while(i!=listOfSteps.end()) {
        OptimizerStep* step = i.value();
        RendererNode *node=0;
        if( step->GetMergedNode(thread, &node) )
            lstSteps.insert(i.key(), node );
        ++i;
    }
    mutex.unlock();
    return lstSteps;
}

QList<RendererNode*> Optimizer::GetListOfNodes()
{
    mutex.lock();
    QList<RendererNode*>newList;
    QMap<int,OptimizerStep*>::iterator i = listOfSteps.begin();
    while(i!=listOfSteps.end()) {
        OptimizerStep* step = i.value();
        newList << step->GetListOfNodes();
        ++i;
    }
    mutex.unlock();
    return newList;
}

void Optimizer::BuildModel( QStandardItemModel *model )
{
    mutex.lock();
    model->clear();

    QMap<int,OptimizerStep*>::iterator i = listOfSteps.begin();
    while(i!=listOfSteps.end()) {
        OptimizerStep* s = i.value();
        s->AddToModel( model );
        ++i;
    }
    mutex.unlock();
}

void Optimizer::UpdateView( QStandardItemModel *model )
{
    mutex.lock();
    QMap<int,OptimizerStep*>::iterator i = listOfSteps.begin();
    while(i!=listOfSteps.end()) {
        OptimizerStep* s = i.value();
        s->UpdateView( model );
        ++i;
    }
    mutex.unlock();
}

