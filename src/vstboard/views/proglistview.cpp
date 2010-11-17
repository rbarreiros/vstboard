#include "precomp.h"
#include "proglistview.h"
#include "globals.h"
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
    if(currentIndex().isValid()) {
        model()->removeRow( currentIndex().row(), currentIndex().parent() );
        return;
    }
}
