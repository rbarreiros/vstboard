#include "renderthread.h"
#include "renderer.h"

RenderThread::RenderThread(Renderer *renderer, const QString &name)
    : QThread(renderer),
      renderer(renderer),
      sem(0),
      stop(false)
{
    setObjectName(name);
}

RenderThread::~RenderThread()
{
    debug2(<<"delete thread " << objectName())
}

void RenderThread::run()
{
    forever {
        sem.acquire();
        mutex.lock();
        if(stop)
            return;
        RenderStep(step);
        renderer->sem.release();
        mutex.unlock();
    }
}

void RenderThread::Stop()
{
    mutex.lock();
    stop=true;
    mutex.unlock();
    sem.release(1);
}

void RenderThread::RenderStep(int step)
{
    if(step==-1) {
        //reset counters
        QMap<int, QWeakPointer<Connectables::Object> >::iterator i = listOfSteps.begin();
        while (i != listOfSteps.end()) {
            if(!i.value().isNull()) {
                QSharedPointer<Connectables::Object>objPtr = i.value().toStrongRef();
                if(!objPtr->GetSleep()) {
                    objPtr->NewRenderLoop();
                }
            }
            ++i;
        }
        return;
    }

    if(!listOfSteps.contains(step))
        return;

    QSharedPointer<Connectables::Object>objPtr = listOfSteps.value(step).toStrongRef();
    if(objPtr.isNull() || objPtr->GetSleep())
        return;
    objPtr->Render();
}

void RenderThread::SetStep(int step, QWeakPointer<Connectables::Object>obj)
{
    mutex.lock();
    listOfSteps.insert(step, obj);
    mutex.unlock();
}

void RenderThread::ResetSteps()
{
    mutex.lock();
    listOfSteps.clear();
    mutex.unlock();
}

