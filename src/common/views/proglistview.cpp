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
#include "proglistview.h"
#include "globals.h"
#include "models/programsmodel.h"

ProgListView::ProgListView(QWidget *parent) :
    QListView(parent)
{
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(OnContextMenu(QPoint)));

    QAction *actDel = new QAction(QIcon(":/img16x16/delete.png"),tr("Delete program"),this);
    actDel->setShortcut(Qt::Key_Delete);
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

void ProgListView::startDrag(Qt::DropActions supportedActions)
{
    listDragItems = selectionModel()->selectedIndexes();
    QListView::startDrag(supportedActions);

    //go back to the current group
    emit DragFinished();
}

void ProgListView::dragMoveEvent ( QDragMoveEvent * event )
{
    if(event->source() != this) {
        event->ignore();
        return;
    }

    if (event->keyboardModifiers() & Qt::ControlModifier)
        event->setDropAction(Qt::CopyAction);
    else
        event->setDropAction(Qt::MoveAction);

    QListView::dragMoveEvent(event);
}

//void ProgListView::dropEvent(QDropEvent *e)
//{
//    QListView::dropEvent(e);
//}

void ProgListView::OnContextMenu(const QPoint & pos)
{
    NodeType::Enum t = (NodeType::Enum)currentIndex().data(UserRoles::nodeType).toInt();
    if(t == NodeType::program) {
        //item context
        QMenu menu;
        menu.exec(actions(), mapToGlobal(pos), actions().at(0), this);
    } else {
        //widget context

    }
}

void ProgListView::DeleteItem()
{
    ProgramsModel *progModel = qobject_cast<ProgramsModel*>(model());
    if(!progModel)
        return;

    progModel->removeRows(selectedIndexes(),currentIndex().parent());
}

void ProgListView::Copy()
{
    QMimeData *mime = model()->mimeData( selectionModel()->selectedIndexes() );
    QApplication::clipboard()->setMimeData( mime );
}

void ProgListView::Paste()
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
