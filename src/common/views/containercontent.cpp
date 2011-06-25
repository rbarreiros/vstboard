/**************************************************************************
#    Copyright 2010-2011 Raphaël François
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
**************************************************************************/

#include "containercontent.h"
#include "objectview.h"
#include "maincontainerview.h"

#ifdef _MSC_VER
#pragma warning( disable: 4100 )
#endif

using namespace View;

ContainerContent::ContainerContent(MainHost *myHost, QAbstractItemModel *model, MainContainerView * parent, Qt::WindowFlags wFlags ) :
    QGraphicsWidget(parent,wFlags),
    model(model),
    containerView(parent),
    rectAttachLeft(0),
    rectAttachRight(0)
{
    config = &myHost->mainWindow->viewConfig;
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setGeometry(0,0,50,50);

    dropPos.setX(0);
    dropPos.setY(0);

    setAutoFillBackground(true);
    QPalette pal(palette());
    pal.setColor(QPalette::Window, config->GetColor(ColorGroups::Panel,Colors::Background) );
    setPalette( pal );

    connect( config, SIGNAL(ColorChanged(ColorGroups::Enum,Colors::Enum,QColor)),
            this, SLOT(UpdateColor(ColorGroups::Enum,Colors::Enum,QColor)) );
}

void ContainerContent::SetModelIndex(QPersistentModelIndex index)
{
    objIndex = index;
}

QPointF ContainerContent::GetDropPos()
{
    QPointF p = dropPos;
    dropPos.rx()+=20;
    dropPos.ry()+=20;
    return p;
}

void ContainerContent::dragEnterEvent( QGraphicsSceneDragDropEvent *event)
{
    //accepts objects from parking
    if(event->source() == myParking ) {
        QGraphicsWidget::dragEnterEvent(event);
        HighlightStart();
        return;
    }

    if (event->mimeData()->hasUrls()) {
        QString fName;
        QFileInfo info;

        foreach(QUrl url,event->mimeData()->urls()) {
            fName = url.toLocalFile();
            info.setFile( fName );

            if ( info.isFile() && info.isReadable() ) {
#ifdef VSTSDK
                //accept DLL files
                if( info.suffix()=="dll" ) {
                    event->setDropAction(Qt::CopyAction);
                    event->accept();
                    HighlightStart();
                    return;
                }
#endif

                //accept setup and projects files
                if ( info.suffix()==SETUP_FILE_EXTENSION || info.suffix()==PROJECT_FILE_EXTENSION ) {
                    event->setDropAction(Qt::CopyAction);
                    event->accept();
                    HighlightStart();
                    return;
                }
            }
        }
    }


    //accept Audio interface
    //accept Midi interface
    //accept Tools
    if(event->mimeData()->hasFormat("application/x-audiointerface") ||
       event->mimeData()->hasFormat("application/x-midiinterface") ||
       event->mimeData()->hasFormat("application/x-tools")) {
        event->setDropAction(Qt::CopyAction);
        event->accept();
        HighlightStart();
        return;
    }
    event->ignore();
}

void ContainerContent::dragMoveEvent( QGraphicsSceneDragDropEvent *event)
{
    if(!objIndex.isValid())
        return;

    QPointF pt = mapToScene(event->pos());
    QSizeF attachZoneSize(20,45);
    QModelIndex idx;
    int i=0;

    //remove attachLeft on mouseout
    if(attachLeft.isValid()) {
        QPointF p = attachLeft.data(UserRoles::position).toPointF();
        p.rx()-=10;
        QRectF attachZone(p,attachZoneSize);

        if(!attachZone.contains(pt)) {
            if(rectAttachLeft) {
                delete rectAttachLeft;
                rectAttachLeft = 0;
            }
            attachLeft=QModelIndex();
        }
    }

    //remove attachRight on mouseout
    if(attachRight.isValid()) {
        QPointF p = attachRight.data(UserRoles::position).toPointF();
        p.rx()+=105;
        QRectF attachZone(p,attachZoneSize);

        if(!attachZone.contains(pt)) {
            if(rectAttachRight) {
                delete rectAttachRight;
                rectAttachRight = 0;
            }
            attachRight=QModelIndex();
        }
    }

    //find new attach zones if needed
    if(!attachLeft.isValid() || !attachRight.isValid()) {
        while( (idx = objIndex.child(i,0)).isValid() ) {
            if(idx.data(UserRoles::objInfo).isValid()) {
                ObjectInfo info = idx.data(UserRoles::objInfo).value<ObjectInfo>();

                if(info.nodeType == NodeType::object) {

                    //check attach left
                    if(!attachLeft.isValid()) {
                        QPointF p = idx.data(UserRoles::position).toPointF();
                        p.rx()-=10;
                        QRectF attachZone(p,attachZoneSize);

                        //new attachLeft
                        if(attachZone.contains(pt)) {
                            attachLeft=idx;
                            if(rectAttachLeft) {
                                delete rectAttachLeft;
                                rectAttachLeft = 0;
                            }
                            rectAttachLeft = new QGraphicsRectItem(attachZone,parentItem(),scene());
                            rectAttachLeft->setBrush(QColor(255,255,0,128));
                        }
                    }

                    //check attach right
                    if(!attachRight.isValid()) {
                        QPointF p = idx.data(UserRoles::position).toPointF();
                        p.rx()+=105;
                        QRectF attachZone(p,attachZoneSize);

                        //new attachRight
                        if(attachZone.contains(pt)) {
                            attachRight=idx;
                            if(rectAttachRight) {
                                delete rectAttachRight;
                                rectAttachRight = 0;
                            }
                            rectAttachRight = new QGraphicsRectItem(attachZone,parentItem(),scene());
                            rectAttachRight->setBrush(QColor(255,255,0,128));
                        }
                    }
                }
            }
            ++i;
        }
    }
}

void ContainerContent::dragLeaveEvent( QGraphicsSceneDragDropEvent *event)
{
    HighlightStop();
}

void ContainerContent::dropEvent( QGraphicsSceneDragDropEvent *event)
{
    HighlightStop();
    QGraphicsWidget::dropEvent(event);
    dropPos = mapToScene(event->pos());
    event->setAccepted(model->dropMimeData(event->mimeData(), event->proposedAction(), 0, 0, objIndex));
}

void ContainerContent::UpdateColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color)
{
    if(groupId!=ColorGroups::Panel)
        return;

    switch(colorId) {
        case Colors::Background : {
            QPalette pal(palette());
            pal.setColor(QPalette::Window,color);
            setPalette( pal );
            break;
        }
        default:
            break;
    }
}

void ContainerContent::HighlightStart()
{
    QPalette pal(palette());
    pal.setColor(QPalette::Window, config->GetColor(ColorGroups::Panel,Colors::HighlightBackground) );
    setPalette( pal );
}

void ContainerContent::HighlightStop()
{
    QPalette pal(palette());
    pal.setColor(QPalette::Window, config->GetColor(ColorGroups::Panel,Colors::Background) );
    setPalette( pal );
}
