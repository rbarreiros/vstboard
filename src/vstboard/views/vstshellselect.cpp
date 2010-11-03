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

#include "vstshellselect.h"
#include "ui_vstshellselect.h"
#include "../mainhost.h"
#include "../connectables/objectfactory.h"
#include "../connectables/container.h"
#include "../connectables/vstplugin.h"

using namespace View;

VstShellSelect::VstShellSelect() :
    cntPtr(0),
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
//    int cpt=0;
    QMap<ulong,QString>::iterator i = listPlugins.begin();
    while(i!=listPlugins.end()) {
        QListWidgetItem *item = new QListWidgetItem(i.value(),ui->listPlugins);
        item->setData(Qt::UserRole,(int)i.key());
//        ui->listPlugins->insertItem(cpt, i.value());
//        cpt++;
        ++i;
    }
}

void View::VstShellSelect::on_buttonOk_clicked()
{
    QModelIndex index = ui->listPlugins->currentIndex();
    int id = index.data(Qt::UserRole).toInt();

//    QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(containerId);
//    if(objPtr.isNull())
//        return;
//    Connectables::Container* cnt = static_cast<Connectables::Container*>(objPtr.data());
//    cnt->AddChildObject(ObjType::VstPlugin, id,vstDll);

    ObjectInfo info;
    info.nodeType = NodeType::object;
    info.objType = ObjType::VstPlugin;
    info.filename = vstDll;
    info.id = id;

//    QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->NewObject(ObjType::VstPlugin, id, vstDll);
    QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->NewObject(info);
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

//    MainHost::Get()->parkingContainer->AddObject(objPtr);

    //MainHost::Get()->parkingContainer->AddObject( Connectables::ObjectFactory::Get()->NewObject(ObjType::VstPlugin, id, vstDll) );
    close();
}

void View::VstShellSelect::on_buttonCancel_clicked()
{
    close();
}

//void View::VstShellSelect::on_listPlugins_doubleClicked(QModelIndex index)
//{
//    int id = index.data(Qt::UserRole).toInt();

//    QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(containerId);
//    if(objPtr.isNull())
//        return;

//    Connectables::Container* cnt = static_cast<Connectables::Container*>(objPtr.data());
//    cnt->AddChildObject(ObjType::VstPlugin, id,vstDll);
//    close();
//}

