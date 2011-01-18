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

#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include "../precomp.h"

class TextButton : public QGraphicsSimpleTextItem
{
public:
    TextButton(const QString &text, QGraphicsItem *parent, const QBrush &textBrush, const QBrush &outlineBrush);
    ~TextButton();
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    void Toggle(bool value);

protected:
    void CreateCache(const QString &text, const QStyleOptionGraphicsItem * option, QWidget * widget);

    const QBrush textBrush;
    const QBrush outlineBrush;
    QPixmap *pix;
    float scaleX;
    float scaleY;
    QRectF bounding;
    bool toggleState;
};

#endif // TEXTBUTTON_H
