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

#include "vstshellselect.h"
#include "ui_vstshellselect.h"
#include "mainhost.h"
#include "connectables/vstplugin.h"
#include "commands/comaddobject.h"
using namespace View;

ComAddObject* VstShellSelect::command=0;

VstShellSelect::VstShellSelect(MainHost *myHost, Connectables::VstPlugin *plugin) :
    myHost(myHost),
    ui(new Ui::VstShellSelect)
{
    ui->setupUi(this);
    setAttribute( Qt::WA_DeleteOnClose );

    QList<ulong> listPlugins;
    char szName[64];
    ulong id;
    while ((id = plugin->EffGetNextShellPlugin(szName))) {
        if(listPlugins.contains(id))
            continue;
        listPlugins << id;
        QListWidgetItem *item = new QListWidgetItem(szName,ui->listPlugins);
        item->setData(Qt::UserRole,(int)id);
    }

    info = plugin->info();
    info.forcedObjId = 0;
}

VstShellSelect::~VstShellSelect()
{
    Connectables::VstPlugin::shellSelectView=0;
    delete ui;
}

void View::VstShellSelect::on_buttonOk_clicked()
{
    info.id = ui->listPlugins->currentIndex().data(Qt::UserRole).toInt();
    command->ReloadObject(info);
    close();
}

void View::VstShellSelect::on_buttonCancel_clicked()
{
    myHost->undoStack.undo();
    close();
}

