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

class MainHost;
namespace View {

    class ConnectableView;
    class PinView;
    class ObjectView : public QGraphicsWidget
    {
    Q_OBJECT
    public:
        explicit ObjectView(MainHost *myHost, QAbstractItemModel *model, QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
        virtual ~ObjectView();

        virtual void SetModelIndex(QPersistentModelIndex index);
        virtual void UpdateModelIndex();
        void Shrink();

        ListPinsView *listAudioIn;
        ListPinsView *listAudioOut;
        ListPinsView *listMidiIn;
        ListPinsView *listMidiOut;
        ListPinsView *listParametersIn;
        ListPinsView *listParametersOut;
        ListPinsView *listBridge;

    protected:
        void SetErrorMessage(const QString & msg);
//        void SetBackground(const QString & imgName);
        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
        void resizeEvent ( QGraphicsSceneResizeEvent * event );
        virtual void closeEvent ( QCloseEvent * event );
        QVariant itemChange ( GraphicsItemChange  change, const QVariant & value );
        QPen pen;

        void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

        QGraphicsSimpleTextItem *titleText;
        QGraphicsRectItem *border;
//        QGraphicsPixmapItem *backgroundImg;
        QGraphicsRectItem *selectBorder;
        QGraphicsPixmapItem *errorMessage;

        QMap<ConnectionInfo, PinView*>listPins;

        QGraphicsGridLayout *layout;

        QAbstractItemModel *model;
        QPersistentModelIndex objIndex;

        QAction *actDel;

        bool shrinkAsked;
        MainHost *myHost;

    public slots:
        void ShrinkNow();
//        void ShowEditor(bool show);
//        void ToggleLearningMode(bool learn);

    friend class PinView;
    };
}

#endif // OBJECTVIEW_H
