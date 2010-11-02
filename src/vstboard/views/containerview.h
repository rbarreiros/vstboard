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

#ifndef CONTAINERVIEW_H
#define CONTAINERVIEW_H

#include "objectview.h"
#include "containercontent.h"
#include "cableview.h"
#include "bridgeview.h"

namespace View {

    class BridgePinView;
    class ContainerView : public ObjectView
    {
    Q_OBJECT
    public:
        explicit ContainerView(QAbstractItemModel *model, QGraphicsItem *parent = 0);
        virtual ~ContainerView();

        CableView * OnAddConnection(ConnectionInfo pinOut, ConnectionInfo pinIn);
        CableView * OnRemoveConnection(ConnectionInfo pinOut, ConnectionInfo pinIn);
        PinView *FindPin(ConnectionInfo &pin);

        virtual void UpdateModelIndex();

        QPointF GetDropPos();

        BridgeView *bridgeIn;
        BridgeView *bridgeOut;

    protected:
//        void moveEvent ( QGraphicsSceneMoveEvent * event );
        void resizeEvent ( QGraphicsSceneResizeEvent * event );
        void closeEvent ( QCloseEvent * event );
        ContainerContent *content;
        ListPinsView *centerLayout;
        QHash<int,ObjectView*>listChildObjects;
        QList<CableView*>listCables;

//    signals:
//        void AddObject(ObjType::Enum type, int containerId, QPointF pos, int id=0, QString name="");

    };
}

#endif // CONTAINERVIEW_H
