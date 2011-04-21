#include "renderernode.h"
#include "solvernode.h"

RendererNode::RendererNode() :
    Node(),
    minRenderOrderOri(-1),
    maxRenderOrderOri(-1),
    modelNeedUpdate(true),
    cpuTime(0),
    benchCount(0)
{
}

RendererNode::RendererNode(const SolverNode &c) :
    Node(c),
    minRenderOrderOri(c.minRenderOrder),
    maxRenderOrderOri(c.maxRenderOrder),
    modelNeedUpdate(true),
    cpuTime(0),
    benchCount(0)
{
}

RendererNode::RendererNode(const RendererNode &c) :
    Node(c),
    minRenderOrderOri(c.minRenderOrderOri),
    maxRenderOrderOri(c.maxRenderOrderOri),
    modelNeedUpdate(true),
    cpuTime(c.cpuTime),
    benchCount(c.benchCount)
{
    foreach( RendererNode *merged, c.listOfMergedNodes) {
        listOfMergedNodes << new RendererNode(*merged);
    }
}

RendererNode::~RendererNode()
{
    listOfMergedNodes.clear();
}

void RendererNode::NewRenderLoop()
{
//    mutex.lockForRead();
    foreach( QWeakPointer<Connectables::Object> ObjPtr, listOfObj) {
        if(!ObjPtr.isNull()) {
            ObjPtr.toStrongRef()->NewRenderLoop();
        }
    }
    foreach(RendererNode *mergedNode, listOfMergedNodes) {
        mergedNode->NewRenderLoop();
    }
//    mutex.unlock();
}

void RendererNode::Render()
{
//    mutex.lockForRead();

#ifdef _WIN32
    unsigned long timerStart=0;
    FILETIME creationTime, exitTime, kernelTime, userTime;

//    if( benchCount<10) {
        if( GetThreadTimes( GetCurrentThread(), &creationTime, &exitTime, &kernelTime, &userTime) !=0 ) {
            timerStart = kernelTime.dwLowDateTime + userTime.dwLowDateTime;
        }
//    }

#endif

    foreach( QSharedPointer<Connectables::Object> objPtr, listOfObj) {
        if(!objPtr.isNull() && !objPtr->GetSleep()) {
            objPtr->Render();
        }
    }


#ifdef _WIN32
//    if(benchCount<10) {
//        FILETIME creationTime, exitTime, kernelTime, userTime;
        if( GetThreadTimes( GetCurrentThread(), &creationTime, &exitTime, &kernelTime, &userTime) !=0 ) {
            cpuTime += ( (kernelTime.dwLowDateTime + userTime.dwLowDateTime) - timerStart )/1000;
            benchCount++;
            modelNeedUpdate=true;
        }
//    }
#endif

    foreach(RendererNode *mergedNode, listOfMergedNodes) {
        mergedNode->Render();
    }
//    mutex.unlock();
}

long RendererNode::GetTotalCpuUsage()
{
    long cpu = cpuTime;

//    mutex.lockForRead();
    foreach(RendererNode *merged, listOfMergedNodes) {
        cpu+=merged->cpuTime;
    }
//    mutex.unlock();

    return cpu;
}

void RendererNode::AddMergedNode(RendererNode *merged)
{
//    mutex.lockForWrite();
    listOfMergedNodes << merged;
    minRenderOrder = merged->minRenderOrder = std::max(minRenderOrder, merged->minRenderOrder);
    maxRenderOrder = merged->maxRenderOrder = std::min(maxRenderOrder, merged->maxRenderOrder);
//    mutex.unlock();
}

void RendererNode::RemoveMergedNode(RendererNode *merged)
{
//    mutex.lockForWrite();
    listOfMergedNodes.removeAll(merged);
//    mutex.unlock();
}

void RendererNode::ClearMergedNodes()
{
//    mutex.lockForWrite();
    listOfMergedNodes.clear();
//    mutex.unlock();
}

void RendererNode::UpdateModel(QStandardItemModel *model)
{
    if(!modelNeedUpdate || !modelIndex.isValid())
        return;

//    mutex.lockForRead();

    modelNeedUpdate=false;

    QString str = QString("[%1:%2][%3:%4](%5)")
                    .arg(minRenderOrder)
                    .arg(maxRenderOrder)
                    .arg(minRenderOrderOri)
                    .arg(maxRenderOrderOri)
                    .arg(cpuTime);

    foreach( QSharedPointer<Connectables::Object> objPtr, listOfObj) {
        if(!objPtr.isNull() && !objPtr->GetSleep()) {
            str.append("\n" + objPtr->objectName());
        }
    }

    model->setData(modelIndex, str);

    foreach(RendererNode *mergedNode, listOfMergedNodes) {
        mergedNode->UpdateModel(model);
    }

//    mutex.unlock();
}
