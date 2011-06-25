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

#include "cursorview.h"
#include "globals.h"
#include "viewconfig.h"

using namespace View;

#define CURS_WIDTH 5.0f
#define CURD_HEIGHT 7.5f

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
            pol << QPointF(CURS_WIDTH,CURD_HEIGHT) << QPointF(0,CURD_HEIGHT) << QPointF(CURS_WIDTH,0);
            offset.setX(CURS_WIDTH);
            offset.setY(CURD_HEIGHT);
        } else {
            pol << QPointF(CURS_WIDTH,CURD_HEIGHT) << QPointF(0,CURD_HEIGHT) << QPointF(0,0);
            offset.setY(CURD_HEIGHT);
        }
    } else {
        if(isMaxi) {
            pol << QPointF(0,0) << QPointF(CURS_WIDTH,0) << QPointF(CURS_WIDTH,CURD_HEIGHT);
            offset.setX(CURS_WIDTH);
        } else {
            pol << QPointF(0,0) << QPointF(CURS_WIDTH,0) << QPointF(0,CURD_HEIGHT);
        }
    }
    cursor = new QGraphicsPolygonItem(pol,this);
    cursor->setPen(Qt::NoPen);
    cursor->setBrush(QColor(64,64,64));

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::SplitHCursor);
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
    if(newVal>1.0f) newVal=1.0f;
    if(newVal<0.0f) newVal=0.0f;

    model->setData(modelIndex,newVal,UserRoles::value);
}

QVariant CursorView::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene() && drag) {
        QPointF newPos = value.toPointF();

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

void CursorView::keyPressEvent ( QKeyEvent * event )
{
    int k = event->key();

    if(event->modifiers() & Qt::ControlModifier) {
        if(k==Qt::Key_Left) { ValueChanged(value-0.01); return; }
        if(k==Qt::Key_Right) { ValueChanged(value+0.01); return; }
    } else {
        if(k==Qt::Key_Left) { ValueChanged(value-0.1); return; }
        if(k==Qt::Key_Right) { ValueChanged(value+0.1); return; }
    }

    float val = ViewConfig::KeyboardNumber(k);
    if(val>=0) {
        ValueChanged(val);
        return;
    }

    QGraphicsWidget::keyPressEvent(event);
}
