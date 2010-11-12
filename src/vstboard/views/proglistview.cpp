#include <QDragMoveEvent>
#include "proglistview.h"
#include "globals.h"
ProgListView::ProgListView(QWidget *parent) :
    QListView(parent)
{
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(OnContextMenu(QPoint)));
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
    contextIndex = indexAt(pos);
    NodeType::Enum t = (NodeType::Enum)contextIndex.data(UserRoles::nodeType).toInt();
    if(t == NodeType::program) {
        //item context
        QMenu menu(this);
        menu.addAction(tr("Delete"),this,SLOT(OnContextDelete()));
        menu.exec(mapToGlobal(pos));
    } else {
        //widget context

    }


}

void ProgListView::OnContextDelete()
{
    if(!contextIndex.isValid())
        return;

    model()->removeRow( contextIndex.row(), contextIndex.parent() );
}
