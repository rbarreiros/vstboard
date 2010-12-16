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

CursorView::CursorView(QAbstractItemModel *model,bool upsideDown,QGraphicsItem *parent) :
        QGraphicsWidget(parent),
        upsideDown(upsideDown),
        drag(false),
        value(.0f),
        model(model)
{
    QPolygonF pol;
    if(upsideDown) {
        pol << QPointF(5,0) << QPointF(-5,0) << QPointF(0,-7);
    } else {
        pol << QPointF(-5,0) << QPointF(5,0) << QPointF(0,7);
    }
    cursor = new QGraphicsPolygonItem(pol,this);

    setFlag(QGraphicsItem::ItemIsMovable, true);
//    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCursor(Qt::SplitHCursor);
//    setAutoFillBackground(true);
    resize(cursor->boundingRect().size());
}

QRectF CursorView::boundingRect() const
{
    if(upsideDown) {
        return QRectF(-5,-7,10,7);
    } else {
        return QRectF(-5,0,10,7);
    }
//    return QGraphicsWidget::boundingRect();
}

void CursorView::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    drag=true;
}

void CursorView::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    drag=false;
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

    setPos(value*parentWidget()->rect().width(),pos().y());
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
        SetValue(newPos.x());
        if(newPos.x()<.0f)
            newPos.setX(.0f);
        if(newPos.x()>parentWidget()->rect().width())
            newPos.setX(parentWidget()->rect().width());
        newPos.setY(pos().y());
        ValueChanged(newPos.x()/parentWidget()->rect().width());
        return newPos;
    }
    return QGraphicsWidget::itemChange(change, value);
}
