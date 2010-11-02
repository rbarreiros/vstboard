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

#include "hostmodel.h"
#include "globals.h"
#include "connectables/objectfactory.h"
#include "mainhost.h"
#include "connectables/objectinfo.h"

HostModel::HostModel(QObject *parent) :
        QStandardItemModel(parent)
{

}

QMimeData * HostModel::mimeData ( const QModelIndexList & indexes ) const
{
    QMimeData *mimeData = new QMimeData;

    foreach(QModelIndex index, indexes) {
        ObjectInfo info = index.data(UserRoles::objInfo).value<ObjectInfo>();
//        int nodeType = index.sibling(index.row(),0).data(UserRoles::nodeType).toInt();
        switch(info.nodeType) {
            case NodeType::pin :
                if(index.data(UserRoles::connectionInfo).isValid()) {
                    ConnectionInfo connectInfo = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();

                    QByteArray bytes;
                    QDataStream stream(&bytes,QIODevice::WriteOnly);
                    stream << connectInfo;
//                    stream << connectInfo.container;
//                    stream << connectInfo.objId;
//                    stream << connectInfo.type;
//                    stream << connectInfo.direction;
//                    stream << connectInfo.pinNumber;
//                    stream << connectInfo.bridge;

                    mimeData->setData("application/x-pin",bytes);
                }
                return mimeData;
                break;
            default:
                return QStandardItemModel::mimeData(indexes);

        }
    }

    return QStandardItemModel::mimeData(indexes);
//    return mimeData;
}

//Qt::DropActions HostModel::supportedDropActions () const
//{

//}

QStringList HostModel::mimeTypes () const
{
    QStringList list;
    list << "text/uri-list"
        << "application/x-audiointerface"
        << "application/x-midiinterface"
        << "application/x-tools"
        << "application/x-pin"
        <<  QLatin1String("application/x-qstandarditemmodeldatalist");

    return list;
}

