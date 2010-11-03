/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef RENDERER_H
#define RENDERER_H

#include "precomp.h"
#include "connectables/object.h"
class SolverNode;

typedef QMultiHash<int, SolverNode*> orderedNodes;
typedef QMultiHash<int, QSharedPointer<Connectables::Object> > orderedObjects;

class Renderer : public QThread
{
    Q_OBJECT

public:
    Renderer(QObject *parent = 0);

    void run();
    QMutex mutexRender;

protected:
    orderedObjects renderSteps;

public slots:
    void Clear();
    void StartRender();
    void OnNewRenderingOrder(orderedNodes *newSteps);

};

#endif // RENDERER_H
