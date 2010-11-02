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

#ifndef SOLVERSCENE_H
#define SOLVERSCENE_H

#include "../precomp.h"

class SolverNode;
typedef QMultiHash<int, SolverNode*> orderedNodes;

namespace View {

    class SolverScene : public QGraphicsScene
    {
    Q_OBJECT
    public:
        explicit SolverScene(QObject *parent = 0);

    signals:

    public slots:
        void NewSolver(orderedNodes* nodes);

    };
}



#endif // SOLVERSCENE_H
