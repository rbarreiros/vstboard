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
//#include "precomp.h"
#include "grouplistview.h"
#include "globals.h"
#include "models/programsmodel.h"

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

    QAction *actCopy = new QAction( QIcon(":/img16x16/editcopy.png"), "Copy", this);
    actCopy->setShortcuts(QKeySequence::Copy);
    actCopy->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(actCopy, SIGNAL(triggered()),
            this, SLOT(Copy()));
    addAction(actCopy);

    QAction *actPaste = new QAction( QIcon(":/img16x16/editpaste.png"), "Paste", this);
    actPaste->setShortcuts(QKeySequence::Paste);
    actPaste->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(actPaste, SIGNAL(triggered()),
            this, SLOT(Paste()));
    addAction(actPaste);
}

void GroupListView::dragMoveEvent ( QDragMoveEvent * event )
{
    //default behavior
    if(event->source() == this) {
        setDropIndicatorShown(true);
        QListView::dragMoveEvent(event);
        return;
    }

    //ignore by default
    event->ignore();
    setDropIndicatorShown(false);

    QModelIndex i = indexAt(event->pos());
    if(i.isValid() && event->mimeData()->formats().contains("application/x-programsdata")) {

        //show the content of the group hovered
        emit DragOverItemFromWidget( i );

        //hack to allow program drop on a group
        event->accept();
        if (event->keyboardModifiers() & Qt::ControlModifier)
            event->setDropAction(Qt::CopyAction);
        else
            event->setDropAction(Qt::MoveAction);

    } else {
        event->setDropAction(Qt::IgnoreAction);
    }

}

void GroupListView::dropEvent(QDropEvent *event)
{
    //default behavior
    if(event->source() == this) {
        QListView::dropEvent(event);
        return;
    }

    //hack to allow program drop on a group
    if(event->mimeData()->formats().contains("application/x-programsdata")) {
        QModelIndex i = indexAt(event->pos());
        if(i.isValid()) {
            model()->dropMimeData( event->mimeData(), event->dropAction(), i.row(), 0, QModelIndex() );
        } else {
            event->ignore();
            event->setDropAction(Qt::IgnoreAction);
        }
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
    ProgramsModel *progModel = qobject_cast<ProgramsModel*>(model());
    if(!progModel)
        return;

    progModel->removeRows(selectedIndexes(),currentIndex().parent());
}

void GroupListView::Copy()
{
    QMimeData *mime = model()->mimeData( selectionModel()->selectedIndexes() );
    QApplication::clipboard()->setMimeData( mime );
}

void GroupListView::Paste()
{
    QModelIndex target = currentIndex();
    if(!target.isValid())
        return;

    int row = target.row();
    int column = 0;

    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    foreach(QString type, mimeData->formats()) {
        if( model()->mimeTypes().contains( type ) ) {
            model()->dropMimeData( mimeData, Qt::CopyAction, row, column, target.parent() );
            return;
        }
    }
}
