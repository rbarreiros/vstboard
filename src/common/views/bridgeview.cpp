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

#include "bridgeview.h"
#include "../mainhost.h"
using namespace View;

BridgeView::BridgeView(MainHost *myHost,QAbstractItemModel *model, QGraphicsItem * parent, Qt::WindowFlags wFlags) :
        ObjectView(myHost,model,parent,wFlags),
        lLayout(0)
{
    setGeometry(QRectF(0,0,0,0));

    lLayout = new QGraphicsLinearLayout(Qt::Horizontal) ;
    lLayout->setSpacing(0);
    lLayout->setContentsMargins(0,0,0,0);
    setLayout(lLayout);

    listBridge = new ListPinsView(this);
    listBridge->layout->setOrientation(Qt::Horizontal);
    lLayout->addItem(listBridge);
}

void BridgeView::moveEvent ( QGraphicsSceneMoveEvent * event )
{
    listBridge->UpdateCablesPosition();
}
