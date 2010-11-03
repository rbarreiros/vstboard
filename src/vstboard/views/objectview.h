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

#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H

#include "../precomp.h"
#include "../globals.h"
#include "../connectables/object.h"
#include "listpinsview.h"
#include "textbutton.h"

namespace View {

    class ConnectableView;
    class PinView;
    class ObjectView : public QGraphicsWidget
    {
    Q_OBJECT
    public:
        explicit ObjectView(QAbstractItemModel *model, QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
        virtual ~ObjectView();
        void setPen(const QPen &pen);

        virtual void SetModelIndex(QPersistentModelIndex index);
        virtual void UpdateModelIndex();
        void SetEditorIndex(QPersistentModelIndex index);

    protected:
        void resizeEvent ( QGraphicsSceneResizeEvent * event );
        virtual void closeEvent ( QCloseEvent * event );
        QVariant itemChange ( GraphicsItemChange  change, const QVariant & value );
        bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event );
        QPen pen;

        void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent  * event );
        void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

        QGraphicsSimpleTextItem *titleText;
        QGraphicsRectItem *border;
        QGraphicsPixmapItem *backgroundImg;

        QMap<ConnectionInfo, PinView*>listPins;

        TextButton *editorButton;

        QGraphicsGridLayout *layout;

        QAbstractItemModel *model;
        QPersistentModelIndex objIndex;
        QPersistentModelIndex editorIndex;

    friend class PinView;
    };
}

#endif // OBJECTVIEW_H
