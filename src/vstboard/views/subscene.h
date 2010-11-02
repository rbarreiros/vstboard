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

#ifndef SUBSCENE_H
#define SUBSCENE_H

#include "precomp.h"
#include "../globals.h"
#include "bridgeview.h"

namespace View {
    class SubScene : public QGraphicsScene
    {
        Q_OBJECT
    public:
//        explicit SubScene(QAbstractItemModel *model,QObject *parent = 0);
        explicit SubScene(QObject *parent = 0);
//        void SetModelIndex(QPersistentModelIndex index);
//        void setModel ( QAbstractItemModel * model );

//        BridgeView *bridgeIn;
//        BridgeView *bridgeOut;
//        BridgeView *bridgeSend;
//        BridgeView *bridgeReturn;

    protected:
//        void dragEnterEvent( QGraphicsSceneDragDropEvent *event);
//        void dragMoveEvent ( QGraphicsSceneDragDropEvent * event );
//        //void dragLeaveEvent ( QGraphicsSceneDragDropEvent * event );
//        void dropEvent( QGraphicsSceneDragDropEvent *event);

//        QAbstractItemModel *model;
//        QPersistentModelIndex objIndex;

//        QGraphicsGridLayout *layout;
//        QGraphicsRectItem *rootObj;


//    public slots:
//        void OnViewChanged(QRectF rect);
    };
}
#endif // SUBSCENE_H
