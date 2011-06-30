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
#include "heap.h"


#include "connectablepinview.h"
#include "../connectables/objectfactory.h"

using namespace View;

ConnectablePinView::ConnectablePinView(float angle, QAbstractItemModel *model, QGraphicsItem * parent, const ConnectionInfo &pinInfo, ViewConfig *config) :
       PinView(angle,model,parent,pinInfo,config),
       value(0),
       isParameter(false)
{
    setGeometry(0,0,50,15);
    setMinimumSize(50,15);
    setMaximumSize(50,15);

    outline = new QGraphicsRectItem(geometry(), this);
    highlight = new QGraphicsRectItem(geometry(), this);
    highlight->setVisible(false);

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
    vuColor = config->GetColor(colorGroupId, Colors::VuMeter);
    rectVu->setBrush( vuColor );
    highlight->setBrush( config->GetColor(ColorGroups::Object, Colors::HighlightBackground) );
    textItem->setBrush(  config->GetColor(ColorGroups::Object, Colors::Text) );
}

void ConnectablePinView::UpdateColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color)
{
    if(groupId==colorGroupId && colorId==Colors::Background) {
        outline->setBrush(color);
        return;
    }
    if(groupId==colorGroupId && colorId==Colors::VuMeter) {
        vuColor=color;
        if(connectInfo.type != PinType::Midi) {
            rectVu->setBrush(color);
        }
        return;
    }
    if(groupId==ColorGroups::Object && colorId==Colors::HighlightBackground) {
        highlight->setBrush(color);
        return;
    }

    if(groupId==ColorGroups::Object && colorId==Colors::Text) {
        textItem->setBrush(color);
        return;
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
            QColor c = vuColor;

            if(value<0.7)
                c.setAlphaF( value/0.7 );

            rectVu->setBrush(c);
            break;
        }

        default :
            break;
    }
}

void ConnectablePinView::ValueChanged(float newVal)
{
    if(value==newVal)
        return;
    if(newVal>1.0f) newVal=1.0f;
    if(newVal<0.0f) newVal=0.0f;

    model->setData(pinIndex,newVal,UserRoles::value);
}

void ConnectablePinView::wheelEvent ( QGraphicsSceneWheelEvent * event )
{
    if(!isParameter)
        return;

    event->accept();

    int increm=1;
    if(event->delta()<0)
        increm=-1;

    ValueChanged( pinIndex.data(UserRoles::value).toFloat()
                  + pinIndex.data(UserRoles::stepSize).toFloat()*increm);
}

void ConnectablePinView::keyPressEvent ( QKeyEvent * event )
{
    int k = event->key();

    if(connectInfo.type == PinType::Parameter) {
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
    }

    QGraphicsWidget::keyPressEvent(event);
}
