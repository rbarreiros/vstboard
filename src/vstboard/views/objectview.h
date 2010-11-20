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

#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H

#include "../precomp.h"
#include "../globals.h"
#include "../connectables/object.h"
#include "listpinsview.h"
//#include "textbutton.h"

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

    protected:
        void SetBackground(const QString & imgName);
        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
        void resizeEvent ( QGraphicsSceneResizeEvent * event );
        virtual void closeEvent ( QCloseEvent * event );
        QVariant itemChange ( GraphicsItemChange  change, const QVariant & value );
        QPen pen;

        void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

        QGraphicsSimpleTextItem *titleText;
        QGraphicsRectItem *border;
        QGraphicsPixmapItem *backgroundImg;
        QGraphicsRectItem *selectBorder;

        QMap<ConnectionInfo, PinView*>listPins;

        QGraphicsGridLayout *layout;

        QAbstractItemModel *model;
        QPersistentModelIndex objIndex;

        QAction *actDel;
        QAction *actEditor;
        QAction *actLearn;

    public slots:
        void ShowEditor(bool show);
        void ToggleLearningMode(bool learn);

    friend class PinView;
    };
}

#endif // OBJECTVIEW_H
