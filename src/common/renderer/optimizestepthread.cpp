#include "optimizestepthread.h"
#include "renderernode.h"

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
    RendererNode *n = new RendererNode( *newLst.takeFirst() );
    n->ClearMergedNodes();

    while(!newLst.isEmpty()) {
        RendererNode *merged = new RendererNode( *newLst.takeFirst() );
        merged->ClearMergedNodes();
        n->AddMergedNode( merged );
    }

    *node=n;

    return true;
}

void OptimizeStepThread::AddToModel(QStandardItemModel *model, int row, int col)
{
    if(spanFor) {
        model->setItem(row,col, new QStandardItem("+") );
        return;
    }

    foreach(RendererNode *node, listOfNodes) {
        if(node) {
            QStandardItem *item = new QStandardItem(".");
            model->setItem(row,col,item);
            node->modelIndex=item->index();
        }
        row++;
    }
}

void OptimizeStepThread::UpdateView( QStandardItemModel *model )
{
    foreach(RendererNode *node, listOfNodes) {
        if(node) {
            node->UpdateModel( model );
        }
    }
}
