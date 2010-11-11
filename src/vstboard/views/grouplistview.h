#ifndef GROUPLISTVIEW_H
#define GROUPLISTVIEW_H

#include <QListView>

class GroupListView : public QListView
{
    Q_OBJECT
public:
    explicit GroupListView(QWidget *parent = 0);

protected:
    void dragMoveEvent ( QDragMoveEvent * event );

signals:
    void DragOverItemFromWidget( QWidget *source, QModelIndex index);

public slots:

};

#endif // GROUPLISTVIEW_H
