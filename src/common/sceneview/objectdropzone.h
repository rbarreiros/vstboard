#ifndef OBJECTDROPZONE_H
#define OBJECTDROPZONE_H

#include <QGraphicsWidget>

namespace View {
    class ObjectDropZone : public QGraphicsWidget
    {
        Q_OBJECT
    public:
        explicit ObjectDropZone(QGraphicsItem *parent = 0, QWidget *parking=0);
        QWidget *GetParking() {return myParking;}

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
        void SetParking(QWidget *park) {myParking=park;}
        void UpdateHeight();
    };
}
#endif // OBJECTDROPZONE_H
