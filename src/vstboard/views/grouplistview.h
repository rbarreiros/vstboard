#ifndef GROUPLISTVIEW_H
#define GROUPLISTVIEW_H

#include <QListView>

class GroupListView : public QListView
{
    Q_OBJECT
public:
    explicit GroupListView(QWidget *parent = 0);

protected:
    void startDrag ( Qt::DropActions supportedActions );
    void dragMoveEvent ( QDragMoveEvent * event );

    QModelIndex contextIndex;

signals:
    void DragOverItemFromWidget( QWidget *source, const QModelIndex &index);
    void StartDrag(const QModelIndex &index);
//    void ContextMenu(const QModelIndex &index, const QPoint & globalPos);

public slots:
    void OnContextMenu(const QPoint & pos);
    void OnContextDelete();
};

#endif // GROUPLISTVIEW_H
