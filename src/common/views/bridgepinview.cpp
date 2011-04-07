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
#include "objectview.h"

using namespace View;

/*!
  \class View::BridgePinView
  \brief view class for the bridge pins
  */

/*!
  \param angle angle in rad (0=output, pi=input) used by CableView
  \param model pointer to the model item representing the pin
  \param parent pointer to the parent object view
  \param pinInfo description of the pin
  */
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
    vuValue->setPen(Qt::NoPen);

    outline = new QGraphicsPolygonItem(pol,this);
    outline->setBrush(Qt::NoBrush);
    outline->setPen( config->GetColor(ColorGroups::Bridge,Colors::Borders) );

    highlight = new QGraphicsPolygonItem(pol,this);
    highlight->setVisible(false);
    highlight->setBrush( config->GetColor(ColorGroups::Bridge, Colors::HighlightBackground) );

    QPalette pal(palette());
    pal.setColor(QPalette::Window, config->GetColor(ColorGroups::Bridge,Colors::Background) );
    setPalette( pal );
}

void BridgePinView::UpdateColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color)
{
    if(groupId!=ColorGroups::Bridge)
        return;

    switch(colorId) {
    case Colors::Borders:
        outline->setPen( color );
        break;
    case Colors::Background: {
        QPalette pal(palette());
        pal.setColor(QPalette::Window,color);
        setPalette( pal );
        break;
    }
    case Colors::HighlightBackground :
        highlight->setBrush( color );
    }
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
            c = config->GetColor(ColorGroups::AudioPin, Colors::Background);
            break;
    case PinType::Midi :
            c= config->GetColor(ColorGroups::MidiPin, Colors::Background);
            break;
    case PinType::Parameter :
            c= config->GetColor(ColorGroups::ParameterPin, Colors::Background);
            break;
        default :
            c=Qt::darkGray;
            break;
    }
//    if(value<0.5)
//        c.setAlphaF( value/0.5 );
    vuValue->setBrush(c);
}
