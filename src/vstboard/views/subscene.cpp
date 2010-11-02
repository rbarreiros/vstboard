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

#include "subscene.h"


using namespace View;
SubScene::SubScene(QObject *parent) :
        QGraphicsScene(parent)
{

}

//SubScene::SubScene(QAbstractItemModel *model, QObject *parent) :
//    QGraphicsScene(parent),
//    model(model)
//{
////        layout = new QGraphicsGridLayout() ;
////        layout->setSpacing(0);
////        layout->setContentsMargins(0,0,0,0);
////        setLayout(layout);

//        rootObj = new QGraphicsRectItem(0, this);

//        bridgeIn = new BridgeView(model, rootObj);
//        bridgeIn->listBridgeIn->layout->setOrientation(Qt::Horizontal);
//        bridgeIn->listBridgeOut->layout->setOrientation(Qt::Horizontal);

//        bridgeOut = new BridgeView(model, rootObj);
//        bridgeOut->listBridgeIn->layout->setOrientation(Qt::Horizontal);
//        bridgeOut->listBridgeOut->layout->setOrientation(Qt::Horizontal);

//        bridgeSend = new BridgeView(model, rootObj);
//        bridgeSend->listBridgeIn->layout->setOrientation(Qt::Horizontal);
//        bridgeSend->listBridgeOut->layout->setOrientation(Qt::Horizontal);

//        bridgeReturn = new BridgeView(model, rootObj);
//        bridgeReturn->listBridgeIn->layout->setOrientation(Qt::Horizontal);
//        bridgeReturn->listBridgeOut->layout->setOrientation(Qt::Horizontal);

////        centerLayout = new ListPinsView(this);
////        content = new ContainerContent(centerLayout);
////        centerLayout->layout->addItem(content);

////        layout->setRowFixedHeight(0,10);
////        layout->setRowFixedHeight(2,10);

////        layout->addItem(bridgeIn,0,0,Qt::AlignLeft);
////        layout->addItem(bridgeOut,0,2,Qt::AlignRight);
////        layout->addItem(centerLayout,1,1,Qt::AlignHCenter);
////        layout->addItem(bridgeSend,2,0,Qt::AlignLeft);
////        layout->addItem(bridgeReturn,2,2,Qt::AlignRight);

//    //    titleText = new QGraphicsSimpleTextItem(QString("Title"),this);
//    //    titleText->moveBy(15,1);

////        setAcceptDrops(true);

//    //    setGeometry(QRectF(0,0,100,15));

////        setFlag(QGraphicsItem::ItemIsMovable, false);
////        setFlag(QGraphicsItem::ItemIsSelectable, false);
////        setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);

////        setWindowFlags(0);
//}

//void SubScene::setModel ( QAbstractItemModel * model )
//{
//    this->model=model;
//}

//void SubScene::SetModelIndex(QPersistentModelIndex index)
//{
//    setObjectName(QString("SubScene%1").arg(index.data(UserRoles::value).toInt()));

//    objIndex = index;

//}

//void SubScene::dragEnterEvent( QGraphicsSceneDragDropEvent *event)
//{
//    QGraphicsScene::dragEnterEvent(event);

//#ifdef VSTSDK
//    //accept DLL files
//    if (event->mimeData()->hasUrls()) {
//        QString fName;
//        QFileInfo info;

//        foreach(QUrl url,event->mimeData()->urls()) {
//            fName = url.toLocalFile();
//            info.setFile( fName );
//            if ( info.isFile() && info.isReadable() && info.suffix()=="dll" ) {
//                event->accept();
////                GraphicScene::Get()->SetDropReceiver(this);
//            }
//        }
//        return;
//    }
//#endif

//    //accept Audio interface
//    //accept Midi interface
//    //accept Tools
//    if(event->mimeData()->hasFormat("application/x-audiointerface") ||
//       event->mimeData()->hasFormat("application/x-midiinterface") ||
//       event->mimeData()->hasFormat("application/x-tools")) {
//        event->accept();
//        return;
//    }

//    event->ignore();
//}

//void SubScene::dragMoveEvent ( QGraphicsSceneDragDropEvent * event )
//{
//    //QGraphicsScene::dragMoveEvent(event);
//    event->accept();
//}

//void SubScene::dropEvent( QGraphicsSceneDragDropEvent *event)
//{
//    QGraphicsScene::dropEvent(event);
//    event->setAccepted(model->dropMimeData(event->mimeData(), event->proposedAction(), 0, 0, objIndex));
//}

//void SubScene::OnViewChanged(QRectF rect)
//{
//    bridgeIn->setPos(rect.topLeft());
//    bridgeOut->setPos(rect.topRight().x() - bridgeOut->geometry().width(), rect.topRight().y());
//    bridgeSend->setPos(rect.bottomLeft().x(), rect.bottomLeft().y() - bridgeSend->geometry().height() );
//    bridgeReturn->setPos(rect.bottomRight().x() - bridgeReturn->geometry().width(), rect.bottomRight().y() - bridgeReturn->geometry().height());
////    setGeometry(rect);
//}
