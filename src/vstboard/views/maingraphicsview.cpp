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

#include "maingraphicsview.h"
#include "../globals.h"
#include "../mainconfig.h"

//using namespace View;

MainGraphicsView::MainGraphicsView(QWidget * parent) :
    QGraphicsView(parent)
{
    MainConfig::Get()->ListenToAction("sceneZoomIn",this);
    MainConfig::Get()->ListenToAction("sceneZoomOut",this);
    MainConfig::Get()->ListenToAction("sceneDrag",this);
}

MainGraphicsView::MainGraphicsView(QGraphicsScene * scene, QWidget * parent) :
    QGraphicsView(scene, parent)
{

}

void MainGraphicsView::wheelEvent(QWheelEvent * event)
{
    if(event->modifiers() == Qt::ControlModifier) {
        event->accept();
        if(event->delta()>0)
            zoomIn();
        else
            zoomOut();
        return;
    }
    QGraphicsView::wheelEvent(event);
}

bool MainGraphicsView::event(QEvent *event)
{
    if(event->type() == Event::Shortcut) {
        ShortcutEvent *e = static_cast<ShortcutEvent*>(event);
        if(e->value == true) {
            switch(e->id) {
                case 0:
                    zoomIn();
                    break;
                case 1:
                    zoomOut();
                    break;
                default :
                    break;
            }
        }
        return true;
    }
    return QGraphicsView::event(event);
}

void MainGraphicsView::zoomIn()
{
    scale(1.2, 1.2);
    emit viewResized( mapToScene(viewport()->rect()).boundingRect() );
}

void MainGraphicsView::zoomOut()
{
    scale(1.0/1.2, 1.0/1.2);
    emit viewResized( mapToScene(viewport()->rect()).boundingRect() );
}

void MainGraphicsView::zoomReset()
{
    setTransform( QTransform() );
    emit viewResized( mapToScene(viewport()->rect()).boundingRect() );
}

void MainGraphicsView::scrollContentsBy ( int dx, int dy )
{
    QGraphicsView::scrollContentsBy(dx,dy);
    emit viewResized( mapToScene(viewport()->rect()).boundingRect() );
}

void MainGraphicsView::resizeEvent ( QResizeEvent * event )
{
    QGraphicsView::resizeEvent(event);
    emit viewResized( mapToScene(viewport()->rect()).boundingRect() );
}

