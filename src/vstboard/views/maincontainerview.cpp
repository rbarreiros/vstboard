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

#include "maincontainerview.h"
#include "../connectables/objectfactory.h"

using namespace View;

MainContainerView::MainContainerView(MainHost *myHost,QAbstractItemModel *model) :
        ContainerView(myHost,model)
{
//    setObjectName("MainContainerView");

    content = new ContainerContent(model,this);
    content->setAcceptDrops(true);

    bridgeIn = new BridgeView(myHost, model, this);
    bridgeIn->listBridgeIn->layout->setOrientation(Qt::Horizontal);
    bridgeIn->listBridgeOut->layout->setOrientation(Qt::Horizontal);

    bridgeOut = new BridgeView(myHost, model, this);
    bridgeOut->listBridgeIn->layout->setOrientation(Qt::Horizontal);
    bridgeOut->listBridgeOut->layout->setOrientation(Qt::Horizontal);

    bridgeSend = new BridgeView(myHost, model, this);
    bridgeSend->listBridgeIn->layout->setOrientation(Qt::Horizontal);
    bridgeSend->listBridgeOut->layout->setOrientation(Qt::Horizontal);

    bridgeReturn = new BridgeView(myHost, model, this);
    bridgeReturn->listBridgeIn->layout->setOrientation(Qt::Horizontal);
    bridgeReturn->listBridgeOut->layout->setOrientation(Qt::Horizontal);

    setGeometry(0,0,0,0);
}

void MainContainerView::SetModelIndex(QPersistentModelIndex index)
{
    setObjectName(QString("MainContainerView%1").arg(index.data(UserRoles::value).toInt()));
    content->SetModelIndex(index);
}

void MainContainerView::SetParking(QWidget *parking)
{
    content->myParking = parking;
}

void MainContainerView::OnViewChanged(QRectF rect)
{
    bridgeIn->setPos(rect.topLeft());
    bridgeOut->setPos(rect.right() - bridgeOut->geometry().width(), rect.top());
    bridgeSend->setPos(rect.left(), rect.bottom() - bridgeSend->geometry().height() );
    bridgeReturn->setPos(rect.right() - bridgeReturn->geometry().width(), rect.bottom() - bridgeReturn->geometry().height());
    content->setGeometry(rect);
}
