/**************************************************************************
#    Copyright 2010 Rapha�l Fran�ois
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

#ifndef CURSORVIEW_H
#define CURSORVIEW_H

#include "precomp.h"

namespace View {

    class CursorView : public QGraphicsWidget
    {
        Q_OBJECT
    public:
        explicit CursorView(QAbstractItemModel *model,bool upsideDown,QGraphicsItem *parent = 0);
        QRectF	boundingRect () const;
        void SetValue(float newVal);
        void SetModelIndex(QPersistentModelIndex index) {modelIndex=index;}

    protected:
        void ValueChanged(float newVal);
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        void mousePressEvent ( QGraphicsSceneMouseEvent * event );
        void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
        QGraphicsPolygonItem *cursor;
        bool upsideDown;
        bool drag;
        float value;
        QAbstractItemModel *model;
        QPersistentModelIndex modelIndex;

    signals:

    public slots:

    };
}
#endif // CURSORVIEW_H
