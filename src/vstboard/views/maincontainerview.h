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

#ifndef MAINCONTAINERVIEW_H
#define MAINCONTAINERVIEW_H

#include "objectview.h"
#include "containercontent.h"
#include "cableview.h"
#include "bridgeview.h"

class MainHost;
namespace View {

    class MainContainerView : public ObjectView
    {
    Q_OBJECT
    public:
        explicit MainContainerView(MainHost *myHost,QAbstractItemModel *model);
        ~MainContainerView();
        void SetModelIndex(QPersistentModelIndex index);
        void SetParking(QWidget *parking);
        QPointF GetDropPos();

        BridgeView *bridgeIn;
        BridgeView *bridgeOut;
        BridgeView *bridgeSend;
        BridgeView *bridgeReturn;

    protected:
        ContainerContent *content;
        QList<CableView*>listCables;

    public slots:
        void OnViewChanged(QRectF rect);

    };
}

#endif // MAINCONTAINERVIEW_H
