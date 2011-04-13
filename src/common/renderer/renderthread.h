#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "connectables/object.h"
#include <QSemaphore>

class Renderer;
class RenderThread : public QThread
{
    Q_OBJECT

public:
    RenderThread(Renderer *renderer, const QString &name);
    ~RenderThread();

    void SetStep(int step, QWeakPointer<Connectables::Object>obj);
    void ResetSteps();

    void run();
    void Stop();

protected:
    QMap<int, QWeakPointer<Connectables::Object> > listOfSteps;
    Renderer *renderer;
    QMutex mutex;
    QSemaphore sem;
    int step;
    bool stop;

signals:
    void StepRendered();

public slots:
    void RenderStep(int step);

friend class Renderer;
};

#endif // RENDERTHREAD_H
