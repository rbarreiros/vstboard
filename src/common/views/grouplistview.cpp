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


#include "precomp.h"
#include "grouplistview.h"
#include "globals.h"

GroupListView::GroupListView(QWidget *parent) :
    QListView(parent)
{
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(OnContextMenu(QPoint)));

    QAction *actDel = new QAction(QIcon(":/img16x16/delete.png"),tr("Delete group"),this);
    actDel->setShortcut( Qt::Key_Delete );
    actDel->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(actDel,SIGNAL(triggered()),
            this,SLOT(DeleteItem()));
    addAction(actDel);
    setSelectionMode(ExtendedSelection);
}

void GroupListView::dragMoveEvent ( QDragMoveEvent * event )
{
    if(event->source() == this) {
        setDropIndicatorShown(true);
        QListView::dragMoveEvent(event);
    } else {
        event->setAccepted(false);
        setDropIndicatorShown(false);
        QModelIndex i = indexAt(event->pos());
        if(i.isValid())
            emit DragOverItemFromWidget( event->source(), i);
    }
}

void GroupListView::OnContextMenu(const QPoint & pos)
{
    NodeType::Enum t = (NodeType::Enum)currentIndex().data(UserRoles::nodeType).toInt();
    if(t == NodeType::programGroup) {
        //group context
        QMenu menu;
        menu.exec(actions(), mapToGlobal(pos), actions().at(0), this);
    } else {
        //widget context
    }
}

void GroupListView::DeleteItem()
{
    if(currentIndex().isValid()) {
        model()->removeRow( currentIndex().row() );
        return;
    }
}
