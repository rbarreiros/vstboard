#include "precomp.h"
#include "grouplistview.h"

GroupListView::GroupListView(QWidget *parent) :
    QListView(parent)
{
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(OnContextMenu(QPoint)));
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
    contextIndex = indexAt(pos);
    QMenu menu(this);
    menu.addAction(tr("Delete"),this,SLOT(OnContextDelete()));
    menu.exec(mapToGlobal(pos));

}

void GroupListView::OnContextDelete()
{
    if(!contextIndex.isValid())
        return;

    model()->removeRow( contextIndex.row() );
}
