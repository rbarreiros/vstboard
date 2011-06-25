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

#ifndef CONTAINERCONTENT_H
#define CONTAINERCONTENT_H

#include "precomp.h"
#include "../mainwindow.h"

class MainHost;
namespace View {

    class ContainerContent : public QGraphicsWidget
    {
    Q_OBJECT
    public:
        explicit ContainerContent(MainHost *myHost, QAbstractItemModel *model, QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
        void SetModelIndex(QPersistentModelIndex index);
        QPointF GetDropPos();
        QWidget *myParking;

        ViewConfig *config;
    protected:
        void dragEnterEvent( QGraphicsSceneDragDropEvent *event);
        void dragMoveEvent( QGraphicsSceneDragDropEvent *event);
        void dragLeaveEvent( QGraphicsSceneDragDropEvent *event);
        void dropEvent( QGraphicsSceneDragDropEvent *event);

        QAbstractItemModel *model;
        QPersistentModelIndex objIndex;
        QPointF dropPos;

        /// pointer to the MainHost
        MainHost *myHost;

    signals:

    public slots:
        void UpdateColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
        void HighlightStart();
        void HighlightStop();
    };
}
#endif // CONTAINERCONTENT_H