bool HostModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
{
//    if(QStandardItemModel::dropMimeData(data,action,row,column,parent))
//        return true;

//    if(index.data(UserRoles::parking).toBool()) {
//        QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->GetObj(index);
//        QSharedPointer<Connectables::Object> cntPtr = Connectables::ObjectFactory::Get()->GetObj(parent);
//        if(!cntPtr.isNull() && !objPtr.isNull()) {
//            static_cast<Connectables::Container*>(cntPtr.data())->AddObject(objPtr);
//            return;
//        }

//    }

    QModelIndex rootIndex = parent.sibling(parent.row(),0);

   // debug(QString("hostmodel: drop obj:%1 root:%2 row:%3 col:%4").arg(parent.data(Qt::DisplayRole).toInt()).arg(rootIndex.data(Qt::DisplayRole).toInt()).arg(row).arg(column).toAscii())

    QSharedPointer<Connectables::Container> cntPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(parent.data(UserRoles::value).toInt()).staticCast<Connectables::Container>();
    if(cntPtr.isNull()) {
        debug(QString("HostModel::dropMimeData container not found").toAscii())
        return false;
    }

    ObjectInfo info = rootIndex.data(UserRoles::objInfo).value<ObjectInfo>();

    switch(info.nodeType) {

        case NodeType::container :
        {
            if(data->hasFormat("application/x-qstandarditemmodeldatalist")) {
                QStandardItemModel mod;
                mod.dropMimeData(data,action,0,0,QModelIndex());
                QStandardItem *i = mod.invisibleRootItem()->child(0);
                if(i->data(UserRoles::value).isValid()) {
                    QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->GetObjectFromId( i->data(UserRoles::value).toInt() );
                    if(objPtr.isNull()) {
                        debug("HostModel::dropMimeData x-qstandarditemmodeldatalist object not found")
                        return false;
                    }

                    //parking object are only accepted in programmable objects
                    if(i->data(UserRoles::parking).toBool()) {
                        if(cntPtr->listenProgramChanges) {
                            cntPtr->AddObject(objPtr);
                            return true;
                        } else {
                            return false;
                        }
                    } else {
                        cntPtr->AddObject(objPtr);
                        return true;
                    }
                }
                return false;
            }

    #ifdef VSTSDK
            //vst plugin
            if (data->hasUrls()) {
                QString fName;
                QFileInfo info;

                foreach(QUrl url,data->urls()) {
                    fName = url.toLocalFile();
                    info.setFile( fName );
                    if ( info.isFile() && info.isReadable() && info.suffix()=="dll" ) {

                        ObjectInfo infoVst;
                        infoVst.nodeType = NodeType::object;
                        infoVst.objType = ObjType::VstPlugin;
                        infoVst.filename = fName;
                        infoVst.name = fName;

                        QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->NewObject(infoVst);
                        if(objPtr.isNull()) {
                            debug("HostModel::dropMimeData vstplugin object not found")
                            return false;
                        }
                        cntPtr->AddObject(objPtr);
                    }
                }
                return true;
            }
    #endif

            //audio interface
            if(data->hasFormat("application/x-audiointerface")) {
                QByteArray b = data->data("application/x-audiointerface");
                QDataStream stream(&b,QIODevice::ReadOnly);
                ObjectInfo info;
                stream >> info;

                info.objType=ObjType::AudioInterfaceIn;
                QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->NewObject(info);
                if(objPtr.isNull()) {
                    debug("HostModel::dropMimeData audioin object not found or already used")
                } else {
                    cntPtr->AddObject(objPtr);
                }

                info.objType=ObjType::AudioInterfaceOut;
                objPtr = Connectables::ObjectFactory::Get()->NewObject(info);
                if(objPtr.isNull()) {
                    debug("HostModel::dropMimeData audioout object not found or already used")
                } else {
                    cntPtr->AddObject(objPtr);
                }

                return true;
            }

            //midi interface
            if(data->hasFormat("application/x-midiinterface")) {
                QByteArray b = data->data("application/x-midiinterface");
                QDataStream stream(&b,QIODevice::ReadOnly);
                ObjectInfo info;
                stream >> info;

                QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->NewObject(info);
                if(objPtr.isNull()) {
                    debug("HostModel::dropMimeData midi object not found or already used")
                    return false;
                }
                cntPtr->AddObject(objPtr);

                return true;
            }

            //tools
            if(data->hasFormat("application/x-tools")) {
                QByteArray b = data->data("application/x-tools");
                QDataStream stream(&b,QIODevice::ReadOnly);
                ObjectInfo info;
                stream >> info;

                QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->NewObject(info);
                if(objPtr.isNull()) {
                    debug("HostModel::dropMimeData tool object not found")
                    return false;
                }
                cntPtr->AddObject(objPtr);
                if(info.objType == ObjType::Container) {
                    ObjectInfo in;
                    in.nodeType = NodeType::bridge;
                    in.objType = ObjType::BridgeIn;
                    static_cast<Connectables::Container*>(objPtr.data())->AddObject( Connectables::ObjectFactory::Get()->NewObject(in) );
                    ObjectInfo out;
                    out.nodeType = NodeType::bridge;
                    out.objType = ObjType::BridgeOut;
                    static_cast<Connectables::Container*>(objPtr.data())->AddObject( Connectables::ObjectFactory::Get()->NewObject(out) );
                }


                return true;
            }
            break;
        }

        //connect a pin by drag&drop
        case NodeType::pin :
        {


            if(data->hasFormat("application/x-pin")) {
                ConnectionInfo parentInfo = rootIndex.data(UserRoles::connectionInfo).value<ConnectionInfo>();

                //                  pin   . listpin .obj    . container
                QModelIndex index = rootIndex.parent().parent().parent();
                if(parentInfo.bridge)
                    index=index.parent();

                int cntId = index.data(Qt::DisplayRole).toInt();;
                QSharedPointer<Connectables::Object> cntPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(cntId);
                if(cntPtr.isNull()) {
                    debug(QString("HostModel::dropMimeData NodeType::pin the container %1 is deleted").arg(cntId).toAscii())
                    return false;
                }
                Connectables::Container *cnt = static_cast<Connectables::Container*>(cntPtr.data());


                QByteArray bytes = data->data("application/x-pin");
                ConnectionInfo droppedInfo;

                QDataStream stream(&bytes,QIODevice::ReadOnly);
                stream >> droppedInfo;
//                stream >> droppedInfo.container;
//                stream >> droppedInfo.objId;
//                stream >> droppedInfo.type;
//                stream >> droppedInfo.direction;
//                stream >> droppedInfo.pinNumber;
//                stream >> droppedInfo.bridge;

                if(droppedInfo.CanConnectTo(parentInfo)) {
                    if(droppedInfo.direction == PinDirection::Output)
                        cnt->AddCable(droppedInfo, parentInfo);
                     else
                        cnt->AddCable(parentInfo, droppedInfo);
                     return true;
                 }
            }
            break;
        }
        default:
            return false;


    }

    return false;
}

bool HostModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    QStandardItem *item = itemFromIndex(index);
//    int nodeType = index.data(UserRoles::nodeType).toInt();
    ObjectInfo info = index.data(UserRoles::objInfo).value<ObjectInfo>();

