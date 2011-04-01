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

#include "connectablepinview.h"
#include "../connectables/objectfactory.h"

using namespace View;

ConnectablePinView::ConnectablePinView(float angle, QAbstractItemModel *model, QGraphicsItem * parent, const ConnectionInfo &pinInfo) :
       PinView(angle,model,parent,pinInfo),
       value(0),
       isParameter(false)
{
    setGeometry(0,0,50,15);
    setMinimumSize(50,15);
    setMaximumSize(50,15);

    outline = new QGraphicsRectItem(geometry(), this);

    rectVu = new QGraphicsRectItem(this);
    outline->setBrush(Qt::NoBrush);

//    textItem = new OutlinedText("", this, Qt::black, QColor(255,255,255,150));
    textItem = new QGraphicsSimpleTextItem(this);
    textItem->moveBy(2,1);
    textItem->setZValue(1);

//    QColor c;
    switch(connectInfo.type) {
    case PinType::Audio :
        colorGroupId=ColorGroups::AudioPin;
        break;
    case PinType::Midi :
        colorGroupId=ColorGroups::MidiPin;
        rectVu->setRect(rect());
        break;
    case PinType::Parameter :
        colorGroupId=ColorGroups::ParameterPin;
        break;
    default :
        colorGroupId=ColorGroups::ND;
        break;
    }

    outline->setBrush( config->GetColor(colorGroupId, Colors::Background) );
    rectVu->setBrush( config->GetColor(colorGroupId, Colors::VuMeter) );
}

void ConnectablePinView::UpdateColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color)
{
    if(groupId!=colorGroupId)
        return;

    switch(colorId) {
        case Colors::Background :
            outline->setBrush(color);
            break;
        case Colors::VuMeter :
            rectVu->setBrush(color);
            break;
    }

}

void ConnectablePinView::UpdateModelIndex(const QModelIndex &index)
{
    QString newName = index.data(Qt::DisplayRole).toString();
    if(newName!=textItem->text())
        textItem->setText(newName);

    if(connectInfo.type == PinType::Parameter) {
        value = index.data(UserRoles::value).toFloat();
        float newVu = geometry().width() * value;
        rectVu->setRect(0,0, newVu, geometry().height());
    } else {
        float newVal = index.data(UserRoles::value).toFloat();
        value = std::max(value,newVal);
    }

    ConnectionInfo pinInfo = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();
    if(pinInfo.type == PinType::Parameter)
        isParameter=true;

}

void ConnectablePinView::updateVu()
{
    if(value<.0f)
        return;

    switch(connectInfo.type) {
        case PinType::Audio : {
            value-=.05f;
            float newVu=.0f;
            if(value<.0f) {
                value=-1.0f;
            } else {
                newVu = geometry().width() * value;
            }
            rectVu->setRect(0,0, newVu, geometry().height());
            break;
        }

        case PinType::Midi : {
            value-=.1f;
            if(value<.0f) {
                value=-1.0f;
                rectVu->setBrush(Qt::NoBrush);
                return;
            }
            QColor c=Qt::cyan;

            if(value<0.7)
                c.setAlphaF( value/0.7 );

            rectVu->setBrush(c);
            break;
        }

        default :
            break;
    }
}

void ConnectablePinView::wheelEvent ( QGraphicsSceneWheelEvent * event )
{
    if(!isParameter)
        return;

    event->accept();

    int increm=1;
    if(event->delta()<0)
        increm=-1;

    float val = pinIndex.data(UserRoles::value).toFloat() + pinIndex.data(UserRoles::stepSize).toFloat()*increm;
    if(val>1.0f) val=1.0f;
    if(val<.0f) val=.0f;

    model->setData(pinIndex,val,UserRoles::value);
}
