#ifndef OBJECTDROPZONE_H
#define OBJECTDROPZONE_H

#include <QGraphicsWidget>

namespace View {
    class ObjectDropZone : public QGraphicsWidget
    {
        Q_OBJECT
    public:
        explicit ObjectDropZone(QGraphicsItem *parent = 0);
        void SetParking(QWidget *park) {myParking=park;}

    protected:
        void dragEnterEvent( QGraphicsSceneDragDropEvent *event);
        void dragMoveEvent( QGraphicsSceneDragDropEvent *event);
        void dragLeaveEvent( QGraphicsSceneDragDropEvent *event);
        void dropEvent( QGraphicsSceneDragDropEvent *event);
        QWidget *myParking;

    signals:
        void ObjectDropped(QGraphicsSceneDragDropEvent *event);

    public slots:
        virtual void HighlightStart();
        virtual void HighlightStop();
    };
}
#endif // OBJECTDROPZONE_H
