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

#include "bridgepinview.h"

using namespace View;
#define PINSIZE 12

BridgePinView::BridgePinView(QAbstractItemModel *model,QGraphicsItem * parent, Connectables::Pin *pin) :
        PinView(model,parent, pin)
{
    setGeometry(0,0,PINSIZE,PINSIZE);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMaximumSize(PINSIZE,PINSIZE);
    setMinimumSize(PINSIZE,PINSIZE);

    rectBgnd = new QGraphicsEllipseItem(geometry(),this);
    rectBgnd->setBrush(Qt::NoBrush);
}

const QPointF BridgePinView::pinPos() const
{
    return QPointF(PINSIZE/2,PINSIZE/2);
}
