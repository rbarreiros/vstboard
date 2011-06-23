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

#include "hostmodel.h"
#include "globals.h"
#include "connectables/objectfactory.h"
#include "mainhost.h"
#include "mainwindow.h"
#include "connectables/objectinfo.h"
#include "connectables/vstplugin.h"

HostModel::HostModel(MainHost *parent) :
        QStandardItemModel(parent)
{
    myHost = parent;
}

QMimeData * HostModel::mimeData ( const QModelIndexList & indexes ) const
{
    QMimeData *mimeData = new QMimeData;

    foreach(QModelIndex index, indexes) {
        ObjectInfo info = index.data(UserRoles::objInfo).value<ObjectInfo>();
        switch(info.nodeType) {
            case NodeType::pin :
                if(index.data(UserRoles::connectionInfo).isValid()) {
                    ConnectionInfo connectInfo = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();

                    QByteArray bytes;
                    QDataStream stream(&bytes,QIODevice::WriteOnly);
                    stream << connectInfo;
                    mimeData->setData("application/x-pin",bytes);
                }
                return mimeData;
                break;
            default:
                return QStandardItemModel::mimeData(indexes);

        }
    }

    return QStandardItemModel::mimeData(indexes);
}
/*
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
*/
bool HostModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
{

    QModelIndex index = parent.sibling(parent.row(),0);
    if(!index.isValid()) {
        debug("HostModel::dropMimeData rootIndex not valid")
        return false;
    }
    ObjectInfo info = index.data(UserRoles::objInfo).value<ObjectInfo>();

    switch(info.nodeType) {

        case NodeType::container :
        {
            QSharedPointer<Connectables::Container> cntPtr;
            if(parent.isValid())
                cntPtr = myHost->objFactory->GetObj(index).staticCast<Connectables::Container>();
            if(cntPtr.isNull()) {
                debug(QString("HostModel::dropMimeData container not found").toAscii())
                return false;
            }

            if(data->hasFormat("application/x-qstandarditemmodeldatalist")) {
                QStandardItemModel mod;
                mod.dropMimeData(data,action,0,0,QModelIndex());
                int a=mod.rowCount();
                for(int i=0;i<a;i++) {
                    QStandardItem *it = mod.invisibleRootItem()->child(i);
                    if(it->data(UserRoles::value).isValid()) {
                        QSharedPointer<Connectables::Object> objPtr = myHost->objFactory->GetObjectFromId( it->data(UserRoles::value).toInt() );
                        if(objPtr.isNull()) {
                            debug("HostModel::dropMimeData x-qstandarditemmodeldatalist object not found")
                            continue;
                        }
                        cntPtr->UserAddObject(objPtr);
                    }
                }
                return true;
            }


            if (data->hasUrls()) {

                foreach(QUrl url,data->urls()) {
                    QString fName = url.toLocalFile();
                    QFileInfo info;
                    info.setFile( fName );

                    if ( info.isFile() && info.isReadable() ) {
#ifdef VSTSDK
                        //vst plugin
                        if ( info.suffix()=="dll" ) {

                            ObjectInfo infoVst;
                            infoVst.nodeType = NodeType::object;
                            infoVst.objType = ObjType::VstPlugin;
                            infoVst.filename = fName;
                            infoVst.name = fName;

                            QSharedPointer<Connectables::Object> objPtr = myHost->objFactory->NewObject(infoVst);
                            if(objPtr.isNull()) {
                                if(Connectables::VstPlugin::shellSelectView) {
                                    Connectables::VstPlugin::shellSelectView->cntPtr=cntPtr;
                                } else {
                                    debug("HostModel::dropMimeData vstplugin object not found")
                                }
                                return false;
                            }
                            cntPtr->UserAddObject(objPtr);
                        }
#endif
                        //setup file
                        if ( info.suffix()==SETUP_FILE_EXTENSION ) {
                            if(myHost->mainWindow->userWantsToUnloadSetup()) {
                                //load on the next loop : we have to get out of the container before loading files
                                QSignalMapper *mapper = new QSignalMapper(this);
                                QTimer *t1 = new QTimer(this);
                                t1->setSingleShot(true);
                                connect(t1, SIGNAL(timeout()), mapper, SLOT(map()));
                                connect(mapper, SIGNAL(mapped(QString)), myHost, SLOT(LoadSetupFile(QString)));
                                mapper->setMapping(t1, fName);
                                t1->start(0);
                            }
                        }

                        //project file
                        if ( info.suffix()==PROJECT_FILE_EXTENSION ) {
                            if(myHost->mainWindow->userWantsToUnloadProject()) {
                                //load on the next loop : we have to get out of the container before loading files
                                QSignalMapper *mapper = new QSignalMapper(this);
                                QTimer *t1 = new QTimer(this);
                                t1->setSingleShot(true);
                                connect(t1, SIGNAL(timeout()), mapper, SLOT(map()));
                                connect(mapper, SIGNAL(mapped(QString)), myHost, SLOT(LoadProjectFile(QString)));
                                mapper->setMapping(t1, fName);
                                t1->start(0);
                            }
                        }
                    }
                }
                return true;
            }


            //audio interface
            if(data->hasFormat("application/x-audiointerface")) {
                QByteArray b = data->data("application/x-audiointerface");
                QDataStream stream(&b,QIODevice::ReadOnly);

                while(!stream.atEnd()) {
                    ObjectInfo info;
                    stream >> info;

                    if(info.inputs!=0) {
                        info.objType=ObjType::AudioInterfaceIn;
                        QSharedPointer<Connectables::Object> objPtr = myHost->objFactory->NewObject(info);
                        if(objPtr.isNull()) {
                            debug("HostModel::dropMimeData audioin object not found or already used")
                        } else {
                            cntPtr->UserAddObject(objPtr);
                        }
                    }

                    if(info.outputs!=0) {
                        info.objType=ObjType::AudioInterfaceOut;
                        QSharedPointer<Connectables::Object> objPtr = myHost->objFactory->NewObject(info);
                        if(objPtr.isNull()) {
                            debug("HostModel::dropMimeData audioout object not found or already used")
                        } else {
                            cntPtr->UserAddObject(objPtr);
                        }
                    }
                }

                return true;
            }

            //midi interface
            if(data->hasFormat("application/x-midiinterface")) {
                QByteArray b = data->data("application/x-midiinterface");
                QDataStream stream(&b,QIODevice::ReadOnly);

                while(!stream.atEnd()) {
                    ObjectInfo info;
                    stream >> info;
                    QSharedPointer<Connectables::Object> objPtr = myHost->objFactory->NewObject(info);
                    if(objPtr.isNull()) {
                        debug("HostModel::dropMimeData midi object not found or already used")
                        continue;
                    }
                    cntPtr->UserAddObject(objPtr);
                }
                return true;
            }

            //tools
            if(data->hasFormat("application/x-tools")) {
                QByteArray b = data->data("application/x-tools");
                QDataStream stream(&b,QIODevice::ReadOnly);

                while(!stream.atEnd()) {
                    ObjectInfo info;
                    stream >> info;
                    QSharedPointer<Connectables::Object> objPtr = myHost->objFactory->NewObject(info);
                    if(objPtr.isNull()) {
                        debug("HostModel::dropMimeData tool object not found")
                        continue;
                    }
                    cntPtr->UserAddObject(objPtr);
                }
                return true;
            }
            break;
        }

        //drop fxp and fxb file
        case NodeType::object :
        {
            if(info.objType == ObjType::VstPlugin) {
                QSharedPointer<Connectables::VstPlugin> vstPtr;
                if(parent.isValid())
                    vstPtr = myHost->objFactory->GetObj(index).staticCast<Connectables::VstPlugin>();
                if(vstPtr.isNull()) {
                    debug(QString("HostModel::dropMimeData vstplugin not found").toAscii())
                    return false;
                }

                if (data->hasUrls()) {
                    foreach(QUrl url,data->urls()) {
                        QString fName = url.toLocalFile();
                        QFileInfo info;
                        info.setFile( fName );
                        if ( !info.isFile() || !info.isReadable() )
                            continue;

                        if( info.suffix() == VST_BANK_FILE_EXTENSION && vstPtr->LoadBank(fName) ) {
                            QStandardItem *item = itemFromIndex(index);
                            if(item)
                                item->setData(fName,UserRoles::bankFile);
                        }

                        if( info.suffix() == VST_PROGRAM_FILE_EXTENSION && vstPtr->LoadProgram(fName) ) {
                            QStandardItem *item = itemFromIndex(index);
                            if(item)
                                item->setData(fName,UserRoles::programFile);
                        }

                        if( info.suffix() == "dll" ) {
                            QSharedPointer<Connectables::Container> cntPtr = myHost->objFactory->GetObjectFromId(vstPtr->GetContainerId()).staticCast<Connectables::Container>();
                            if(!cntPtr) {
                                debug("HostModel::dropMimeData replace plugin, container not found")
                                return false;
                            }

                            ObjectInfo infoVst;
                            infoVst.nodeType = NodeType::object;
                            infoVst.objType = ObjType::VstPlugin;
                            infoVst.filename = fName;
                            infoVst.name = fName;

                            QSharedPointer<Connectables::Object> objPtr = myHost->objFactory->NewObject(infoVst);
                            if(objPtr.isNull()) {
                                if(Connectables::VstPlugin::shellSelectView) {
                                    Connectables::VstPlugin::shellSelectView->cntPtr=cntPtr;
                                } else {
                                    debug("HostModel::dropMimeData replace plugin, new object not found")
                                }
                                return false;
                            }

                            cntPtr->UserAddObject(objPtr);
                            cntPtr->CopyCablesFromObj(objPtr, vstPtr);
                            vstPtr->CopyStatusTo(objPtr);
                            cntPtr->ParkObject(vstPtr);

                        }
                    }
                    return true;
                }
            }
        }

        //connect a pin by drag&drop
//        case NodeType::pin :
//        {
//            if(data->hasFormat("application/x-pin")) {
//                ConnectionInfo parentInfo = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();

//                //                  pin   . listpin .obj    . container
//                QModelIndex index = index.parent().parent().parent();
//                if(parentInfo.bridge)
//                    index=index.parent();

//                int cntId = index.data(Qt::DisplayRole).toInt();;
//                QSharedPointer<Connectables::Object> cntPtr = myHost->objFactory->GetObjectFromId(cntId);
//                if(cntPtr.isNull()) {
//                    debug(QString("HostModel::dropMimeData NodeType::pin the container %1 is deleted").arg(cntId).toAscii())
//                    return false;
//                }
//                Connectables::Container *cnt = static_cast<Connectables::Container*>(cntPtr.data());


//                QByteArray bytes = data->data("application/x-pin");
//                ConnectionInfo droppedInfo;

//                QDataStream stream(&bytes,QIODevice::ReadOnly);
//                stream >> droppedInfo;

//                if(droppedInfo.CanConnectTo(parentInfo)) {
//                    if(droppedInfo.direction == PinDirection::Output)
//                        cnt->UserAddCable(droppedInfo, parentInfo);
//                     else
//                        cnt->UserAddCable(parentInfo, droppedInfo);
//                     return true;
//                 }
//            }
//            break;
//        }
        default:
            return false;


    }

    return false;
}

