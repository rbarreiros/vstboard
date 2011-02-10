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

#include "cableview.h"

using namespace View;

CableView::CableView(ConnectionInfo pinOut, ConnectionInfo pinIn, QGraphicsItem *parent)
    : QGraphicsPathItem(parent),
    QObject(),
    pinOut(pinOut),
    pinIn(pinIn)
{
//    setObjectName("cable");
}

void CableView::UpdatePosition(const ConnectionInfo &pinInfo, const float angle, const QPointF &pt)
{
    //move one end of the cable
    if(pinInfo == pinOut)
        PtOut=mapFromScene(pt);
    if(pinInfo == pinIn)
        PtIn=mapFromScene(pt);

    if(pinInfo == pinOut) {
        CtrlPtOut = PtOut;
        CtrlPtOut.rx()+=50*cos(angle);
        CtrlPtOut.ry()+=50*sin(angle);
    }
    if(pinInfo == pinIn) {
        CtrlPtIn = PtIn;
        CtrlPtIn.rx()+=50*cos(angle);
        CtrlPtIn.ry()+=50*sin(angle);
    }

    QPainterPath newPath;
    newPath.moveTo(PtOut);
    newPath.cubicTo(CtrlPtOut,CtrlPtIn,PtIn);

    setPath(newPath);
}

