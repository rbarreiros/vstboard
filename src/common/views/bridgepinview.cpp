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

#include "bridgepinview.h"

using namespace View;

BridgePinView::BridgePinView(float angle, QAbstractItemModel *model,QGraphicsItem * parent, const ConnectionInfo &pinInfo) :
        PinView(angle, model,parent, pinInfo),
        value(.0f),
        valueType(PinType::ND)
{
    setGeometry(0,0,PINSIZE,PINSIZE);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMaximumSize(PINSIZE,PINSIZE);
    setMinimumSize(PINSIZE,PINSIZE);


    QPolygonF pol;

    if( (connectInfo.direction==PinDirection::Input && pinAngle<0)
        || (connectInfo.direction==PinDirection::Output && pinAngle>0) ) {
        pol << QPointF(PINSIZE/2,PINSIZE) << QPointF(0,0) << QPointF(PINSIZE,0);
    } else {
        pol << QPointF(PINSIZE/2,0) << QPointF(0,PINSIZE) << QPointF(PINSIZE,PINSIZE);
    }

    vuValue = new QGraphicsPolygonItem(pol,this);
//    vuValue = new QGraphicsEllipseItem(geometry().adjusted(BRPIN_MRG,BRPIN_MRG,-BRPIN_MRG,-BRPIN_MRG),this);
    vuValue->setPen(Qt::NoPen);

    rectBgnd = new QGraphicsPolygonItem(pol,this);
    rectBgnd->setBrush(Qt::NoBrush);
}

const QPointF BridgePinView::pinPos() const
{
    if(pinAngle>0) {
        return QPointF(PINSIZE/2,PINSIZE);
    } else {
        return QPointF(PINSIZE/2,0);
    }
}

void BridgePinView::UpdateModelIndex(const QModelIndex &index)
{
    float newVal=index.data(UserRoles::value).toFloat();
    value = std::max(value,newVal);
    valueType = (PinType::Enum)index.data(UserRoles::type).toInt();
}

void BridgePinView::updateVu()
{
    if(value<.0f)
        return;

    value-=.3f;
    if(value<.0f) {
        value=-1.0f;
        vuValue->setBrush(Qt::NoBrush);
        return;
    }

    QColor c;
    switch(valueType) {
        case PinType::Audio:
            c=Qt::yellow;
            break;
    case PinType::Midi :
            c=Qt::cyan;
            break;
    case PinType::Parameter :
            c=Qt::red;
            break;
        default :
            c=Qt::darkGray;
            break;
    }
//    if(value<0.5)
//        c.setAlphaF( value/0.5 );
    vuValue->setBrush(c);
}
