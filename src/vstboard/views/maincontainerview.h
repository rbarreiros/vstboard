/******************************************************************************
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
******************************************************************************/

#ifndef MAINCONTAINERVIEW_H
#define MAINCONTAINERVIEW_H

#include "containerview.h"

class MainHost;
namespace View {

    class MainContainerView : public ContainerView
    {
    Q_OBJECT
    public:
        explicit MainContainerView(MainHost *myHost,QAbstractItemModel *model);
        void SetModelIndex(QPersistentModelIndex index);
        void SetParking(QWidget *parking);
        BridgeView *bridgeSend;
        BridgeView *bridgeReturn;

    public slots:
        void OnViewChanged(QRectF rect);

    };
}

#endif // MAINCONTAINERVIEW_H
