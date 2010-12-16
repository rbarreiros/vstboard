/**************************************************************************
#    Copyright 2010 Rapha�l Fran�ois
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

#include "minmaxpinview.h"

using namespace View;

MinMaxPinView::MinMaxPinView(float angle, QAbstractItemModel *model,QGraphicsItem * parent, Connectables::Pin *pin) :
        ConnectablePinView(angle,model,parent,pin)
{
    inMin=new CursorView(model,false,this);
    inMin->setPos(rect().topLeft());
    inMin->SetValue(.0f);

    inMax=new CursorView(model,false,this);
    inMax->setPos(rect().topLeft());
    inMax->SetValue(1.0f);

    outMin=new CursorView(model,true,this);
    outMin->setPos(rect().bottomLeft());
    outMin->SetValue(.0f);

    outMax=new CursorView(model,true,this);
    outMax->setPos(rect().bottomLeft());
    outMax->SetValue(1.0f);
}

void MinMaxPinView::SetLimitModelIndex(ObjType::Enum type, QPersistentModelIndex index)
{
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
            debug("MinMaxPinView::SetLimitModelIndex unknown type")
            break;
    }
}

void MinMaxPinView::UpdateLimitModelIndex(const QModelIndex &index)
{
    inMin->SetValue( index.child(0,0).data(UserRoles::value).toFloat() );
    inMax->SetValue( index.child(1,0).data(UserRoles::value).toFloat() );
    outMin->SetValue( index.child(2,0).data(UserRoles::value).toFloat() );
    outMax->SetValue( index.child(3,0).data(UserRoles::value).toFloat() );
}
