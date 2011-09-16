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
#include "precomp.h"
#include "minmaxpinview.h"
#include "objectview.h"

using namespace View;

MinMaxPinView::MinMaxPinView(float angle, QAbstractItemModel *model,QGraphicsItem * parent, const ConnectionInfo &pinInfo, ViewConfig *config) :
    ConnectablePinView(angle,model,parent,pinInfo,config),
    cursorCreated(false),
    changingValue(false),
    startDragValue(.0f),
    inMin(0),
    inMax(0),
    outMin(0),
    outMax(0),
    scaledView(0)
{

}

void MinMaxPinView::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
    ConnectablePinView::resizeEvent(event);

    if(cursorCreated) {
        inMin->setPos( inMin->GetValue()*event->newSize().width(), 0 );
        inMax->setPos( inMax->GetValue()*event->newSize().width(), 0 );
        outMin->setPos( outMin->GetValue()*event->newSize().width(), 0 );
        outMax->setPos( outMax->GetValue()*event->newSize().width(), 0 );
        UpdateScaleView();
    }
}

void MinMaxPinView::CreateCursors()
{
    cursorCreated=true;

    scaledView = new QGraphicsPolygonItem(this);
    scaledView->setBrush(QColor(0,0,0,30));

    inMin=new CursorView(model,false,false,this,config);
    inMin->setPos(rect().topLeft());
    inMin->SetValue(.0f);

    inMax=new CursorView(model,true,false,this,config);
    inMax->setPos(rect().topRight());
    inMax->SetValue(1.0f);

    outMin=new CursorView(model,false,true,this,config);
    outMin->setPos(rect().bottomLeft());
    outMin->SetValue(.0f);

    outMax=new CursorView(model,true,true,this,config);
    outMax->setPos(rect().bottomRight());
    outMax->SetValue(1.0f);
}

void MinMaxPinView::SetLimitModelIndex(ObjType::Enum type, QPersistentModelIndex index)
{
    if(!cursorCreated)
        CreateCursors();

    switch(type) {
        case ObjType::limitInMin:
            inMin->SetModelIndex(index);
            break;
        case ObjType::limitInMax:
            inMax->SetModelIndex(index);
            break;
        case ObjType::limitOutMin:
            outMin->SetModelIndex(index);
            break;
        case ObjType::limitOutMax:
            outMax->SetModelIndex(index);
            break;
        default:
            LOG("unknown type"<<type);
            break;
    }
}

void MinMaxPinView::UpdateLimitModelIndex(const QModelIndex &index)
{
    inMin->SetValue( index.child(0,0).data(UserRoles::value).toFloat() );
    inMax->SetValue( index.child(1,0).data(UserRoles::value).toFloat() );
    outMin->SetValue( index.child(2,0).data(UserRoles::value).toFloat() );
    outMax->SetValue( index.child(3,0).data(UserRoles::value).toFloat() );
    UpdateScaleView();
}

void MinMaxPinView::UpdateModelIndex(const QModelIndex &index)
{
    ConnectablePinView::UpdateModelIndex(index);

    //avoid "jumps"
    if(changingValue)
        value = startDragValue;
    else
        value = index.data(UserRoles::value).toFloat();

    float newVu = geometry().width() * value;
    rectVu->setRect(0,0, newVu, geometry().height());

    if(cursorCreated)
        UpdateScaleView();

    ObjectView *parentObj = static_cast<ObjectView*>(parentWidget()->parentWidget());
    if(parentObj) {
        if(connectInfo.pinNumber == FixedPinNumber::editorVisible) {
            parentObj->SetEditorPin(this, value);
        }
        if(connectInfo.pinNumber == FixedPinNumber::learningMode) {
            parentObj->SetLearnPin(this, value);
        }
    }
}

void MinMaxPinView::UpdateScaleView()
{
    float limitVal=value;
    if(limitVal<inMin->GetValue())
        limitVal=inMin->GetValue();
    if(limitVal>inMax->GetValue())
        limitVal=inMax->GetValue();

    float outVal=limitVal;
    outVal-=inMin->GetValue();

    if(inMax->GetValue()!=inMin->GetValue())
        outVal/=(inMax->GetValue()-inMin->GetValue());
    outVal*=(outMax->GetValue()-outMin->GetValue());
    outVal+=outMin->GetValue();

    limitVal*=rect().width();
    outVal*=rect().width();

    QPolygonF pol;
    pol << QPointF(limitVal,0) << QPointF(inMin->GetValue(),0)  << QPointF(outMin->GetValue(),rect().height()) << QPointF(outVal,rect().height());
    scaledView->setPolygon(pol);
}

void MinMaxPinView::ValueChanged(float newVal)
{
    if(value==newVal)
        return;
    if(newVal>1.0f) newVal=1.0f;
    if(newVal<0.0f) newVal=0.0f;

    model->setData(pinIndex,newVal,UserRoles::value);
}

void MinMaxPinView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    const KeyBind::MoveBind b = config->keyBinding->GetMoveSortcuts(KeyBind::changeValue);
    if(b.input == KeyBind::mouse && b.buttons == event->buttons() && b.modifier == event->modifiers()) {
        changingValue=true;
        startDragPos=event->pos();
        startDragValue=pinIndex.data(UserRoles::value).toFloat();
        grabMouse();
        return;
    }

//    if(config->EditMode()!=EditMode::Value) {
        ConnectablePinView::mousePressEvent(event);
        return;
//    }

//    changingValue=true;
//    startDragPos=event->screenPos();
//    startDragValue=pinIndex.data(UserRoles::value).toFloat();
//    grabMouse();
}

void MinMaxPinView::mouseMoveEvent ( QGraphicsSceneMouseEvent  * event )
{
    if(changingValue) {
        float mouseSensibility = 1.0f/size().width();
        if(event->modifiers() & Qt::ControlModifier)
            mouseSensibility /= 10;
        if(event->modifiers() & Qt::ShiftModifier)
            mouseSensibility /= 10;
        if(event->modifiers() & Qt::AltModifier)
            mouseSensibility /= 10;

        qreal increm = event->pos().x() - startDragPos.x();
        startDragValue += mouseSensibility*increm;
        startDragValue = std::max(.0f,startDragValue);
        startDragValue = std::min(1.0f,startDragValue);

        startDragPos=event->pos();
        ValueChanged( startDragValue );
    } else {
        ConnectablePinView::mouseMoveEvent(event);
    }
}

void MinMaxPinView::mouseReleaseEvent ( QGraphicsSceneMouseEvent  * event )
{
    if(changingValue) {
        event->accept();
        ungrabMouse();
        changingValue=false;
    }
    ConnectablePinView::mouseReleaseEvent(event);
}

void MinMaxPinView::wheelEvent ( QGraphicsSceneWheelEvent * event )
{
    const KeyBind::MoveBind b = config->keyBinding->GetMoveSortcuts(KeyBind::changeValue);
    if(b.input == KeyBind::mouseWheel && b.modifier == event->modifiers()) {
        event->accept();

        int increm=1;
        if(event->delta()<0)
            increm=-1;

        ValueChanged( pinIndex.data(UserRoles::value).toFloat()
                      + pinIndex.data(UserRoles::stepSize).toFloat()*increm);
    }
}

void MinMaxPinView::keyPressEvent ( QKeyEvent * event )
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
