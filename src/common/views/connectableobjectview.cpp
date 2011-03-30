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

#include "connectableobjectview.h"
#include "connectablepinview.h"
#include "../connectables/objectfactory.h"
#include "../connectables/container.h"
#include "mainwindow.h"

#ifdef VSTSDK
    #include "../connectables/vstplugin.h"
    #include "vstpluginwindow.h"
#endif

using namespace View;

ConnectableObjectView::ConnectableObjectView(MainHost *myHost,QAbstractItemModel *model,QGraphicsItem * parent, Qt::WindowFlags wFlags ) :
    ObjectView(myHost,model,parent,wFlags)
{
    setGeometry(QRectF(0,0,105,15));
    border = new QGraphicsRectItem(geometry(),this,scene());

    titleText = new QGraphicsSimpleTextItem(QString("Title"),this);
    titleText->moveBy(2,1);

    layout = new QGraphicsGridLayout() ;
    layout->setSpacing(0);
    layout->setHorizontalSpacing(5);
    layout->setContentsMargins(0,15,0,0);
    setLayout(layout);

    setFlag(QGraphicsItem::ItemIsMovable, true);
//    setFlag(QGraphicsItem::ItemIsSelectable, true);

    listAudioIn = new ListPinsView(this);
    listAudioOut = new ListPinsView(this);
    listMidiIn = new ListPinsView(this);
    listMidiOut = new ListPinsView(this);
    listParametersIn = new ListPinsView(this);
    listParametersOut = new ListPinsView(this);

    layout->addItem(listAudioIn,0,0,Qt::AlignLeft | Qt::AlignTop);
    layout->addItem(listAudioOut,0,1,Qt::AlignRight | Qt::AlignTop);
    layout->addItem(listMidiIn,1,0,Qt::AlignLeft | Qt::AlignTop);
    layout->addItem(listMidiOut,1,1,Qt::AlignRight | Qt::AlignTop);
    layout->addItem(listParametersIn,2,0,Qt::AlignLeft | Qt::AlignTop);
    layout->addItem(listParametersOut,2,1,Qt::AlignRight | Qt::AlignTop);

    setAcceptDrops(true);
   // setCursor(Qt::UpArrowCursor);
}

void ConnectableObjectView::dragEnterEvent( QGraphicsSceneDragDropEvent *event)
{
    //accept fxp files
    if (event->mimeData()->hasUrls()) {
        QString fName;
        QFileInfo info;

        foreach(QUrl url,event->mimeData()->urls()) {
            fName = url.toLocalFile();
            info.setFile( fName );
            if ( info.isFile() && info.isReadable() && (info.suffix()=="fxb" || info.suffix()=="fxp")) {
                event->setDropAction(Qt::CopyAction);
                event->accept();
                return;
            }
        }
    }
    event->ignore();
}

void ConnectableObjectView::dropEvent( QGraphicsSceneDragDropEvent *event)
{
    QGraphicsWidget::dropEvent(event);
    event->setAccepted(model->dropMimeData(event->mimeData(), event->proposedAction(), 0, 0, objIndex));
}
