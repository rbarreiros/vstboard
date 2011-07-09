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

#include "maingraphicsview.h"
#include "../globals.h"

//using namespace View;

MainGraphicsView::MainGraphicsView(QWidget * parent) :
    QGraphicsView(parent),
    currentProgId(0)
{
//    MainConfig::Get()->ListenToAction("sceneZoomIn",this);
//    MainConfig::Get()->ListenToAction("sceneZoomOut",this);
//    MainConfig::Get()->ListenToAction("sceneDrag",this);
}

MainGraphicsView::MainGraphicsView(QGraphicsScene * scene, QWidget * parent) :
    QGraphicsView(scene, parent)
{

}

void MainGraphicsView::ForceResize()
{
    emit viewResized( mapToScene(viewport()->rect()).boundingRect() );
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

void MainGraphicsView::mousePressEvent ( QMouseEvent * event )
{
    if(event->modifiers() == Qt::ControlModifier
       && event->button() == Qt::MidButton) {
        event->accept();
        zoomReset();
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

//bool MainGraphicsView::event(QEvent *event)
//{
//    if(event->type() == Event::Shortcut) {
//        ShortcutEvent *e = static_cast<ShortcutEvent*>(event);
//        if(e->value == true) {
//            switch(e->id) {
//                case 0:
//                    zoomIn();
//                    break;
//                case 1:
//                    zoomOut();
//                    break;
//                default :
//                    break;
//            }
//        }
//        return true;
//    }
//    return QGraphicsView::event(event);
//}

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

void MainGraphicsView::paintEvent ( QPaintEvent * event )
{
    emit viewResized( mapToScene(viewport()->rect()).boundingRect() );
    QGraphicsView::paintEvent(event);
}

void MainGraphicsView::scrollContentsBy ( int dx, int dy )
{
    QGraphicsView::scrollContentsBy(dx,dy);
    emit viewResized( mapToScene(viewport()->rect()).boundingRect() );
}

void MainGraphicsView::resizeEvent ( QResizeEvent * event )
{
    QGraphicsView::resizeEvent(event);
//    emit viewResized( mapToScene(viewport()->rect()).boundingRect() );
}

void MainGraphicsView::SaveProgram()
{
    SceneProg state;
    state.scale=transform().m11();
    state.scrollx=horizontalScrollBar()->value();
    state.scrolly=verticalScrollBar()->value();
    listPrograms.insert(currentProgId,state);
}


void MainGraphicsView::SetViewProgram(const QModelIndex &prg)
{
    SetViewProgram(prg.data(UserRoles::value).toInt());
}

void MainGraphicsView::SetViewProgram(int progId)
{
    SaveProgram();

    currentProgId = progId;
    if(!listPrograms.contains(progId)) {
        setTransform( QTransform() );
        horizontalScrollBar()->setValue( 0 );
        verticalScrollBar()->setValue( 0 );
        SaveProgram();
        return;
    }

    SceneProg state = listPrograms.value(currentProgId);
    setTransform( QTransform() );
    scale( state.scale, state.scale );
    horizontalScrollBar()->setValue( state.scrollx );
    verticalScrollBar()->setValue( state.scrolly );
}

void MainGraphicsView::CopyViewProgram(int ori, int dest)
{
    if(!listPrograms.contains(ori))
        return;
    listPrograms.insert(dest, listPrograms.value(ori));
}

void MainGraphicsView::RemoveViewProgram(int prg)
{
    if(!listPrograms.contains(prg))
        return;
    listPrograms.remove(prg);
}

void MainGraphicsView::ClearViewPrograms()
{
    SetViewProgram(EMPTY_PROGRAM);
    listPrograms.clear();
}

QDataStream & MainGraphicsView::toStream (QDataStream& out) const
{
    out << (quint16)listPrograms.size();
    QMap<qint16,SceneProg>::ConstIterator i = listPrograms.constBegin();
    while(i!=listPrograms.constEnd()) {
        qint16 prog = i.key();
        out << prog;
        SceneProg state = i.value();
        out << state.scale;
        out << state.scrollx;
        out << state.scrolly;
        ++i;
    }
    return out;
}

QDataStream & MainGraphicsView::fromStream (QDataStream& in)
{
    ClearViewPrograms();

    quint16 nbProg;
    in >> nbProg;

    for(quint16 i=0; i<nbProg; i++) {
        qint16 key;
        in >> key;
        SceneProg state;
        in >> state.scale;
        in >> state.scrollx;
        in >> state.scrolly;
        listPrograms.insert(key,state);
    }
    return in;
}

QDataStream & operator<< (QDataStream & out, const MainGraphicsView& value)
{
    return value.toStream(out);
}

QDataStream & operator>> (QDataStream & in, MainGraphicsView& value)
{
    return value.fromStream(in);
}
