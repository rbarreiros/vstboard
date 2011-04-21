#ifndef RENDERERNODE_H
#define RENDERERNODE_H

#include "node.h"

class SolverNode;
class RendererNode : public Node
{
public:
    RendererNode();
    RendererNode(const SolverNode &c);
    RendererNode(const RendererNode &c);
    ~RendererNode();

    void NewRenderLoop();
    void Render();
    void AddMergedNode(RendererNode *merged);
    void RemoveMergedNode(RendererNode *merged);
    void ClearMergedNodes();
    QList<RendererNode*> GetListOfMergedNodes() const {return listOfMergedNodes;}
    void UpdateModel(QStandardItemModel *model);
    long GetTotalCpuUsage();

    int minRenderOrderOri;
    int maxRenderOrderOri;
    QPersistentModelIndex modelIndex;
    bool modelNeedUpdate;
    long cpuTime;

protected:
    int benchCount;
    QList<RendererNode*>listOfMergedNodes;
//    QReadWriteLock mutex;
};

#endif // RENDERERNODE_H
