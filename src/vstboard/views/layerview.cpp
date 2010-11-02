//#include "layerview.h"
//#include "../connectables/objectfactory.h"
////#include "../hostlayer.h"
////#include "../mainhost.h"
//#include "containerview.h"
//#include "connectableobjectview.h"
//#include "graphicscene.h"
////#ifdef VSTSDK
////    #include "../connectables/vstplugin.h"
////#endif

//using namespace View;

//LayerView::LayerView(int layer, ObjectView *parent) :
//        ObjectView(parent),
//        layer(layer),
//        hidden(false)
//{
//    debug("add view layer %d",layer)

//    setAcceptDrops(true);
//    setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
//    setWindowFlags(Qt::Tool);
//}

//LayerView::~LayerView()
//{
//    foreach(Cable *c, listCables) {
//        c->scene()->removeItem(c);
//        delete c;
//    }
//}

//void LayerView::Show()
//{
//    if(!hidden)
//        return;

//    foreach(ObjectView *obj, listObjects) {
//        obj->show();
//    }

//    foreach(Cable *c, listCables) {
//        c->show();
//    }

//    hidden = false;
//}

//void LayerView::Hide()
//{
//    if(hidden)
//        return;

//    foreach(ObjectView *obj, listObjects) {
//        obj->hide();
//    }

//    foreach(Cable *c, listCables) {
//        c->hide();
//    }

//    hidden = true;
//}

//void LayerView::OnAddObject(ObjectView *objView, int id, int containerId, const QPointF &dropPos)
//{
//    if(containerId!=-1) {
//        static_cast<ContainerView*>(listObjects.value(containerId))->OnAddChildObject(id);
//        return ;
//    }


//    if(hidden)
//        objView->hide();
//    listObjects[id] = objView;
//    objView->setPos(dropPos);
//    objView->setPen(layerColor);
//}

//void LayerView::OnRemoveObject(int id, int containerId)
//{
//    if(containerId!=-1) {
//        static_cast<ContainerView*>(listObjects.value(containerId))->OnRemoveChildObject(id);
//        return ;
//    }

//    ObjectView *obj = listObjects.take(id);
//    if(!obj) {
//        debug("LayerView::OnRemoveObject : obj %d not found",id)
//        return;
//    }

//    debug("LayerView::OnRemoveObject : obj %d removed from scene",id)
//    if(obj->scene()) {
//        obj->scene()->removeItem(obj);
//    }

//    //containers crash if deleted
////    obj->deleteLater();
//}

//PinView *LayerView::GetPinView(ConnectionInfo pin)
//{
//    if(!listObjects.contains(pin.objId)) {
//        debug("LayerView::GetPinView : object %d not found",pin.objId)
//        return 0;
//    }
//    return listObjects.value(pin.objId)->GetPinView(pin);
//}

//Cable * LayerView::OnAddConnection(ConnectionInfo pinOut, ConnectionInfo pinIn)
//{
//    Cable *cable = new Cable(pinOut,pinIn);
////    listCables.insert(pinOut, cable);
//    listCables << cable;
//    cable->setPen(layerColor);
//    if(hidden)
//        cable->hide();
//    return cable;
//}

//Cable * LayerView::OnRemoveConnection(ConnectionInfo pinOut, ConnectionInfo pinIn)
//{
//    //only one cable can connect two pins
////    QMultiHash<ConnectionInfo, Cable*>::iterator i = listCables.find(pinOut);
////    while (i != listCables.end() && i.key()==pinOut) {
////        if(i.value()->pinIn == pinIn)
////            return i.value();
////        ++i;
////    }

//    foreach(Cable *c, listCables) {
//        if(c->pinOut == pinOut && c->pinIn == pinIn) {
//            listCables.removeAll(c);
//            return c;
//        }
//    }
//    return 0;
//}

//void LayerView::ClearLayer()
//{
//    debug("clear view layer %d",layer)

//    foreach(Cable *c, listCables) {
//        delete c;
//    }

//    listCables.clear();

////    foreach(ObjectView *obj, listObjects) {
//////        obj->close();
////        obj->deleteLater();
////    }

//    while(!listObjects.isEmpty()) {
//        ObjectView *obj = listObjects.begin().value();
//        obj->scene()->removeItem(obj);
//        obj->deleteLater();
//        listObjects.erase(listObjects.begin());
//    }
//    //listObjects.clear();
//}

//QDataStream & LayerView::toStream(QDataStream& stream) const
//{
//    stream << hidden;
//    stream << (qint16)listObjects.size();
//    QHash<int,ObjectView*>::const_iterator i = listObjects.constBegin();
//    while(i!=listObjects.constEnd()) {
//        stream << (qint16)i.key();
//        stream << i.value()->pos();
//        ++i;
//    }
//    return stream;
//}

//QDataStream & LayerView::fromStream(QDataStream& stream)
//{
//    bool savedHidden;
//    stream >> savedHidden;
//    if(savedHidden)
//        Hide();

//    qint16 nbObj;
//    stream >> nbObj;
//    for(int i=0;i<nbObj;i++) {
//        qint16 objId;
//        stream >> objId;
//        int id = Connectables::ObjectFactory::Get()->IdFromSavedId(objId);
//        if(id==-1) {
//            debug("LayerView::fromStream : obj not found in host")
//            return stream;
//        }
//        QPointF pt;
//        stream >> pt;

//        if(!listObjects.contains(id)) {
//            debug("LayerView::fromStream : obj not found in scene")
//            return stream;
//        }
//        listObjects.value(id)->setPos(pt);
//    }
//    return stream;
//}

//QDataStream & operator<< (QDataStream& stream, const View::LayerView& layer)
//{
//    return layer.toStream(stream);
//}

//QDataStream & operator>> (QDataStream& stream, View::LayerView& layer)
//{
//    return layer.fromStream(stream);
//}
