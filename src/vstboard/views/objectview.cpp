/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "objectview.h"
#include "pinview.h"
#include "../globals.h"
#include "mainwindow.h"

using namespace View;

ObjectView::ObjectView(QAbstractItemModel *model, QGraphicsItem * parent, Qt::WindowFlags wFlags ) :
    QGraphicsWidget(parent,wFlags),
    titleText(0),
    border(0),
    backgroundImg(0),
    editorButton(0),
    layout(0),
//    container(0),
    model(model)

{
//    if(parent)
//        container = parent;
}

ObjectView::~ObjectView()
{
    setActive(false);
//    debug("ObjectView::~ObjectView : %d deleted",objectId)
}

void ObjectView::SetModelIndex(QPersistentModelIndex index)
{
//    setObjectName(QString("ObjectView%1").arg(index.data(UserRoles::value).toInt()));

    ObjectInfo info = index.data(UserRoles::objInfo).value<ObjectInfo>();
    if(info.objType == ObjType::dummy) {
        QPixmap pix(":/img32x32/agt_action_fail.png");
        backgroundImg = new QGraphicsPixmapItem(pix,this,scene());
        backgroundImg->setToolTip(tr("Unable to load this item"));
        QPointF pt = geometry().center();
        pt.rx()-=pix.width()/2;
        pt.ry()-=pix.height()/2;
        backgroundImg->setPos( pt );

    }

    objIndex = index;
//    objectId = index.data(UserRoles::value).toInt();
    if(titleText) {
        titleText->setText(index.data(Qt::DisplayRole).toString());
    }
}

void ObjectView::UpdateModelIndex()
{
    if(!objIndex.isValid())
        return;

    if(objIndex.data(UserRoles::position).isValid())
        setPos( objIndex.data(UserRoles::position).toPointF() );

    if(titleText) {
        titleText->setText(objIndex.data(Qt::DisplayRole).toString());
    }

}

void ObjectView::SetEditorIndex(QPersistentModelIndex index)
{
    if(!editorIndex.isValid() && scene()) {
        if(editorButton) {
            editorIndex=index;
            editorButton->show();
            editorButton->installSceneEventFilter(this);
        }
    }
    editorButton->Toggle(editorIndex.data(UserRoles::value).toBool());
}

void ObjectView::closeEvent ( QCloseEvent * event )
{
//    debug("ObjectView::closeEvent %d",objectId)

    setActive(false);

//    emit removeObject(objIndex);
    static_cast<Connectables::Container*>(
            Connectables::ObjectFactory::Get()->GetObj(objIndex.parent()).data()
        )->RemoveObject(
            Connectables::ObjectFactory::Get()->GetObj(objIndex)
        );

    event->ignore();
}

/*
PinView *ObjectView::FindPin(ConnectionInfo &pin)
{
    if(!listPins.contains(pin))
        return 0;
    return listPins.value(pin);
}
*/

QVariant ObjectView::itemChange ( GraphicsItemChange  change, const QVariant & value )
{
    switch(change) {
        case ItemSceneHasChanged :
            if(editorButton && value.value<QGraphicsScene*>()!=0) {
                editorButton->installSceneEventFilter(this);
            }
            break;

//        case ItemPositionHasChanged:
//            emit indexEdited(objIndex,value,UserRoles::position);
//            break;

//        case ItemPositionChange:
//        {
//            if(container) {
//                QPointF pt = value.toPointF();
//                if(pt.x()<0)
//                    pt.setX(0);
//                if(pt.y()<0)
//                    pt.setY(0);
//                if(pt.x()+size().width() > container->rect().width())
//                    pt.setX( container->rect().width()-size().width());
//                if(pt.y()+size().height() > container->rect().height())
//                    pt.setY( container->rect().height()-size().height());
//                return pt;
//            }
//                break;
//        }

        default:
            break;
    }
    return QGraphicsItem::itemChange(change, value);
}

void ObjectView::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
//    if(!container)
//        return;

//    QPointF pt = pos();
//    if(pt.x()<0)
//        pt.setX(0);
//    if(pt.y()<0)
//        pt.setY(0);
//    if(pt.x()+size().width() > container->rect().width())
//        pt.setX( container->rect().width()-size().width());
//    if(pt.y()+size().height() > container->rect().height())
//        pt.setY( container->rect().height()-size().height());

//    setPos(pt);

//    emit indexEdited(objIndex,event->newSize(),UserRoles::size);

    if(model)
        model->setData(objIndex,event->newSize(),UserRoles::size);
}
/*
void ObjectView::UnitWithRect( QRectF &rect )
{
    QRectF child = mapToParent(rect).boundingRect().adjusted(2,2,2,2);
    QRectF myRect = geometry();

    if(!myRect.contains(child)) {
        setGeometry(myRect.united(child));
        rect.moveTopLeft( mapFromParent(child).boundingRect().topLeft() );
    }
}
*/
bool ObjectView::sceneEventFilter ( QGraphicsItem * watched, QEvent * event )
{
    if(watched == editorButton) {
        if(event->type() == QEvent::GraphicsSceneMousePress) {
            bool val = !editorIndex.data(UserRoles::value).toBool();
//            emit indexEdited(editorIndex,val,UserRoles::value);
            model->setData(editorIndex,val,UserRoles::value);
        }
    }

    return false;
}

void ObjectView::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent  * /*event*/ )
{
    close();
}

//void ObjectView::mousePressEvent ( QGraphicsSceneMouseEvent * event )
//{
//    emit Clicked();
//    QGraphicsItem::mousePressEvent(event);
//}

void ObjectView::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    QGraphicsWidget::mouseReleaseEvent(event);
//    emit indexEdited(objIndex,pos(),UserRoles::position);
    model->setData(objIndex,pos(),UserRoles::position);
}

/*

void ObjectView::Shrink()
{
    resize(0,0);
}



void ObjectView::setPen(const QPen &pen)
{
//    QColor c = pen.color();
//    c.setAlpha(150);
    if(border)
        border->setBrush(QBrush(pen.color(),Qt::SolidPattern));
}

*/
