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

#include "bridgeview.h"

using namespace View;

BridgeView::BridgeView(QAbstractItemModel *model, QGraphicsItem * parent, Qt::WindowFlags wFlags) :
        ConnectableObjectView(model,parent,wFlags)
{
    setGeometry(QRectF(0,0,0,0));
    border->hide();
    titleText->hide();
    editorButton->hide();
    layout->setContentsMargins(0,0,0,0);

    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
}

void BridgeView::moveEvent ( QGraphicsSceneMoveEvent * event )
{
    listBridgeIn->UpdateCablesPosition();
    listBridgeOut->UpdateCablesPosition();
}
