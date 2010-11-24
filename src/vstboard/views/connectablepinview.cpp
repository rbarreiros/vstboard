/******************************************************************************
#    Copyright 2010 Raphaël François
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
******************************************************************************/

#include "connectablepinview.h"
#include "../connectables/objectfactory.h"

using namespace View;

QBrush ConnectablePinView::normalBrush;
QBrush ConnectablePinView::highBrush;

ConnectablePinView::ConnectablePinView(float angle, QAbstractItemModel *model, QGraphicsItem * parent, Connectables::Pin *pin) :
       PinView(angle,model,parent,pin),
       value(0),
       isParameter(false)
{
    falloffSpeed = .05f;

    setGeometry(0,0,50,15);
    setMinimumSize(50,15);
    setMaximumSize(50,15);

    rectBgnd = new QGraphicsRectItem(geometry(), this);

//    textItem = new OutlinedText("", this, Qt::black, QColor(255,255,255,150));
    textItem = new QGraphicsSimpleTextItem(this);
    textItem->moveBy(2,1);

    rectVu = new QGraphicsRectItem(this);
    rectBgnd->setBrush(Qt::NoBrush);

    QLinearGradient normalgradient(0, 0, geometry().right(), 0);
    normalgradient.setColorAt(0, QColor::fromRgbF(0, 0, 0, 0));
    normalgradient.setColorAt(1, QColor::fromRgbF(0, 1, 0, 1));
    normalBrush = QBrush(normalgradient);

    QLinearGradient highgradient(0, 0, geometry().right(), 0);
    highgradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 0));
    highgradient.setColorAt(1, QColor::fromRgbF(0, 0, 1, 1));
    highBrush = QBrush(highgradient);

    rectVu->setBrush(normalBrush);
}

void ConnectablePinView::UpdateModelIndex(const QModelIndex &index)
{
    QString newName = index.data(Qt::DisplayRole).toString();
    if(newName!=textItem->text())
        textItem->setText(newName);

    falloffSpeed = index.data(UserRoles::falloff).toFloat();

    if(falloffSpeed>.0f) {
        if(index.data(UserRoles::value).toFloat()>value)
            value = index.data(UserRoles::value).toFloat();
    } else {
        value = index.data(UserRoles::value).toFloat();
        float newVu = geometry().width() * value;
        rectVu->setRect(0,0, newVu, geometry().height());
    }

    ConnectionInfo pinInfo = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();
    if(pinInfo.type == PinType::Parameter)
        isParameter=true;

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

//    if(value>=.9f) {
//        rectVu->setBrush(highBrush);
//    } else {
//        rectVu->setBrush(normalBrush);
//    }
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
