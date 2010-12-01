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
}

void GroupListView::startDrag ( Qt::DropActions supportedActions )
{
    emit StartDrag(currentIndex());
    QListView::startDrag(supportedActions);
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
