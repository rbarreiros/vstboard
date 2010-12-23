/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    under the terms of the GNU Lesser General Public License for more details.
#
#    You should have received a copy of the under the terms of the GNU Lesser General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "objectview.h"
#include "pinview.h"
#include "../globals.h"
//#include "mainwindow.h"
#include "../mainhost.h"

using namespace View;

ObjectView::ObjectView(MainHost *myHost, QAbstractItemModel *model, QGraphicsItem * parent, Qt::WindowFlags wFlags ) :
    QGraphicsWidget(parent,wFlags),
    titleText(0),
    border(0),
    selectBorder(0),
    errorMessage(0),
    layout(0),
    model(model),
    shrinkAsked(false),
    myHost(myHost),
    listAudioIn(0),
    listAudioOut(0),
    listMidiIn(0),
    listMidiOut(0),
    listParametersIn(0),
    listParametersOut(0),
    listBridgeIn(0),
    listBridgeOut(0)
{
//    setFocusPolicy(Qt::StrongFocus);

    actDel = new QAction(QIcon(":/img16x16/delete.png"),tr("Delete"),this);
    actDel->setShortcut( Qt::Key_Delete );
    actDel->setShortcutContext(Qt::WidgetShortcut);
    connect(actDel,SIGNAL(triggered()),
            this,SLOT(close()));
    addAction(actDel);

//    actEditor = new QAction(QIcon(":/img16x16/configure.png"), tr("Show editor"),this);
//    actEditor->setEnabled(false);
//    actEditor->setCheckable(true);
//    actEditor->setShortcut( Qt::Key_Asterisk );
//    actEditor->setShortcutContext(Qt::WidgetShortcut);
//    connect(actEditor, SIGNAL(toggled(bool)),
//            this,SLOT(ShowEditor(bool)));
//    addAction(actEditor);

//    actLearn = new QAction(tr("Learning mode"),this);
//    actLearn->setEnabled(false);
//    actLearn->setCheckable(true);
//    actLearn->setShortcut( Qt::Key_Plus );
//    actLearn->setShortcutContext(Qt::WidgetShortcut);
//    connect(actLearn, SIGNAL(toggled(bool)),
//            this,SLOT(ToggleLearningMode(bool)));
//    addAction(actLearn);

    backgroundImg = new QGraphicsPixmapItem(this,scene());
}

ObjectView::~ObjectView()
{
    setActive(false);
}

void ObjectView::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    menu.exec(actions(),event->screenPos(),actions().at(0),event->widget());
}

//void ObjectView::ShowEditor(bool show)
//{
//    model->setData(objIndex,show,UserRoles::editorVisible);
//}

//void ObjectView::ToggleLearningMode(bool learn)
//{
//    model->setData(objIndex,learn,UserRoles::paramLearning);
//}

void ObjectView::SetModelIndex(QPersistentModelIndex index)
{
    ObjectInfo info = index.data(UserRoles::objInfo).value<ObjectInfo>();
    if(info.objType == ObjType::dummy) {
        SetErrorMessage("object not loaded");
    }

//    actEditor->setEnabled( objIndex.data(UserRoles::hasEditor).toBool() );
//    actLearn->setEnabled( objIndex.data(UserRoles::canLearn).toBool() );

    if(info.nodeType == NodeType::bridge) {
        actDel->setEnabled(false);
    }

    objIndex = index;
    if(titleText) {
        titleText->setText(index.data(Qt::DisplayRole).toString());
    }

//    if(index.data(UserRoles::editorImage).isValid()) {
//        SetBackground( index.data(UserRoles::editorImage).toString() );
//    }
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

    if(objIndex.data(UserRoles::errorMessage).isValid()) {
        QString err = objIndex.data(UserRoles::errorMessage).toString();
        SetErrorMessage( err );
    }
}

void ObjectView::SetErrorMessage(const QString & msg)
{
    if(msg.isEmpty()) {
        if(errorMessage) {
            delete errorMessage;
            errorMessage=0;
            setToolTip("");
        }
    } else {
        if(!errorMessage) {
            QPixmap pix(":/img32x32/agt_action_fail.png");
            errorMessage = new QGraphicsPixmapItem(pix,this,scene());
//            QPointF pt = geometry().center();
//            pt.rx() -= pix.width()/2;
//            pt.ry() -= pix.height()/2;
//            errorMessage->setPos(pt);
            errorMessage->setPos(0,0);
        }
        setToolTip(msg);
    }
}

//void ObjectView::SetBackground(const QString & imgName)
//{
//    if(!backgroundImg->pixmap().isNull() &&
//       backgroundImg->pixmap().size() == boundingRect().size().toSize())
//        return;

//    QPixmap pix = ImageCollection::Get()->GetImage( imgName ).copy(boundingRect().toRect());
//    if(pix.isNull())
//        return;

//    backgroundImg->setPixmap(pix);
//    QGraphicsBlurEffect *eff = new QGraphicsBlurEffect(this);
//    eff->setBlurRadius(2);
//    backgroundImg->setGraphicsEffect(eff);
//}

void ObjectView::closeEvent ( QCloseEvent * event )
{
    setActive(false);

    static_cast<Connectables::Container*>(
            myHost->objFactory->GetObj(objIndex.parent()).data()
        )->ParkObject(
            myHost->objFactory->GetObj(objIndex)
        );

    event->ignore();
}

QVariant ObjectView::itemChange ( GraphicsItemChange  change, const QVariant & value )
{
    switch(change) {
        case QGraphicsItem::ItemSelectedChange :
            if(value.toBool()) {
                if(selectBorder)
                    delete selectBorder;
                selectBorder=new QGraphicsRectItem( -2,-2, size().width()+4, size().height()+4 , this );
            } else {
                if(selectBorder) {
                    delete selectBorder;
                    selectBorder =0;
                }
            }
            break;
        default:
            break;
    }
    return QGraphicsItem::itemChange(change, value);
}

void ObjectView::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
    if(selectBorder)
        selectBorder->setRect( -2, -2, event->newSize().width()+4, event->newSize().height()+4);
//    if(model)
//        model->setData(objIndex,event->newSize(),UserRoles::size);
}

void ObjectView::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    QGraphicsWidget::mouseReleaseEvent(event);
    model->setData(objIndex,pos(),UserRoles::position);
}

void ObjectView::Shrink()
{
    if(shrinkAsked)
        return;

    shrinkAsked=true;
    QTimer::singleShot(100, this, SLOT(ShrinkNow()));
}

void ObjectView::ShrinkNow()
{
    shrinkAsked=false;
    resize(0,0);
}
