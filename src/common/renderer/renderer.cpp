/**************************************************************************
#    Copyright 2010-2011 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    under the terms of the GNU Lesser General Public License for more details.
#
#    You should have received a copy of the under the terms of the GNU Lesser General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "renderer.h"
#include "pathsolver.h"
#include "connectables/object.h"
#include "solvernode.h"
#include "mainhost.h"

Renderer::Renderer(MainHost *myHost)
    : QObject(),
      numberOfThreads(0),
      numberOfSteps(0),
      stop(false),
      sem(0),
      myHost(myHost)
{
    maxNumberOfThreads = myHost->GetSetting("MaxNbThreads", 4).toInt();
    if(maxNumberOfThreads<=0) maxNumberOfThreads = 4;
    InitThreads();
}

Renderer::~Renderer()
{
    Clear();
}

void Renderer::InitThreads()
{
    for(int i=0; i<maxNumberOfThreads; i++) {
        RenderThread *th = new RenderThread(this, QString::number(i));
        listOfThreads << th;
        th->start(QThread::TimeCriticalPriority);
    }
}

void Renderer::Clear()
{
    mutex.lock();
    stop=true;
    numberOfThreads=0;
    numberOfSteps=0;
    foreach(RenderThread *th, listOfThreads) {
        th->Stop();
    }
    listOfThreads.clear();
    mutex.unlock();
}

void Renderer::SetNbThreads(int nbThreads)
{
    Clear();

    mutex.lock();
    maxNumberOfThreads = nbThreads;
    if(maxNumberOfThreads<=0) maxNumberOfThreads = 4;
    InitThreads();
    stop=false;
    mutex.unlock();
}

void Renderer::OnNewRenderingOrder(orderedNodes *newSteps)
{
    mutex.lock();
    if(stop) {
        mutex.unlock();
        return;
    }

    int step = -1;
    int threadCount = -1;

    numberOfSteps = step;
    numberOfThreads = threadCount;

    foreach(RenderThread *th, listOfThreads) {
        th->ResetSteps();
    }

    orderedNodes::iterator i = newSteps->begin();
    while (i != newSteps->end()) {

        if(step==i.key()) {
            threadCount++;
            if(threadCount>=maxNumberOfThreads)
                threadCount=0;
        } else {
            threadCount=0;
        }

        if(threadCount>numberOfThreads)
            numberOfThreads=threadCount;

        step = i.key();
        if(step>numberOfSteps)
            numberOfSteps=step;

        listOfThreads.value(threadCount)->SetStep( step, i.value()->objectPtr );
        ++i;
    }

    numberOfSteps++;
    numberOfThreads++;

    mutex.unlock();
}

void Renderer::StartRender()
{

    if(!mutex.tryLock()) {
        debug2(<<"Renderer::StartRender can't lock")
        return;
    }

    if(stop) {
        mutex.unlock();
        return;
    }

    if(numberOfThreads==0) {
        mutex.unlock();
        return;
    }

    sem.release(numberOfThreads);
    for(int currentStep=-1; currentStep<numberOfSteps; currentStep++) {
        sem.acquire(numberOfThreads);
        for(int i=0;i<numberOfThreads; i++) {
            RenderThread *th = listOfThreads.value(i);
            th->step = currentStep;
            th->sem.release();
        }
    }
    sem.acquire(numberOfThreads);
    mutex.unlock();
}

