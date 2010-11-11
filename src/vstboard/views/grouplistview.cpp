#include <QDragEnterEvent>
#include "grouplistview.h"

GroupListView::GroupListView(QWidget *parent) :
    QListView(parent)
{
}


void GroupListView::dragMoveEvent ( QDragMoveEvent * event )
{
    if(event->source() == this) {
        QListView::dragMoveEvent(event);
    } else {
        event->setAccepted(false);
        QModelIndex i = indexAt(event->pos());
        if(i.isValid())
            emit DragOverItemFromWidget( event->source(), i);
    }
}
