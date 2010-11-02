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

#include "listmidiinterfacesmodel.h"
#include "../connectables/objectinfo.h"

ListMidiInterfacesModel::ListMidiInterfacesModel(QObject *parent):
        QStandardItemModel(parent)
{
}

QMimeData  * ListMidiInterfacesModel::mimeData ( const QModelIndexList  & indexes ) const
{
    QMimeData  *data = new QMimeData();//QStandardItemModel::mimeData ( indexes ) ;
    QStandardItem *item = itemFromIndex(indexes.first());

    //don't drag api
//    if(item->parent()==0)
//        return 0;

    QByteArray b;
    QDataStream stream(&b,QIODevice::WriteOnly);

//    stream << item->text();
    stream << item->data(UserRoles::objInfo).value<ObjectInfo>();
    //stream << item->data(Qt::UserRole+2).toInt();
    //stream << item->data(Qt::UserRole+3).toInt();

    data->setData("application/x-midiinterface",b);

    return data;
}
