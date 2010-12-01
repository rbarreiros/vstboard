/******************************************************************************
#    Copyright 2010 Raphaël François
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
******************************************************************************/

#include "vstpluginwindow.h"
#include "ui_vstpluginwindow.h"
#include "../connectables/vstplugin.h"
#include "../mainhost.h"

using namespace View;

VstPluginWindow::VstPluginWindow(QWidget *parent) :
    QFrame(parent),
    menuHeight(0),
    ui(new Ui::VstPluginWindow)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
}

VstPluginWindow::~VstPluginWindow()
{
    delete ui;
}

//void VstPluginWindow::changeEvent(QEvent *e)
//{
//    QFrame::changeEvent(e);
//    switch (e->type()) {
//    case QEvent::LanguageChange:
//        ui->retranslateUi(this);
//        break;
//    default:
//        break;
//    }
//}

WId VstPluginWindow::GetWinId()
{
    return ui->scrollAreaWidgetContents->winId();
}

bool VstPluginWindow::SetPlugin(Connectables::VstPlugin *plugin)
{
    bool windowOk = false;
    this->plugin = plugin;
    setWindowFlags(Qt::Tool);

    long res;
    ERect *rect = 0;

    //try to get rect (some plugins wants it before editopen)
    res = plugin->EffEditGetRect(&rect);
    if(res == 1L)
        windowOk=true;

    //try to open the window
    plugin->objMutex.lock();
    res = plugin->EffEditOpen(ui->scrollAreaWidgetContents->winId());
    plugin->objMutex.unlock();

    if(res == 1L)
        windowOk=true;

    //try to get rect again
    res = plugin->EffEditGetRect(&rect);
    if(res == 1L)
        windowOk=true;

    if(!windowOk)
        return false;

    SetWindowSize(rect->right, rect->bottom);
    setWindowTitle(plugin->objectName());

    connect(this,SIGNAL(destroyed()),
            plugin,SLOT(EditorDestroyed()));
    connect(plugin,SIGNAL(WindowSizeChange(int,int)),
            this,SLOT(SetWindowSize(int,int)));
    return true;
}

void VstPluginWindow::SavePosSize()
{
    if(plugin->modelIndex.isValid()) {
        MainHost::GetModel()->setData(plugin->modelIndex, size(), UserRoles::editorSize);
        MainHost::GetModel()->setData(plugin->modelIndex, pos(), UserRoles::editorPos);
        MainHost::GetModel()->setData(plugin->modelIndex, ui->scrollArea->verticalScrollBar()->value(), UserRoles::editorVScroll);
        MainHost::GetModel()->setData(plugin->modelIndex, ui->scrollArea->horizontalScrollBar()->value(), UserRoles::editorHScroll);
    }
}

void VstPluginWindow::closeEvent( QCloseEvent * event )
{
    SavePosSize();

    hide();
//    if(plugin->modelIndex.isValid())
//        MainHost::GetModel()->setData(plugin->modelIndex,false,UserRoles::editorVisible);
    plugin->ToggleEditor(false);

    event->ignore();
}

void VstPluginWindow::SetWindowSize(int newWidth, int newHeight)
{
    ui->scrollAreaWidgetContents->setFixedSize(newWidth,newHeight);
    resize(newWidth,newHeight+menuHeight);
}

void VstPluginWindow::SetScrollValue(int Hscroll, int Vscroll)
{
    ui->scrollArea->horizontalScrollBar()->setValue(Hscroll);
    ui->scrollArea->verticalScrollBar()->setValue(Vscroll);
}

void VstPluginWindow::showEvent ( QShowEvent * event )
{
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    int w = ui->scrollAreaWidgetContents->width();
    int h = ui->scrollAreaWidgetContents->height();
    setMaximumSize(w,h);
    resize(w,h);
}

void VstPluginWindow::resizeEvent ( QResizeEvent * event )
{
    int maxH=ui->scrollAreaWidgetContents->height();
    int maxW=ui->scrollAreaWidgetContents->width();

    if( event->size().width() >= maxW &&
        event->size().height() >= maxH &&
        ui->scrollArea->horizontalScrollBarPolicy()!=Qt::ScrollBarAlwaysOff) {

        ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setMaximumSize(maxW,maxH);
    } else
        if(ui->scrollArea->horizontalScrollBarPolicy()!=Qt::ScrollBarAsNeeded) {

        ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        maxW+=ui->scrollArea->verticalScrollBar()->width();
        maxH+=ui->scrollArea->horizontalScrollBar()->height();
        setMaximumSize(maxW,maxH);
    }
}


//const QPixmap VstPluginWindow::GetScreenshot()
//{
//    QPixmap pix = QPixmap::grabWindow(ui->scrollAreaWidgetContents->winId() ,0,0,size().width(),size().height());//45);
//    QPainter p(&pix);
//    p.fillRect(pix.rect(), QColor(255,255,255,150));
//    return pix;
//}
