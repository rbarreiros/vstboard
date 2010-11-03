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

ConnectableObjectView::ConnectableObjectView(QAbstractItemModel *model,QGraphicsItem * parent, Qt::WindowFlags wFlags ) :
    ObjectView(model,parent,wFlags)
{
    setGeometry(QRectF(0,0,100,15));
    border = new QGraphicsRectItem(geometry(),this,scene());
    backgroundImg = new QGraphicsPixmapItem(this,scene());
    titleText = new QGraphicsSimpleTextItem(QString("Title"),this);
    titleText->moveBy(2,1);
    editorButton =  new TextButton("Ed",this,Qt::black,QColor(255,255,255));
    editorButton->moveBy(rect().width()-15,0);
    editorButton->hide();

    layout = new QGraphicsGridLayout() ;
    layout->setSpacing(0);
    layout->setContentsMargins(0,15,0,0);
    setLayout(layout);

    setFlag(QGraphicsItem::ItemIsMovable, true);

    listAudioIn = new ListPinsView(this);
    listAudioOut = new ListPinsView(this);
    listMidiIn = new ListPinsView(this);
    listMidiOut = new ListPinsView(this);
    listParametersIn = new ListPinsView(this);
    listParametersOut = new ListPinsView(this);
    listBridgeIn = new ListPinsView(this);
    listBridgeOut = new ListPinsView(this);

    layout->addItem(listAudioIn,0,0,Qt::AlignLeft);
    layout->addItem(listAudioOut,0,1,Qt::AlignRight);
    layout->addItem(listMidiIn,1,0,Qt::AlignLeft);
    layout->addItem(listMidiOut,1,1,Qt::AlignRight);
    layout->addItem(listParametersIn,2,0,Qt::AlignLeft);
    layout->addItem(listParametersOut,2,1,Qt::AlignRight);
    layout->addItem(listBridgeIn,3,0,Qt::AlignLeft);
    layout->addItem(listBridgeOut,3,1,Qt::AlignRight);
}

void ConnectableObjectView::SetBackground()
{
    qreal height = 15
                   + std::max(listAudioIn->size().height(), listAudioOut->size().height())
                   + std::max(listMidiIn->size().height(),  listMidiOut->size().height());
    QPixmap pix = QPixmap::grabWindow(backOri,0,0,size().width(), height);//45);
    QPainter p(&pix);
    p.fillRect(pix.rect(), QColor(255,255,255,150));

    backgroundImg->setPixmap(pix);
    QGraphicsBlurEffect *eff = new QGraphicsBlurEffect(this);
    eff->setBlurRadius(2);
    backgroundImg->setGraphicsEffect(eff);
}



