#ifndef PROGLISTVIEW_H
#define PROGLISTVIEW_H

#include <QListView>

class ProgListView : public QListView
{
    Q_OBJECT
public:
    explicit ProgListView(QWidget *parent = 0);

protected:
    void dragMoveEvent ( QDragMoveEvent * event );

signals:

public slots:
    void OnContextMenu(const QPoint & pos);
    void DeleteItem();
};

#endif // PROGLISTVIEW_H
