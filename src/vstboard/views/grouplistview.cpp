#include <QDragEnterEvent>
#include "grouplistview.h"

GroupListView::GroupListView(QWidget *parent) :
    QListView(parent)
{
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
