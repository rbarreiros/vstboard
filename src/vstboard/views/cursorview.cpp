/**************************************************************************
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
**************************************************************************/

#include "cursorview.h"
#include "globals.h"

using namespace View;

CursorView::CursorView(QAbstractItemModel *model,bool isMaxi,bool upsideDown,QGraphicsItem *parent) :
        QGraphicsWidget(parent),
        isMaxi(isMaxi),
        upsideDown(upsideDown),
        drag(false),
        value(.0f),
        model(model),
        offset(QPointF(0,0))
{
    QPolygonF pol;

    if(upsideDown) {
        if(isMaxi) {
            pol << QPointF(5,7) << QPointF(0,7) << QPointF(5,0);
            offset.setX(5);
            offset.setY(7);
        } else {
            pol << QPointF(5,7) << QPointF(0,7) << QPointF(0,0);
            offset.setY(7);
        }
    } else {
        if(isMaxi) {
            pol << QPointF(0,0) << QPointF(5,0) << QPointF(5,7);
            offset.setX(5);
        } else {
            pol << QPointF(0,0) << QPointF(5,0) << QPointF(0,7);
        }
    }
    cursor = new QGraphicsPolygonItem(pol,this);
    cursor->setBrush(Qt::black);

    setFlag(QGraphicsItem::ItemIsMovable, true);
//    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCursor(Qt::SplitHCursor);
//    setAutoFillBackground(true);
    resize(cursor->boundingRect().size());
}

void CursorView::setPos ( const QPointF & pos )
{
    QPointF p( pos);

    if(upsideDown) {
        p.setY(parentWidget()->rect().height());
    } else {
        p.setY(.0f);
    }

    p-=offset;
    QGraphicsWidget::setPos(p);
}

void CursorView::setPos ( qreal x, qreal y )
{
    if(upsideDown) {
        y=parentWidget()->rect().height();
    } else {
        y=.0f;
    }

    x-=offset.x();
    y-=offset.y();
    QGraphicsWidget::setPos(x,y);
}

QRectF CursorView::boundingRect() const
{
//    if(upsideDown) {
//        return QRectF(-5,-7,10,7);
//    } else {
//        return QRectF(-5,0,10,7);
//    }
    return QGraphicsWidget::boundingRect();
}

void CursorView::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    drag=true;
    QGraphicsWidget::mousePressEvent(event);
}

void CursorView::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    drag=false;
    QGraphicsWidget::mouseReleaseEvent(event);
}

void CursorView::SetValue(float newVal)
{
    if(newVal>1.0f)
        newVal=1.0f;
    if(newVal<.0f)
        newVal=.0f;

    if(value==newVal)
        return;
    value=newVal;

    setPos(value*parentWidget()->rect().width(),0);
}

void CursorView::SetModelIndex(QPersistentModelIndex index)
{
    modelIndex=index;
    SetValue(modelIndex.data(UserRoles::value).toFloat());
}

void CursorView::ValueChanged(float newVal)
{
    if(value==newVal)
        return;
    model->setData(modelIndex,newVal,UserRoles::value);
//    value=newVal;
}

QVariant CursorView::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene() && drag) {
        QPointF newPos = value.toPointF();
        //SetValue(newPos.x());
        if(newPos.x()<-offset.x())
            newPos.setX(-offset.x());
        if(newPos.x()>parentWidget()->rect().width()-offset.x())
            newPos.setX(parentWidget()->rect().width()-offset.x());
        if(upsideDown) {
            newPos.setY(parentWidget()->rect().height()-offset.y());
        } else {
            newPos.setY(-offset.y());
        }
        if(parentWidget()->rect().width()!=0)
            ValueChanged((newPos.x()+offset.x())/parentWidget()->rect().width());
        return newPos;
    }
    return QGraphicsWidget::itemChange(change, value);
}
