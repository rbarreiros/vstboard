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

#include "pinview.h"
#include "connectableobjectview.h"
#include "cableview.h"
#include "../connectables/pin.h"
#include "../connectables/objectfactory.h"

using namespace View;

QBrush PinView::highlightBrush(QColor(100,0,200,100),Qt::SolidPattern);

QGraphicsLineItem *PinView::currentLine = 0;

PinView::PinView(float angle, QAbstractItemModel *model,QGraphicsItem * parent, Connectables::Pin *pin) :
        QGraphicsWidget(parent),
        connectInfo(pin->GetConnectionInfo()),
        model(model),
        pinAngle(angle)
{
//    setObjectName(QString("PinView%1").arg((long)pin,0,16));
    setAcceptDrops(true);
    setCursor(Qt::OpenHandCursor);
}

PinView::~PinView()
{
    debug("pin deleted")
}

void PinView::UpdateCablesPosition()
{
    foreach (CableView *cable, connectedCables) {
        cable->UpdatePosition(connectInfo, pinAngle, mapToScene(pinPos()) );
    }
}

void PinView::polishEvent()
{
//    UpdateCablesPosition();
}

QVariant PinView::itemChange ( GraphicsItemChange change, const QVariant & value )
{
    if(!scene())
        return QGraphicsWidget::itemChange(change, value);

    if(change == QGraphicsItem::ItemScenePositionHasChanged)
        UpdateCablesPosition();

    return QGraphicsWidget::itemChange(change, value);
}

void PinView::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }

    setCursor(Qt::ClosedHandCursor);
}

void PinView::mouseMoveEvent ( QGraphicsSceneMouseEvent  * event )
{

    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton)).length() < QApplication::startDragDistance()) {
        return;
    }

    QDrag *drag = new QDrag(event->widget());

    QMimeData *mime = new QMimeData;

    QByteArray bytes;
    CreateMimeData(bytes);
    mime->setData("application/x-pin",bytes);
    drag->setMimeData(mime);

    if(!currentLine) {
        currentLine = new QGraphicsLineItem(this);
        currentLine->setLine(QLineF(pinPos(), event->pos()));
        currentLine->setVisible(false);
    }

    drag->exec();

    if(currentLine) {
        delete currentLine;
        currentLine = 0;
    }
    setCursor(Qt::OpenHandCursor);
}

void PinView::mouseReleaseEvent ( QGraphicsSceneMouseEvent  * /*event*/ )
{
    setCursor(Qt::OpenHandCursor);

}

void PinView::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * /*event*/ )
{
    emit RemoveCablesFromPin(connectInfo);
}

void PinView::dragMoveEvent ( QGraphicsSceneDragDropEvent * event )
{
    if(event->mimeData()->hasFormat("application/x-pin")) {
        QByteArray bytes = event->mimeData()->data("application/x-pin");
        ConnectionInfo otherConnInfo;
        ReadMimeData(bytes,otherConnInfo);

        if(!connectInfo.CanConnectTo(otherConnInfo)) {
            event->ignore();
            return;
        }

        event->acceptProposedAction();

        if(currentLine) {
            QLineF newLine(currentLine->line().p1(), currentLine->mapFromScene(mapToScene(pinPos())));
            currentLine->setLine(newLine);
            currentLine->setVisible(true);
        }
        rectBgnd->setBrush(highlightBrush);
    } else {
        event->ignore();
    }
}

void PinView::dragLeaveEvent( QGraphicsSceneDragDropEvent  * /*event*/ )
{
    rectBgnd->setBrush(Qt::NoBrush);
    if(currentLine)
        currentLine->setVisible(false);
}

void PinView::dropEvent ( QGraphicsSceneDragDropEvent  * event )
{
    rectBgnd->setBrush(Qt::NoBrush);
    QByteArray bytes = event->mimeData()->data("application/x-pin");
    ConnectionInfo connInfo;
    ReadMimeData(bytes,connInfo);
    emit ConnectPins(connectInfo, connInfo);
}

const QPointF PinView::pinPos() const
{
    qreal x = 0;
    if(connectInfo.direction==PinDirection::Output)
        x = geometry().width();
    return QPointF(x,geometry().height()/2);
}

void PinView::CreateMimeData(QByteArray &bytes)
{
    QDataStream stream(&bytes,QIODevice::WriteOnly);
    stream << connectInfo.container;
    stream << connectInfo.objId;
    stream << connectInfo.type;
    stream << connectInfo.direction;
    stream << connectInfo.pinNumber;
    stream << connectInfo.bridge;
}

void PinView::ReadMimeData(QByteArray &bytes, ConnectionInfo &data)
{
    QDataStream stream(&bytes,QIODevice::ReadOnly);
    stream >> data.container;
    stream >> data.objId;
    stream >> data.type;
    stream >> data.direction;
    stream >> data.pinNumber;
    stream >> data.bridge;
}


void PinView::AddCable(CableView *cable)
{
    cable->UpdatePosition(connectInfo, pinAngle, mapToScene(pinPos()) );
    connectedCables.append(cable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
}

void PinView::RemoveCable(CableView *cable)
{
    connectedCables.removeAll(cable);
    if(connectedCables.isEmpty())
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges, false);
}
