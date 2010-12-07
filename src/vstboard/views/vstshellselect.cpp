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

#include "vstshellselect.h"
#include "ui_vstshellselect.h"
//#include "../mainhost.h"
#include "../connectables/objectfactory.h"
#include "../connectables/container.h"
#include "../connectables/vstplugin.h"

using namespace View;

VstShellSelect::VstShellSelect(Connectables::ObjectFactory *objFactory) :
    cntPtr(0),
    objFactory(objFactory),
    ui(new Ui::VstShellSelect)
{
    ui->setupUi(this);
    setAttribute( Qt::WA_DeleteOnClose );
}

VstShellSelect::~VstShellSelect()
{
    Connectables::VstPlugin::shellSelectView=0;
    delete ui;
}

void VstShellSelect::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void VstShellSelect::SetListPlugins(QString file, QMap<ulong,QString> &listPlugins)
{
    vstDll = file;
    QMap<ulong,QString>::iterator i = listPlugins.begin();
    while(i!=listPlugins.end()) {
        QListWidgetItem *item = new QListWidgetItem(i.value(),ui->listPlugins);
        item->setData(Qt::UserRole,(int)i.key());
        ++i;
    }
}

void View::VstShellSelect::on_buttonOk_clicked()
{
    QModelIndex index = ui->listPlugins->currentIndex();
    int id = index.data(Qt::UserRole).toInt();

    ObjectInfo info;
    info.nodeType = NodeType::object;
    info.objType = ObjType::VstPlugin;
    info.filename = vstDll;
    info.id = id;

    QSharedPointer<Connectables::Object> objPtr = objFactory->NewObject(info);
    if(objPtr.isNull()) {
        debug("View::VstShellSelect::on_buttonOk_clicked object not loaded")
        close();
        return;
    }
    if(!cntPtr) {
        debug("View::VstShellSelect::on_buttonOk_clicked container not set")
        close();
        return;
    }

    cntPtr->AddObject(objPtr);
    close();
}

void View::VstShellSelect::on_buttonCancel_clicked()
{
    close();
}