//    debug(QString("HostModel::setData %1:%2").arg(nodeType).arg(value.toString()).toAscii())

    switch(info.nodeType) {
        case NodeType::object :
        case NodeType::container :
        {
            int objId = index.data(UserRoles::value).toInt();
            if(!objId) {
                debug("HostModel::setData NodeType::object has no object Id")
                return false;
            }
            QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(objId);
            if(objPtr.isNull()) {
                debug(QString("HostModel::setData NodeType::object the object %1 is deleted").arg(objId).toAscii())
                return false;
            }

            if(role==UserRoles::position)
                objPtr->position = value.toPointF();
            if(role==UserRoles::size)
                objPtr->size = value.toSizeF();

            break;
        }

        case NodeType::editor :
        {
            int objId = index.parent().data(UserRoles::value).toInt();
            if(!objId) {
                debug("HostModel::setData NodeType::editor has no object Id")
                return false;
            }

            QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(objId);
            if(objPtr.isNull()) {
                debug(QString("HostModel::setData NodeType::editor the object %1 is deleted").arg(objId).toAscii())
                return false;
            }

            bool newVal = value.toBool();

            if(newVal)
                newVal = objPtr->OpenEditor();
            else
                objPtr->CloseEditor();


       //     item->setData(newVal,role);
      //      setData(index,newVal,role);

            break;
        }

        case NodeType::pin :
        {
            ConnectionInfo info = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();
            if(role==UserRoles::value) {
                bool ok=true;
                float newVal = value.toFloat(&ok);// item->data(Qt::DisplayRole).toFloat(&ok);
                if(!ok)
                    return false;

                if(newVal>1.0f) newVal=1.0f;
                if(newVal<.0f) newVal=.0f;
                static_cast<Connectables::ParameterPin*>(Connectables::ObjectFactory::Get()->GetPin(info))->ChangeValue( newVal );
                item->setData(newVal,role);
            }
            break;
        }
        default:
            return false;
    }

    return false;
}

bool HostModel::setItemData ( const QModelIndex & index, const QMap<int, QVariant> & roles )
{
    return true;
}

//bool HostModel::removeRows ( int row, int count, const QModelIndex & parent )
//{
//    for(int i=row;i<row+count;i++) {
//        QModelIndex index = parent.child(row,0);

//        QStandardItem *item = itemFromIndex(parent);
//        item->removeRow(i);
//        QStandardItemModel::removeRows(row,count,parent);
//        debug(QString("HostModel::removeRows row:%1 count:%2 nodetype:%3 objid:%4").arg(row).arg(i).arg(index.data(UserRoles::nodeType).toInt()).arg(index.data(Qt::DisplayRole).toString()).toAscii())

//        switch(index.data(UserRoles::nodeType).toInt()) {
//            case NodeType::container :
//            case NodeType::object :
//            {
//                int cntId = parent.data(UserRoles::value).toInt();
//                int objId = index.data(UserRoles::value).toInt();
//                QSharedPointer<Connectables::Object> cntPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(cntId);
//                if(cntPtr.isNull()) {
//                    debug(QString("HostModel::removeRows NodeType::object the container %1 is deleted").arg(cntId).toAscii())
//                    return false;
//                }
//                Connectables::Container *cnt = static_cast<Connectables::Container*>(cntPtr.data());
//                QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(objId);
//                if(objPtr.isNull()) {
//                    debug(QString("HostModel::removeRows NodeType::object the object %1 is deleted").arg(objId).toAscii())
//                    return false;
//                }
//                cnt->RemoveChildObject(objPtr);
//                break;
//            }
//            case NodeType::pin :
//                switch(parent.data(UserRoles::nodeType).toInt()) {
//                    case NodeType::listParamIn:
//                    case NodeType::listParamOut:
//                    {
//                        ConnectionInfo info = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();
//                        Connectables::ParameterPin *pin = static_cast<Connectables::ParameterPin*>(Connectables::ObjectFactory::Get()->GetPin(info));
//                        if(pin)
//                            pin->SetVisible(false);
//                        break;
//                    }
//                }
//                break;

//            case NodeType::cable :
//            {
//                    int cntId = parent.parent().data(UserRoles::value).toInt();
//                ConnectionInfo outInfo = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();
//                ConnectionInfo inInfo = parent.child(row,1).data(UserRoles::connectionInfo).value<ConnectionInfo>();
//                QSharedPointer<Connectables::Object> cntPtr = Connectables::ObjectFactory::Get()->GetObjectFromId(cntId);
//                if(cntPtr.isNull()) {
//                    debug(QString("HostModel::removeRows NodeType::cable the container %1 is deleted").arg(cntId).toAscii())
//                    return false;
//                }

//                static_cast<Connectables::Container*>(cntPtr.data())->RemoveCable(outInfo,inInfo);
//                break;
//            }
//        }
//    }
//    return true;
//}
