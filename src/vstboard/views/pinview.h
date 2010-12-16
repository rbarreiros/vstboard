/******************************************************************************
#    Copyright 2010 Raphaël François
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
******************************************************************************/
#ifndef PINVIEW_H
#define PINVIEW_H

#include "../precomp.h"
#include "../globals.h"
#include "../connectables/connectioninfo.h"


namespace Connectables {
    class Pin;
}

namespace View {
    class CableView;
    class ConnectableObjectView;
    class PinView : public QGraphicsWidget
    {
    Q_OBJECT

    public:

        explicit PinView(float angle, QAbstractItemModel *model,QGraphicsItem * parent, Connectables::Pin *pin);
        const ConnectionInfo GetConnectionInfo() const {return connectInfo;}
        virtual const QPointF pinPos() const;
        void AddCable(CableView *cable);
        void RemoveCable(CableView *cable);

        virtual void UpdateModelIndex(const QModelIndex &index) {}
        virtual void SetPinModelIndex(QPersistentModelIndex index) {pinIndex = index; UpdateModelIndex(index);}
        void UpdateCablesPosition();

    protected:

        QVariant itemChange ( GraphicsItemChange change, const QVariant & value );
        void mousePressEvent ( QGraphicsSceneMouseEvent * event );
        void mouseMoveEvent ( QGraphicsSceneMouseEvent  * event );
        void mouseReleaseEvent ( QGraphicsSceneMouseEvent  * event );
        void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
        void dragLeaveEvent( QGraphicsSceneDragDropEvent  * event );
        void dragMoveEvent ( QGraphicsSceneDragDropEvent * event );
        void dropEvent ( QGraphicsSceneDragDropEvent  * event );
        void polishEvent();

        QAbstractGraphicsShapeItem *rectBgnd;

        static QBrush highlightBrush;

        QList<CableView *> connectedCables;

        ConnectionInfo connectInfo;

        static QGraphicsLineItem *currentLine;
        void CreateMimeData(QByteArray &bytes);
        void ReadMimeData(QByteArray &bytes, ConnectionInfo &data);
        float falloffSpeed;

        QAbstractItemModel *model;
        QPersistentModelIndex pinIndex;

        float pinAngle;

    signals:
        void ConnectPins(ConnectionInfo outputPin, ConnectionInfo inputPin);
        void RemoveCablesFromPin(ConnectionInfo pin);

    public slots:
        virtual void updateVu(){}

    friend class Cable;
    };
}

#endif // PINVIEW_H
