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
    model(model)

{

}

ObjectView::~ObjectView()
{
    setActive(false);
}

void ObjectView::SetModelIndex(QPersistentModelIndex index)
{
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
    setActive(false);

    static_cast<Connectables::Container*>(
            Connectables::ObjectFactory::Get()->GetObj(objIndex.parent()).data()
        )->RemoveObject(
            Connectables::ObjectFactory::Get()->GetObj(objIndex)
        );

    event->ignore();
}

QVariant ObjectView::itemChange ( GraphicsItemChange  change, const QVariant & value )
{
    switch(change) {
        case ItemSceneHasChanged :
            if(editorButton && value.value<QGraphicsScene*>()!=0) {
                editorButton->installSceneEventFilter(this);
            }
            break;
        default:
            break;
    }
    return QGraphicsItem::itemChange(change, value);
}

void ObjectView::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
    if(model)
        model->setData(objIndex,event->newSize(),UserRoles::size);
}

bool ObjectView::sceneEventFilter ( QGraphicsItem * watched, QEvent * event )
{
    if(watched == editorButton) {
        if(event->type() == QEvent::GraphicsSceneMousePress) {
            bool val = !editorIndex.data(UserRoles::value).toBool();
            model->setData(editorIndex,val,UserRoles::value);
        }
    }

    return false;
}

void ObjectView::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent  * /*event*/ )
{
    close();
}

void ObjectView::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    QGraphicsWidget::mouseReleaseEvent(event);
    model->setData(objIndex,pos(),UserRoles::position);
}
