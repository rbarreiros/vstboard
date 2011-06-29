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
#include "heap.h"
#ifndef QT_NO_DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

#include "objectdropzone.h"
#include "globals.h"

using namespace View;

ObjectDropZone::ObjectDropZone(QGraphicsItem *parent, QWidget *parking) :
    QGraphicsWidget(parent),
    myParking(parking)
{
    setAcceptDrops(true);
}

void ObjectDropZone::dragEnterEvent( QGraphicsSceneDragDropEvent *event)
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

void ObjectDropZone::dragMoveEvent( QGraphicsSceneDragDropEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier)
        event->setDropAction(Qt::CopyAction);
    else
        event->setDropAction(Qt::MoveAction);

    QGraphicsWidget::dragMoveEvent(event);
}

void ObjectDropZone::dragLeaveEvent( QGraphicsSceneDragDropEvent *event)
{
    HighlightStop();
    QGraphicsWidget::dragLeaveEvent(event);
}

void ObjectDropZone::dropEvent( QGraphicsSceneDragDropEvent *event)
{
    HighlightStop();
    emit(ObjectDropped(event));
    //reste to copy : we don't really want a moveaction
    event->setDropAction(Qt::CopyAction);
    QGraphicsWidget::dropEvent(event);
}

void ObjectDropZone::HighlightStart()
{
    setAutoFillBackground(true);
}

void ObjectDropZone::HighlightStop()
{
    setAutoFillBackground(false);
}

void ObjectDropZone::UpdateHeight()
{
    resize( size().width(), parentWidget()->size().height() );
}
