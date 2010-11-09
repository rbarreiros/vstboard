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

#include "containerview.h"
#include "../connectables/container.h"
#include "../connectables/objectfactory.h"
#include "bridgepinview.h"
#include "connectableobjectview.h"

using namespace View;

ContainerView::ContainerView(QAbstractItemModel *model, QGraphicsItem *parent) :
    ObjectView(model, parent)
{
//    setObjectName("ContainerView");
}

ContainerView::~ContainerView()
{
    foreach(CableView *c, listCables) {
        c->scene()->removeItem(c);
        delete c;
    }
}

void ContainerView::closeEvent ( QCloseEvent * event )
{
    ObjectView::closeEvent(event);
}

QPointF ContainerView::GetDropPos()
{
    return mapFromScene( content->GetDropPos() );
}

void ContainerView::UpdateModelIndex()
{
    if(!objIndex.isValid())
        return;

    ObjectView::UpdateModelIndex();

    if(objIndex.data(UserRoles::position).isValid())
        resize(objIndex.data(UserRoles::size).toSizeF() );
}

void ContainerView::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
    ObjectView::resizeEvent(event);

    //when resizing : "position changed" event is not sent to children, force it :
    bridgeOut->listBridgeOut->UpdateCablesPosition();
    bridgeOut->listBridgeIn->UpdateCablesPosition();

}
