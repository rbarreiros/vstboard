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

#include "connectablepinview.h"
#include "../connectables/objectfactory.h"

using namespace View;

ConnectablePinView::ConnectablePinView(QAbstractItemModel *model, QGraphicsItem * parent, Connectables::Pin *pin) :
       PinView(model,parent,pin),
       value(0)
{
    falloffSpeed = .05f;

    setGeometry(0,0,50,15);
    setMinimumSize(50,15);
    setMaximumSize(50,15);

    rectBgnd = new QGraphicsRectItem(geometry(), this);

    textItem = new OutlinedText(pin->GetDisplayedText(), this, Qt::black, QColor(255,255,255,150));
    textItem->moveBy(2,1);

    rectVu = new QGraphicsRectItem(this);
    rectBgnd->setBrush(Qt::NoBrush);

    QLinearGradient gradient(0, 0, geometry().right(), 0);
    gradient.setColorAt(0, QColor::fromRgbF(0, 0, 0, 0));
    gradient.setColorAt(1, QColor::fromRgbF(0, 1, 0, 1));
    rectVu->setBrush(QBrush(gradient));
}

void ConnectablePinView::UpdateModelIndex(QModelIndex index)
{
    QString newName = index.data(Qt::DisplayRole).toString();
    if(newName!=textItem->text())
        textItem->setText(newName);

    falloffSpeed = index.data(UserRoles::falloff).toFloat();

    if(falloffSpeed>.0f) {
        if(index.data(UserRoles::value).toFloat()>value)
            value = index.data(UserRoles::value).toFloat();
    } else {
        float newVu = geometry().width() * index.data(UserRoles::value).toFloat();
            rectVu->setRect(0,0, newVu, geometry().height());
    }



}

void ConnectablePinView::updateVu()
{
    if(falloffSpeed==.0f)
        return;

    value-=falloffSpeed;
    if(value<.0f)
        value=.0f;

    float newVu = geometry().width() * value;
    rectVu->setRect(0,0, newVu, geometry().height());
}

void ConnectablePinView::wheelEvent ( QGraphicsSceneWheelEvent * event )
{
    event->accept();

    int increm=1;
    if(event->delta()<0)
        increm=-1;

    float val = pinIndex.data(UserRoles::value).toFloat() + pinIndex.data(UserRoles::stepSize).toFloat()*increm;
    if(val>1.0f) val=1.0f;
    if(val<.0f) val=.0f;

    model->setData(pinIndex,val,UserRoles::value);
//    emit indexEdited(pinIndex,val,UserRoles::value);
}
