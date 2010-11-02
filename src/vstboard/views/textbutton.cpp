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

#include "textbutton.h"

TextButton::TextButton(const QString &text, QGraphicsItem *parent, const QBrush &textBrush, const QBrush &outlineBrush) :
        QGraphicsSimpleTextItem(text, parent),
        textBrush(textBrush),
        outlineBrush(outlineBrush),
        pix(0),
        toggleState(false)
{
    setText(text);
    setCursor(Qt::PointingHandCursor);
}

TextButton::~TextButton()
{
    delete pix;
}

void TextButton::Toggle(bool value)
{
    toggleState = value;
    update(boundingRect());
}

void TextButton::CreateCache(const QString &/*text*/, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    //create a cached pixmap
    delete pix;

    QRectF rect = QGraphicsSimpleTextItem::boundingRect();

    qreal centering = .0f;
    if(rect.width()<rect.height()) {
        centering = (rect.height() - rect.width())/2;
        rect.setWidth(rect.height());
    }
    bounding = rect;

    rect.setWidth(rect.width()*scaleX);
    rect.setHeight(rect.height()*scaleY);

    centering*=scaleX;

    pix = new QPixmap( rect.width(), rect.height() );

    //if the image is too small
    if(pix->isNull())
        return;

    pix->fill(Qt::transparent);

    //create a pixmap whith the white text
    QMatrix mat;
    mat.scale(scaleX,scaleY);

    QPainter p2( pix );

    //draw circle
    p2.setPen(Qt::NoPen);

    QRectF rectR = rect;

    p2.setBrush(outlineBrush);
    rectR.setWidth(rect.width()-2);
    rectR.setHeight(rect.height()-2);
    rectR.moveTo(1,1);
    p2.drawEllipse(rectR);

    rectR.setWidth(rect.width()-4);
    rectR.setHeight(rect.height()-4);
    rectR.moveTo(2,2);
    p2.drawEllipse(rectR);

    //draw the black text on the cache
    setBrush(textBrush);
    p2.translate(centering,0);
    p2.setMatrix(mat * p2.matrix());
    QGraphicsSimpleTextItem::paint(&p2,option,widget);
}

QRectF TextButton::boundingRect() const
{
//    //our bounding is the text bounding with a 2px margin
//    QRectF bounding = QGraphicsSimpleTextItem::boundingRect();
//    rect.setWidth(rect.width()+2);
//    rect.setHeight(rect.height()+2);
//    return rect;
    return bounding;
}

void TextButton::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    if(!pix                         //no cache yet
        || fabs( scaleX - static_cast<float>( painter->matrix().m11() )) >0.01f //size changed
        || fabs( scaleY - static_cast<float>( painter->matrix().m22() )) >0.001f
     ) {
        //redraw
        scaleX = painter->matrix().m11();
        scaleY = painter->matrix().m22();
        CreateCache(text(),option,widget);
    }

    //else draw current cache, transformed
    QTransform trans;
    trans.scale(scaleX, scaleY);
    trans = trans.inverted()*painter->worldTransform();
    painter->setWorldTransform(trans);
    painter->drawPixmap(0,0,*pix);
    if(toggleState) {
        painter->drawRect(boundingRect());
    }
}
