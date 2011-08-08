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

#ifndef GUI_H
#define GUI_H


#include <QObject>

#include "aeffeditor.h"
#include "mainwindowvst.h"
#include "resizehandle.h"
#include <qwinwidget.h>

class Gui : public QObject, public AEffEditor
{
    Q_OBJECT
    QWinWidget *widget;
    AudioEffectX* effect;

public:
    Gui(AudioEffectX* effect);
    ~Gui();

    virtual bool open(void* ptr);
    virtual bool getRect (ERect** rect);
    virtual void close();
    void SetMainWindow(MainWindowVst *win);

protected:
    bool hostCanSizeWindow;
    ERect rectangle;
    MainWindowVst *myWindow;
    ResizeHandle *resizeH;
    QPoint widgetOffset;

signals:
    void update(float value);

public slots:
    void UpdateColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
    void OnResizeHandleMove(const QPoint &pt);

};

#endif // GUI_H