bool HostModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    ObjectInfo info = index.data(UserRoles::objInfo).value<ObjectInfo>();
    switch(info.nodeType) {
        case NodeType::object :
        //case NodeType::container :
        {
            int objId = index.data(UserRoles::value).toInt();
            if(!objId) {
                debug("HostModel::setData NodeType::object has no object Id")
                return false;
            }
            QSharedPointer<Connectables::Object> objPtr = myHost->objFactory->GetObjectFromId(objId);
            if(objPtr.isNull()) {
                debug(QString("HostModel::setData NodeType::object the object %1 is deleted").arg(objId).toAscii())
                return false;
            }

//            if(role == UserRoles::editorVisible)
//                objPtr->OnEditorVisibilityChanged( value.toBool() );

            //save vst bank file
            if(role == UserRoles::bankFile) {
                objPtr.staticCast<Connectables::VstPlugin>()->SaveBank( value.toString() );
                QStandardItem *item = itemFromIndex(index);
                if(item)
                    item->setData(value,UserRoles::bankFile);
                return true;
            }

            //save vst program file
            if(role == UserRoles::programFile) {
                objPtr.staticCast<Connectables::VstPlugin>()->SaveProgram( value.toString() );
                QStandardItem *item = itemFromIndex(index);
                if(item)
                    item->setData(value,UserRoles::programFile);
                return true;
            }
            break;
        }

        case NodeType::pin :
        {
            ConnectionInfo pinInfo = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();
            if(pinInfo.type==PinType::Parameter) {
                if(role==UserRoles::value) {
                    bool ok=true;
                    float newVal = value.toFloat(&ok);// item->data(Qt::DisplayRole).toFloat(&ok);
                    if(!ok) {
                        debug("HostModel::setData pin can't convert value to float")
                        return false;
                    }

                    if(newVal>1.0f) newVal=1.0f;
                    if(newVal<.0f) newVal=.0f;
                    Connectables::ParameterPin* pin = static_cast<Connectables::ParameterPin*>(myHost->objFactory->GetPin(pinInfo));
                    if(!pin) {
                        return false;
                    }
                    pin->ChangeValue( newVal );

                    QStandardItem *item = itemFromIndex(index);
                    if(item)
                        item->setData(newVal,role);
                    return true;
                }
            }
            break;
        }
        case NodeType::pinLimit :
        {
            ConnectionInfo pinInfo = index.parent().data(UserRoles::connectionInfo).value<ConnectionInfo>();
            if(pinInfo.type==PinType::Parameter) {
                if(role==UserRoles::value) {
                    Connectables::ParameterPin* pin = static_cast<Connectables::ParameterPin*>(myHost->objFactory->GetPin(pinInfo));
                    ObjectInfo info = index.data(UserRoles::objInfo).value<ObjectInfo>();
                    pin->SetLimit(info.objType,value.toFloat());

                    QStandardItem *item = itemFromIndex(index);
                    if(item)
                        item->setData(value,role);
                }
            }
        }
        default:
            break;

    }

    return QStandardItemModel::setData(index,value,role);
}

//bool HostModel::setItemData ( const QModelIndex & index, const QMap<int, QVariant> & roles )
//{
//    return true;
//}
