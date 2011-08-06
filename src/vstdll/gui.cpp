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
#include "gui.h"
#include <QtGui/QHBoxLayout>

Gui::Gui(AudioEffectX* effect) :
    widget(0),
    myWindow(0),
    effect(effect),
    resizeH(0)
{
    rectangle.top = 0;
    rectangle.left = 0;
    rectangle.bottom = 600;
    rectangle.right = 800;

    //reaper needs an offset.. can't find a good solution
    char str[64];
    effect->getHostProductString(str);
    if(!strcmp(str,"REAPER")) {
        widgetOffset.setY(27);
    }
}

Gui::~Gui()
{
    if(myWindow) {
        myWindow->writeSettings();
        myWindow->setParent(0);
    }
    if(widget) {
        delete widget;
        widget=0;
    }
}

void Gui::UpdateColor(ColorGroups::Enum groupId, Colors::Enum /*colorId*/, const QColor & /*color*/)
{
    if(groupId!=ColorGroups::Window)
        return;

    if(widget)
        widget->setPalette( myWindow->palette() );
}

void Gui::SetMainWindow(MainWindowVst *win)
{
    if(win==myWindow)
        return;

    myWindow = win;
    if(!myWindow)
        return;

    connect( myWindow->viewConfig, SIGNAL(ColorChanged(ColorGroups::Enum,Colors::Enum,QColor)),
             this, SLOT(UpdateColor(ColorGroups::Enum,Colors::Enum,QColor)),
             Qt::UniqueConnection);
}

bool Gui::open(void* ptr)
{
    if(!ptr || !myWindow)
        return false;

    AEffEditor::open(ptr);
    widget = new QWinWidget(static_cast<HWND>(ptr));
    widget->setAutoFillBackground(false);
    widget->setObjectName("QWinWidget");

    myWindow->setParent(widget);
    myWindow->readSettings();
    myWindow->move(0,0);

    rectangle.bottom = myWindow->height();
    rectangle.right = myWindow->width();

    widget->move( 0, 0 );
    widget->resize( rectangle.right, rectangle.bottom );
    widget->setPalette( myWindow->palette() );

    resizeH = new ResizeHandle(widget);
    QPoint pos( widget->geometry().bottomRight() );
    pos.rx()-=resizeH->width();
    pos.ry()-=resizeH->height();
    resizeH->move(pos);
    resizeH->show();

    connect(resizeH, SIGNAL(Moved(QPoint)),
            this, SLOT(OnResizeHandleMove(QPoint)));

    widget->show();
    return true;
}



void Gui::OnResizeHandleMove(const QPoint &pt)
{
    widget->resize( pt.x(), pt.y() );
    widget->move(widgetOffset);

    if(myWindow)
        myWindow->resize(pt.x(), pt.y());

    if(effect)
        effect->sizeWindow(pt.x(), pt.y());

    rectangle.right = pt.x();
    rectangle.bottom = pt.y();
}

void Gui::close()
{
    if(myWindow) {
        myWindow->writeSettings();
        myWindow->setParent(0);
    }
    if(widget) {
        delete widget;
        widget=0;
    }
}

bool Gui::getRect (ERect** rect)
{
    if(!widget || !rect)
        return false;

    *rect = &rectangle;
    return true;
}
