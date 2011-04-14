#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "solvernode.h"
#include <QSemaphore>


class Renderer;
class RenderThread : public QThread
{
    Q_OBJECT

public:
    RenderThread(Renderer *renderer, const QString &name);

    bool SetStep(SolverNode *node, bool strict=true);
    void ResetSteps();

    void run();
    void Stop();

protected:
    QMap<int, SolverNode* > listOfSteps;
    Renderer *renderer;
    QMutex mutex;
    QSemaphore sem;
    int step;
    bool stop;
    int lastStepForRendering;

signals:
    void StepRendered();

public slots:
    void RenderStep(int step);

friend class Renderer;
};

#endif // RENDERTHREAD_H
