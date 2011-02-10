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

#ifndef CONNECTABLEOBJECTVIEW_H
#define CONNECTABLEOBJECTVIEW_H

#include "objectview.h"
#include "cableview.h"

class MainHost;
namespace View {

    class ConnectableObjectView : public ObjectView
    {
    Q_OBJECT
    public:
        ConnectableObjectView(MainHost *myHost,QAbstractItemModel *model,QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);


    protected:
//        WId backOri;

        QList<CableView*>listCables;

    signals:
        void CommandStep(ConnectionInfo pinInfo, int delta);

//    public slots:
//        void SetBackground();
    };

}

#endif // CONNECTABLEOBJECTVIEW_H
