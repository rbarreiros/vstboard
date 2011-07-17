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

    actAddNew = new QAction(QIcon(":/img16x16/edit_add.png"),tr("Insert a new program"),this);
    actAddNew->setShortcut( Qt::Key_Insert );
    actAddNew->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(actAddNew,SIGNAL(triggered()),
            this,SLOT(InsertItem()));
    addAction(actAddNew);

    actDel = new QAction(QIcon(":/img16x16/delete.png"),tr("Delete program"),this);
    actDel->setShortcut(Qt::Key_Delete);
    actDel->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(actDel,SIGNAL(triggered()),
            this,SLOT(DeleteItem()));
    addAction(actDel);

    actCopy = new QAction( QIcon(":/img16x16/editcopy.png"), "Copy", this);
    actCopy->setShortcuts(QKeySequence::Copy);
    actCopy->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(actCopy, SIGNAL(triggered()),
            this, SLOT(Copy()));
    addAction(actCopy);

    actPaste = new QAction( QIcon(":/img16x16/editpaste.png"), "Paste", this);
    actPaste->setShortcuts(QKeySequence::Paste);
    actPaste->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(actPaste, SIGNAL(triggered()),
            this, SLOT(Paste()));
    addAction(actPaste);
}

void ProgListView::startDrag(Qt::DropActions supportedActions)
{
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
    QListView::dragMoveEvent(event);
}

void ProgListView::OnContextMenu(const QPoint & pos)
{
    if(!selectedIndexes().isEmpty()) {
        actDel->setEnabled(true);
        actCopy->setEnabled(true);
    } else {
        actDel->setEnabled(false);
        actCopy->setEnabled(false);
    }

    actPaste->setEnabled(false);

    const QMimeData *mime =  QApplication::clipboard()->mimeData();
    if( mime->hasFormat(MIMETYPE_PROGRAM) )
        actPaste->setEnabled(true);
    else
        actPaste->setEnabled(false);

    QMenu menu;
    menu.exec(actions(), mapToGlobal(pos), actions().at(0), this);
}

void ProgListView::DeleteItem()
{
    ProgramsModel *progModel = qobject_cast<ProgramsModel*>(model());
    if(!progModel)
        return;

    if(!selectedIndexes().isEmpty())
        progModel->removeRows(selectedIndexes(),rootIndex());
    else if(currentIndex().isValid())
        progModel->removeRows(QModelIndexList()<<currentIndex(),rootIndex());
}

void ProgListView::InsertItem()
{
    ProgramsModel *progModel = qobject_cast<ProgramsModel*>(model());
    if(!progModel)
        return;

    int row=-1;
    if(currentIndex().isValid())
        row=currentIndex().row();

    progModel->NewProgram( rootIndex().parent().row(), row );
}

void ProgListView::Copy()
{
    QMimeData *mime=0;

    if(!selectedIndexes().isEmpty())
        mime = model()->mimeData( selectedIndexes() );
    else if(currentIndex().isValid())
        mime = model()->mimeData( QModelIndexList() << currentIndex() );

    if(!mime)
        return;

    QApplication::clipboard()->setMimeData( mime );
}

void ProgListView::Paste()
{
    int row=-1;
    if(currentIndex().isValid())
        row=currentIndex().row();

    const QMimeData *mime = QApplication::clipboard()->mimeData();
    model()->dropMimeData( mime, Qt::CopyAction, row, 0, rootIndex() );
}
