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

#include "maincontainerview.h"
#include "../connectables/objectfactory.h"

using namespace View;

MainContainerView::MainContainerView(QAbstractItemModel *model) :
        ContainerView(model)
{
//    layout = new QGraphicsGridLayout() ;
//    layout->setSpacing(0);
//    layout->setContentsMargins(0,0,0,0);
//    setLayout(layout);

//    centerLayout = new ListPinsView(this);
    content = new ContainerContent(model,this);
   // content->setFlag(QGraphicsItem::ItemIsMovable, false);
    content->setAcceptDrops(true);

//    centerLayout->layout->addItem(content);

    bridgeIn = new BridgeView(model, this);
    bridgeIn->listBridgeIn->layout->setOrientation(Qt::Horizontal);
    bridgeIn->listBridgeOut->layout->setOrientation(Qt::Horizontal);

    bridgeOut = new BridgeView(model, this);
    bridgeOut->listBridgeIn->layout->setOrientation(Qt::Horizontal);
    bridgeOut->listBridgeOut->layout->setOrientation(Qt::Horizontal);

    bridgeSend = new BridgeView(model, this);
    bridgeSend->listBridgeIn->layout->setOrientation(Qt::Horizontal);
    bridgeSend->listBridgeOut->layout->setOrientation(Qt::Horizontal);

    bridgeReturn = new BridgeView(model, this);
    bridgeReturn->listBridgeIn->layout->setOrientation(Qt::Horizontal);
    bridgeReturn->listBridgeOut->layout->setOrientation(Qt::Horizontal);



//    layout->setRowFixedHeight(0,10);
//    layout->setRowFixedHeight(2,10);

//    layout->addItem(bridgeIn,0,0,Qt::AlignLeft);
//    layout->addItem(bridgeOut,0,2,Qt::AlignRight);
//    layout->addItem(centerLayout,0,0,Qt::AlignHCenter);
//    layout->addItem(bridgeSend,2,0,Qt::AlignLeft);
//    layout->addItem(bridgeReturn,2,2,Qt::AlignRight);

//    titleText = new QGraphicsSimpleTextItem(QString("Title"),this);
//    titleText->moveBy(15,1);

//    setAcceptDrops(true);

//    setGeometry(QRectF(0,0,100,15));

//    setFlag(QGraphicsItem::ItemIsMovable, false);
//    setFlag(QGraphicsItem::ItemIsSelectable, false);
//    setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);

//    setWindowFlags(0);
    setGeometry(0,0,0,0);
}

void MainContainerView::SetModelIndex(QPersistentModelIndex index)
{
    setObjectName(QString("MainContainerView%1").arg(index.data(UserRoles::value).toInt()));
    content->SetModelIndex(index);
}



void MainContainerView::OnViewChanged(QRectF rect)
{
    bridgeIn->setPos(rect.topLeft());
    bridgeOut->setPos(rect.right() - bridgeOut->geometry().width(), rect.top());
    bridgeSend->setPos(rect.left(), rect.bottom() - bridgeSend->geometry().height() );
    bridgeReturn->setPos(rect.right() - bridgeReturn->geometry().width(), rect.bottom() - bridgeReturn->geometry().height());
    content->setGeometry(rect);
    //setGeometry(rect);
    //scene()->setSceneRect(childrenBoundingRect());

//    setGeometry(rect);
}
