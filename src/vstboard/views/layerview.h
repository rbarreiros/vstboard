//#ifndef LAYERVIEW_H
//#define LAYERVIEW_H

//#include "../precomp.h"
//#include "../globals.h"
//#include "../connectables/object.h"
//#include "objectview.h"
//#include "cable.h"

//namespace View {
//    class LayerView : public ObjectView
//    {
//    Q_OBJECT
//    public:
//        LayerView(int layer, ObjectView *parent=0);
////        void SetObject(Connectables::Object* obj);
//        ~LayerView();
//        int GetId() {return layer;}
//        void Show();
//        void Hide();
//        PinView *GetPinView(ConnectionInfo pin);

//        Cable * OnAddConnection(ConnectionInfo pinOut, ConnectionInfo pinIn);
//        Cable * OnRemoveConnection(ConnectionInfo pinOut, ConnectionInfo pinIn);

//        QDataStream & toStream(QDataStream& stream) const;
//        QDataStream & fromStream(QDataStream& stream);

//        QPen layerColor;

//    protected:

//        int layer;
//        bool hidden;
//        QHash<int,ObjectView*>listObjects;
////        QMultiHash<ConnectionInfo, Cable*>listCables;
//        QList<Cable*>listCables;



//    public slots:
//        void OnAddObject(ObjectView *objView, int id, int containerId, const QPointF &dropPos);
//        void OnRemoveObject(int id, int containerId);
//        void ClearLayer();
//    };

//}

//QDataStream & operator<< (QDataStream& stream, const View::LayerView& layer);
//QDataStream & operator>> (QDataStream& stream, View::LayerView& layer);

//#endif // LAYERVIEW_H
