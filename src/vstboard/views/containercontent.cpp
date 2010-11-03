/******************************************************************************
#    Copyright 2010 Rapha�l Fran�ois
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

#include "containercontent.h"
#include "objectview.h"

#ifdef _MSC_VER
#pragma warning( disable: 4100 )
#endif

using namespace View;

ContainerContent::ContainerContent(QAbstractItemModel *model, QGraphicsItem * parent, Qt::WindowFlags wFlags ) :
    QGraphicsWidget(parent,wFlags),
    model(model)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setGeometry(0,0,50,50);

    QPalette pal;
    pal = palette();
    pal.setBrush(QPalette::Active, QPalette::Window, Qt::lightGray);
    setPalette(pal);
}

void ContainerContent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(palette().brush(QPalette::Window));
    painter->drawRect(boundingRect());
}

void ContainerContent::SetModelIndex(QPersistentModelIndex index)
{
    objIndex = index;
}

QPointF ContainerContent::GetDropPos()
{
    QPointF p = dropPos;
    dropPos.setX(0);
    dropPos.setY(0);
    return p;
}

void ContainerContent::dragEnterEvent( QGraphicsSceneDragDropEvent *event)
{
    QGraphicsWidget::dragEnterEvent(event);

#ifdef VSTSDK
    //accept DLL files
    if (event->mimeData()->hasUrls()) {
        QString fName;
        QFileInfo info;

        foreach(QUrl url,event->mimeData()->urls()) {
            fName = url.toLocalFile();
            info.setFile( fName );
            if ( info.isFile() && info.isReadable() && info.suffix()=="dll" ) {
                event->accept();
            }
        }
        return;
    }
#endif

    //accept Audio interface
    //accept Midi interface
    //accept Tools
    if(event->mimeData()->hasFormat("application/x-audiointerface") ||
       event->mimeData()->hasFormat("application/x-midiinterface") ||
       event->mimeData()->hasFormat("application/x-tools")) {
        event->accept();
        return;
    }

    event->ignore();
}

void ContainerContent::dropEvent( QGraphicsSceneDragDropEvent *event)
{
    QGraphicsWidget::dropEvent(event);
    dropPos = mapToScene(event->pos());
    event->setAccepted(model->dropMimeData(event->mimeData(), event->proposedAction(), 0, 0, objIndex));
}
