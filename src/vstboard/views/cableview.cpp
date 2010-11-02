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

#include "cableview.h"

using namespace View;

CableView::CableView(ConnectionInfo pinOut, ConnectionInfo pinIn, QGraphicsItem *parent)
    : QGraphicsPathItem(parent),
    QObject(),
    pinOut(pinOut),
    pinIn(pinIn)
{

}

void CableView::UpdatePosition(const ConnectionInfo &pinInfo, const QPointF &pt)
{
    QPainterPath p(path());

    QPointF pt1(path().pointAtPercent(0));
    QPointF pt2(path().pointAtPercent(1));

    if(pinInfo == pinOut)
        pt1=mapFromScene(pt);
    if(pinInfo == pinIn)
        pt2=mapFromScene(pt);

    QPointF ctrlPt1(pt1);
    QPointF ctrlPt2(pt2);
    ctrlPt1+=QPointF(50,0);
    ctrlPt2+=QPointF(-50,0);

    QPainterPath p2;
    p2.moveTo(pt1);
    p2.cubicTo(ctrlPt1,ctrlPt2,pt2);

    setPath(p2);
}

