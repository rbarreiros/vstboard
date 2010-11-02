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

#include "outlinedtext.h"

OutlinedText::OutlinedText(const QString &text, QGraphicsItem *parent, const QBrush &textBrush, const QBrush &outlineBrush) :
        QGraphicsSimpleTextItem(text, parent),
        textBrush(textBrush),
        outlineBrush(outlineBrush),
        pix(0)
{
    setText(text);
}

OutlinedText::~OutlinedText()
{
    delete pix;
}

void OutlinedText::CreateCache(const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    cachedText = text();
    //create a cached pixmap
    delete pix;

    QRectF rect = QGraphicsSimpleTextItem::boundingRect();
    rect.setRight(rect.right()*scaleX);
    rect.setBottom(rect.bottom()*scaleY);

    pix = new QPixmap( rect.width()+2, rect.height()+2 );
    if(pix->isNull())
        return;

    pix->fill(Qt::transparent);

    //create a pixmap of the white text
    QPixmap tmpPix( rect.width(), rect.height() );

    //pix too small
    if(tmpPix.isNull())
        return;

    tmpPix.fill(Qt::transparent);
    QPainter p( &tmpPix );
    QMatrix mat;
    mat.scale(scaleX,scaleY);
    p.setMatrix(mat);

    setBrush(outlineBrush);
    QGraphicsSimpleTextItem::paint(&p,option,widget);

    //draw the white pixmap on the cache
    QPainter p2( pix );

    p2.drawPixmap(0,0,tmpPix);
    p2.drawPixmap(1,0,tmpPix);
    p2.drawPixmap(2,0,tmpPix);

    p2.drawPixmap(0,1,tmpPix);
    p2.drawPixmap(2,1,tmpPix);

    p2.drawPixmap(0,2,tmpPix);
    p2.drawPixmap(1,2,tmpPix);
    p2.drawPixmap(2,2,tmpPix);

    //draw the black text on the cache
    setBrush(textBrush);
    p2.translate(1,1);
    p2.setMatrix(mat * p2.matrix());

    QGraphicsSimpleTextItem::paint(&p2,option,widget);
}

QRectF OutlinedText::boundingRect() const
{
    //our bounding is the text bounding with a 2px margin
    QRectF rect = QGraphicsSimpleTextItem::boundingRect();
    rect.setWidth(rect.width()+2);
    rect.setHeight(rect.height()+2);
    return rect;
}

void OutlinedText::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    //redraw if the scale changed enough or if we're close to the original size
//    if(!pix //we have no cache yet
//       || cachedText != text() //text changed
//       || ( fabs( static_cast<float>( painter->matrix().m22() ) - 1.0f ) < 0.2f
//           && fabs( scaleX - static_cast<float>( painter->matrix().m11() )) >0.001f
//           ) //small change but close to 1.0
//       || fabs( (scaleX - static_cast<float>(painter->matrix().m11()) )/scaleX ) > 0.4f //big change, far from 1.0
//     ) {

    if(text().length()==0)
        return;

    if(!pix                         //no cache yet
        || cachedText != text()     //text changed
        || fabs( scaleX - static_cast<float>( painter->matrix().m11() )) >0.001f //size changed
        || fabs( scaleY - static_cast<float>( painter->matrix().m22() )) >0.001f
     ) {
        scaleX = painter->matrix().m11();
        scaleY = painter->matrix().m22();
        CreateCache(option,widget);
    }

    //else draw current cache, transformed
    QTransform trans;
    trans.scale(scaleX, scaleY);
    trans = trans.inverted()*painter->worldTransform();
    painter->setWorldTransform(trans);

    painter->drawPixmap(0,0,*pix);
}
