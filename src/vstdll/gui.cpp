/**************************************************************************
#    Copyright 2010 Rapha�l Fran�ois
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

bool Gui::open(void* ptr)
{
    AEffEditor::open (ptr);
    widget = new QWinWidget(static_cast<HWND>(ptr));
//    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    QHBoxLayout layout(widget);
    layout.setContentsMargins(0,0,0,0);
    win = MainWindow::Create();
    layout.addWidget(win);

//    win->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    widget->move( 0, 0 );
    widget->adjustSize();
    rectangle.top = 0;
    rectangle.left = 0;
    rectangle.bottom = widget->height();
    rectangle.right = widget->width();
    //  win->setMinimumSize(win->size());

    widget->show();
//    win->show();
    clientResize(static_cast<HWND>(ptr), widget->width(), widget->height());

    return true;
}

void Gui::close()
{
    //  disconnect(this, SIGNAL(update(float)), win, SLOT(update(float)));
    //  delete win;
//    win->hide();
}

bool Gui::getRect (ERect** rect)
{
    *rect = &rectangle;
    return true;
}

void clientResize(HWND h_parent, int width, int height)
{
    RECT rcClient, rcWindow;
    POINT ptDiff;
    GetClientRect(h_parent, &rcClient);
    GetWindowRect(h_parent, &rcWindow);
    ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
    ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
    MoveWindow(h_parent, rcWindow.left, rcWindow.top, width + ptDiff.x, height + ptDiff.y, TRUE);
}
