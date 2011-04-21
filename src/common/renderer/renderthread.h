#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "precomp.h"
#include "renderernode.h"

class Renderer;
class RenderThread : public QThread
{
    Q_OBJECT

public:
    RenderThread(Renderer *renderer, const QString &name);
    ~RenderThread();
    void ResetSteps();
    void run();

protected:
    void Stop();
    void RenderStep(int step);

    QMap<int, RendererNode* > listOfSteps;
    Renderer *renderer;
    QReadWriteLock mutex;

    QSemaphore sem;
    int step;
    bool stop;
    int lastStepForRendering;

friend class Renderer;
};

#endif // RENDERTHREAD_H
