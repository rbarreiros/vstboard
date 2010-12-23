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

#include "sceneview.h"
#include "globals.h"
#include "../connectables/objectfactory.h"
#include "objectview.h"
#include "containerview.h"
#include "connectableobjectview.h"
#include "maincontainerview.h"
#include "bridgeview.h"
#include "connectablepinview.h"
#include "minmaxpinview.h"
#include "bridgepinview.h"
#include "../connectables/container.h"
#include "../connectables/objectfactory.h"
#include "../mainhost.h"
#include <QSplitter>

using namespace View;

SceneView::SceneView(MainHost *myHost,Connectables::ObjectFactory *objFactory, MainGraphicsView *viewHost, MainGraphicsView *viewProject, MainGraphicsView *viewProgram, MainGraphicsView *viewInsert,QWidget *parent) :
        QAbstractItemView(parent),
        viewHost(viewHost),
        viewProject(viewProject),
        viewProgram(viewProgram),
        viewInsert(viewInsert),
        sceneHost(0),
        sceneProgram(0),
        sceneInsert(0),
        objFactory(objFactory),
        myHost(myHost)
{
    setHidden(true);
    timerFalloff = new QTimer(this);
    timerFalloff->start(50);

    sceneHost = new QGraphicsScene(this);
    sceneProject = new QGraphicsScene(this);
    sceneProgram = new QGraphicsScene(this);
    sceneInsert = new QGraphicsScene(this);

    //we need a root object to avoid a bug when the scene is empty
    rootObjHost = new QGraphicsRectItem(0, sceneHost);
    rootObjProject = new QGraphicsRectItem(0, sceneProject);
    rootObjProgram = new QGraphicsRectItem(0, sceneProgram);
    rootObjInsert = new QGraphicsRectItem(0, sceneInsert);

    viewHost->setScene(sceneHost);
    viewProject->setScene(sceneProject);
    viewProgram->setScene(sceneProgram);
    viewInsert->setScene(sceneInsert);
}

void SceneView::SetParkings(QWidget *progPark, QWidget *groupPark)
{
    progParking = progPark;
    groupParking = groupPark;
}

void SceneView::dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight  )
{
    QAbstractItemView::dataChanged(topLeft, bottomRight);

    QModelIndex tmpIndex = topLeft;
    do {
        ObjectInfo info = tmpIndex.data(UserRoles::objInfo).value<ObjectInfo>();

        switch( info.nodeType ) {
        case NodeType::object :
        case NodeType::container :
            {
                ObjectView *view = static_cast<ObjectView*>(hashItems.value(tmpIndex,0));
                if(!view) {
                    debug("SceneView::dataChanged object not found")
                            return;
                }
                view->UpdateModelIndex();
                break;
            }
        case NodeType::pin :
            {
                PinView *view = static_cast<PinView*>(hashItems.value(tmpIndex,0));
                if(!view) {
                    debug("SceneView::dataChanged pin not found")
                            return;
                }
                view->UpdateModelIndex(tmpIndex);
                break;
            }
//        case NodeType::editor :
//            {
//                ObjectView *view = static_cast<ObjectView*>(hashItems.value(tmpIndex.parent(),0));
//                if(!view) {
//                    debug("SceneView::dataChanged editor not found")
//                            return;
//                }
//                view->SetEditorIndex(tmpIndex);
//                break;
//            }
//        case NodeType::learning :
//            {
//                ObjectView *view = static_cast<ObjectView*>(hashItems.value(tmpIndex.parent(),0));
//                if(!view) {
//                    debug("SceneView::dataChanged editor not found")
//                            return;
//                }
//                view->SetLearningIndex(tmpIndex);
//                break;
//            }
        case NodeType::pinLimit :
            {
                PinView *view = static_cast<PinView*>(hashItems.value(tmpIndex.parent(),0));
                if(!view) {
                    debug("SceneView::dataChanged pin not found")
                            return;
                }
                static_cast<MinMaxPinView*>(view)->UpdateLimitModelIndex(tmpIndex.parent());
                break;
            }
        default:
            break;
        }

        if ( tmpIndex == bottomRight )
            break;
        tmpIndex = traverseTroughIndexes ( tmpIndex );
    } while ( tmpIndex.isValid() );
}

QModelIndex SceneView::traverseTroughIndexes ( QModelIndex index ) {
    // 1. dive deep into the structure until we found the bottom (not bottomRight)
    QModelIndex childIndex = model()->index ( 0,0,index );

    if ( childIndex.isValid() )
        return childIndex;

    // 2. now traverse all elements in the lowest hierarchy
    QModelIndex tmpIndex = model()->index ( index.row() +1,0,model()->parent ( index ) );//index.sibling(index.row()+1,0);
    if ( tmpIndex.isValid() )
        return tmpIndex;

    // 3. if no more childs are found, return QModelIndex()
    return QModelIndex();
}

void SceneView::rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end  )
{
    for ( int i = start; i <= end; ++i ) {
        const QPersistentModelIndex index = parent.child(i,0);

        ObjectInfo info = index.data(UserRoles::objInfo).value<ObjectInfo>();
        switch(info.nodeType) {
        case NodeType::object :
            {
                ObjectView *obj = static_cast<ObjectView*>(hashItems.value(index,0));
                if(!obj) {
                    debug("SceneView::rowsAboutToBeRemoved obj not found")
                            continue;
                }
                obj->hide();
                obj->scene()->removeItem(obj);
                obj->deleteLater();
                break;
            }
        case NodeType::container :
            {
                QGraphicsWidget *obj = static_cast<QGraphicsWidget*>( hashItems.value(index,0) );
                if(!obj) {
                    debug("SceneView::rowsAboutToBeRemoved container not found")
                            continue;
                }
                obj->hide();
                obj->scene()->removeItem(obj);
                delete obj;
                break;
            }
        case NodeType::pin :
            {
                PinView *pin = static_cast<PinView*>(hashItems.value(index,0));
                if(!pin) {
                    debug("SceneView::rowsAboutToBeRemoved pin not found")
                            continue;
                }

                ListPinsView *list = static_cast<ListPinsView*>(hashItems.value(parent,0));
                pin->hide();
                list->layout->removeItem(pin);
                pin->scene()->removeItem(pin);
                pin->deleteLater();

                ObjectView *obj = static_cast<ObjectView*>( hashItems.value(index.parent().parent(),0) );
                if(obj) {
                    obj->Shrink();
                }

                mapConnectionInfo.remove(pin->GetConnectionInfo());
                break;
            }
        case NodeType::cable :
            {
                ConnectionInfo infoOut = index.data(UserRoles::value).value<ConnectionInfo>();
                ConnectionInfo infoIn = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();

                QPersistentModelIndex ixOut =  mapConnectionInfo.value(infoOut);
                QPersistentModelIndex ixIn =  mapConnectionInfo.value(infoIn);
                PinView* pinOut = static_cast<PinView*>(hashItems.value( ixOut,0 ));
                PinView* pinIn = static_cast<PinView*>(hashItems.value( ixIn,0 ));

                CableView *cable = static_cast<CableView*>(hashItems.value(index,0));
                if(!cable) {
                    debug("SceneView::rowsAboutToBeRemoved cable not found")
                            continue;
                }

                if(pinOut)
                    pinOut->RemoveCable(cable);
                if(pinIn)
                    pinIn->RemoveCable(cable);

                cable->hide();
                cable->scene()->removeItem(cable);
                delete cable;
                break;
            }
        default:
            break;
        }
        hashItems.remove(index);
    }
    QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void SceneView::rowsInserted ( const QModelIndex & parent, int start, int end  )
{
    if(parent.row()==-1) {
        QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
        return;
    }

    if(!parent.isValid()) {
        debug("SceneView::rowsInserted parent not valid")
        return;
    }


    for ( int i = start; i <= end; ++i ) {
        QPersistentModelIndex index( parent.child(i,0) ); // model()->index( i, 0, parent );

        ObjectView *objView = 0;
        ObjectInfo info = index.data(UserRoles::objInfo).value<ObjectInfo>();

        switch(info.nodeType) {
            case NodeType::container :
            {
                switch(info.objType) {
                    case ObjType::MainContainer :
                    {
                        int objId = index.data(UserRoles::value).toInt();

                        if(objId == FixedObjId::hostContainer) {
                            hostContainerView = new MainContainerView(myHost, model());
                            objView=hostContainerView;
                            hostContainerView->setParentItem(rootObjHost);
                            connect(viewHost,SIGNAL(viewResized(QRectF)),
                                    hostContainerView,SLOT(OnViewChanged(QRectF)));
                            QTimer::singleShot(0, viewHost, SLOT(ForceResize()));
                        }

                        if(objId == FixedObjId::projectContainer) {
                            projectContainerView = new MainContainerView(myHost, model());
                            objView=projectContainerView;
                            projectContainerView->setParentItem(rootObjProject);
                            connect(viewProject,SIGNAL(viewResized(QRectF)),
                                    projectContainerView,SLOT(OnViewChanged(QRectF)));
                            QTimer::singleShot(0, viewProject, SLOT(ForceResize()));
                        }

                        if(objId == FixedObjId::programContainer) {
                            programContainerView = new MainContainerView(myHost, model());
                            objView=programContainerView;
                            programContainerView->setParentItem(rootObjProgram);
                            connect(viewProgram,SIGNAL(viewResized(QRectF)),
                                    programContainerView,SLOT(OnViewChanged(QRectF)));
                            QTimer::singleShot(0, viewProgram, SLOT(ForceResize()));
                            programContainerView->SetParking(progParking);
                        }

                        if(objId == FixedObjId::groupContainer) {
                            MainContainerView *groupContainerView = new MainContainerView(myHost, model());
                            objView=groupContainerView;
                            groupContainerView->setParentItem(rootObjInsert);
                            connect(viewInsert,SIGNAL(viewResized(QRectF)),
                                    groupContainerView,SLOT(OnViewChanged(QRectF)));
                            QTimer::singleShot(0, viewInsert, SLOT(ForceResize()));
                            groupContainerView->SetParking(groupParking);
                        }


                        break;
                    }
                    case ObjType::Container :
                    {
                        ObjectView *parentView = static_cast<ObjectView*>(hashItems.value(parent,0));
                        if(!parentView) {
                            debug("SceneView::rowsInserted parent not found")
                                    continue;
                        }
                        objView = new ContainerView(myHost, model(),parentView);
                        break;
                    }
                    default:
                        continue;
                }


                hashItems.insert( index , objView);
                connect(objView,SIGNAL(destroyed(QObject*)),
                        this,SLOT(graphicObjectRemoved(QObject*)));

                objView->SetModelIndex(index);

                break;
            }
            case NodeType::bridge :
            {

                ObjectView *parentView = static_cast<ObjectView*>(hashItems.value(parent,0));
                if(!parentView) {
                    debug("SceneView::rowsInserted parent not found")
                    continue;
                }

                switch(info.objType) {
                    case ObjType::BridgeIn :
                        objView = static_cast<ContainerView*>(parentView)->bridgeIn;
                        break;
                    case ObjType::BridgeOut :
                        objView = static_cast<ContainerView*>(parentView)->bridgeOut;
                        break;
                    case ObjType::BridgeSend :
                        objView = static_cast<MainContainerView*>(parentView)->bridgeSend;
                        break;
                    case ObjType::BridgeReturn :
                        objView = static_cast<MainContainerView*>(parentView)->bridgeReturn;
                        break;
                    default:
                        debug("SceneView::rowsInserted unknown listpin")
                        continue;
                }


                hashItems.insert( index , objView);
                connect(objView,SIGNAL(destroyed(QObject*)),
                        this,SLOT(graphicObjectRemoved(QObject*)));
                objView->SetModelIndex(index);
               // viewHost->ForceResize();
                break;
            }
            case NodeType::object :
            {
                ContainerView *parentView = static_cast<ContainerView*>(hashItems.value(parent,0));
                if(!parentView) {
                    debug("SceneView::rowsInserted object parent not found")
                            continue;
                }

                objView = new ConnectableObjectView(myHost, model(),parentView);
                hashItems.insert( index , objView);
                objView->SetModelIndex(index);
                QPointF pos = parentView->GetDropPos();
                objView->setPos(pos);
                model()->setData(index,pos,UserRoles::position);

                connect(objView,SIGNAL(destroyed(QObject*)),
                        this,SLOT(graphicObjectRemoved(QObject*)));
                break;
            }
            case NodeType::listPin :
            {
//                ObjectInfo infoParent = parent.data(UserRoles::objInfo).value<ObjectInfo>();
                ObjectView *parentView = static_cast<ObjectView*>(hashItems.value(parent,0));
                if(!parentView) {
                    debug("SceneView::rowsInserted listAudioIn parent not found")
                            continue;
                }

                ListPinsView *v=0;
                ConnectableObjectView* CntObj = static_cast<ConnectableObjectView*>(parentView);

                switch(info.objType) {
                    case ObjType::listAudioIn :
                        v = CntObj->listAudioIn;
                        break;
                    case ObjType::listAudioOut :
                        v = CntObj->listAudioOut;
                        break;
                    case ObjType::listMidiIn :
                        v = CntObj->listMidiIn;
                        break;
                    case ObjType::listMidiOut :
                        v = CntObj->listMidiOut;
                        break;
                    case ObjType::listParamIn :
                        v = CntObj->listParametersIn;
                        break;
                    case ObjType::listParamOut :
                        v = CntObj->listParametersOut;
                        break;
                    case ObjType::listBridgeIn :
                        v = CntObj->listBridgeIn;
                        break;
                    case ObjType::listBridgeOut :
                        v = CntObj->listBridgeOut;
                        break;
                    default:
                        debug("SceneView::rowsInserted unknown pin type")
                        continue;
                }

                if(!v) {
                    //debug("SceneView::rowsInserted listpin not found")
                    continue;
                }
                hashItems.insert(index, v);
                connect(v,SIGNAL(destroyed(QObject*)),
                        this,SLOT(graphicObjectRemoved(QObject*)));
                break;
            }
            case NodeType::pin :
            {
                ListPinsView *parentList = static_cast<ListPinsView*>(hashItems.value(parent,0));
                if(!parentList) {
                    debug("SceneView::rowsInserted NodeType::pin list not found")
                            continue;
                }
                ConnectionInfo pinInfo = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();
                Connectables::Pin *pin = objFactory->GetPin(pinInfo);
                if(!pin) {
                    debug("SceneView::rowsInserted NodeType::pin pin not found")
                            continue;
                }

                PinView *pinView;
                ObjectInfo parentInfo = parent.parent().data(UserRoles::objInfo).value<ObjectInfo>();
                float angle=.0f;

                if(parentInfo.nodeType == NodeType::bridge) {
                    if(parentInfo.objType==ObjType::BridgeIn || parentInfo.objType==ObjType::BridgeOut)
                        angle=3.1416f/2.0f;
                    if(parentInfo.objType==ObjType::BridgeSend || parentInfo.objType==ObjType::BridgeReturn)
                        angle=-3.1416f/2.0f;

                    pinView = static_cast<PinView*>( new BridgePinView(angle, model(), parentList, pin) );
                } else {
                    if(pinInfo.direction==PinDirection::Input)
                        angle=3.1416f;
                    if(pinInfo.direction==PinDirection::Output)
                        angle=.0f;


                    if(pinInfo.type==PinType::Parameter) {
                        MinMaxPinView *p = new MinMaxPinView(angle,model(),parentList,pin);
                        connect(timerFalloff,SIGNAL(timeout()),
                                p,SLOT(updateVu()));
                        pinView = static_cast<PinView*>(p);
                    } else {
                        ConnectablePinView *p = new ConnectablePinView(angle, model(), parentList, pin);
                        connect(timerFalloff,SIGNAL(timeout()),
                                p,SLOT(updateVu()));
                        pinView = static_cast<PinView*>(p);
                    }

                }
                pinView->SetPinModelIndex(index);

                hashItems.insert(index, pinView);
                connect(pinView,SIGNAL(destroyed(QObject*)),
                        this,SLOT(graphicObjectRemoved(QObject*)));
                mapConnectionInfo.insert(pinInfo,index);

                int pinPlace = parentList->GetPinPosition(pinInfo.pinNumber);
                parentList->layout->insertItem(pinPlace, pinView);

                parentList->layout->setAlignment(pinView,Qt::AlignTop);
                connect(pinView, SIGNAL(ConnectPins(ConnectionInfo,ConnectionInfo)),
                        this, SLOT(ConnectPins(ConnectionInfo,ConnectionInfo)));
                connect(pinView,SIGNAL(RemoveCablesFromPin(ConnectionInfo)),
                        this,SLOT(RemoveCablesFromPin(ConnectionInfo)));

                pinView->UpdateModelIndex(index);
                break;
            }
            case NodeType::pinLimit :
            {
                MinMaxPinView* pin = static_cast<MinMaxPinView*>(hashItems.value( index.parent(),0 ));
                if(!pin) {
                    debug("SceneView::rowsInserted add pinLimit, pin not found")
                    continue;
                }
                ObjectInfo info = index.data(UserRoles::objInfo).value<ObjectInfo>();
                pin->SetLimitModelIndex(info.objType, index);
                break;
            }
            case NodeType::cable :
            {
                ContainerView *cnt = static_cast<ContainerView*>(hashItems.value(parent.parent(),0));
                if(!cnt) {
                    debug("SceneView::rowsInserted add cable, container not found")
                            continue;
                }
                ConnectionInfo infoOut = index.data(UserRoles::value).value<ConnectionInfo>();
                ConnectionInfo infoIn = index.data(UserRoles::connectionInfo).value<ConnectionInfo>();

                if(!mapConnectionInfo.contains(infoOut) || !mapConnectionInfo.contains(infoIn)) {
                    debug("SceneView::rowsInserted add cable, connectioninfo not found")
                            continue;
                }

                QPersistentModelIndex ixOut =  mapConnectionInfo.value(infoOut);
                QPersistentModelIndex ixIn =  mapConnectionInfo.value(infoIn);

                PinView* pinOut = static_cast<PinView*>(hashItems.value( ixOut,0 ));
                PinView* pinIn = static_cast<PinView*>(hashItems.value( ixIn,0 ));

                if(!pinOut || !pinIn) {
                    debug("SceneView::rowsInserted addcable : pin not found")
                            continue;
                }
                CableView *cable = new CableView(infoOut,infoIn,cnt);
                pinOut->AddCable(cable);
                pinIn->AddCable(cable);
                hashItems.insert(index, cable);
                connect(cable,SIGNAL(destroyed(QObject*)),
                        this,SLOT(graphicObjectRemoved(QObject*)));
                break;
            }
            default:
                if(index.data().toString()!="cables")
                    debug("SceneView::rowsInserted nodetype not found")
                    break;

        }
    }

    QAbstractItemView::rowsInserted(parent, start, end);
}

void SceneView::graphicObjectRemoved ( QObject* obj)
{
    hashItems.remove( hashItems.key(obj) );
}

void SceneView::ConnectPins(ConnectionInfo pinOut,ConnectionInfo pinIn)
{
    QPersistentModelIndex ixOut = mapConnectionInfo.value(pinOut);
    QPersistentModelIndex ixIn = mapConnectionInfo.value(pinIn);

    //                      pin  . list   . object . container
    QModelIndex parentOut = ixOut.parent().parent().parent();
    QModelIndex parentIn = ixIn.parent().parent().parent();

    if(pinOut.bridge) parentOut = parentOut.parent();
    if(pinIn.bridge) parentIn = parentIn.parent();

    QSharedPointer<Connectables::Object> cntPtr = objFactory->GetObjectFromId(parentOut.data(UserRoles::value).toInt());

    if(pinOut.direction==PinDirection::Output)
        static_cast<Connectables::Container*>(cntPtr.data())->AddCable(pinOut,pinIn);
    else
        static_cast<Connectables::Container*>(cntPtr.data())->AddCable(pinIn,pinOut);
}

void SceneView::RemoveCablesFromPin(ConnectionInfo pin)
{
    QPersistentModelIndex ix = mapConnectionInfo.value(pin);
    QModelIndex parent = ix.parent().parent().parent();
    if(pin.bridge) parent = parent.parent();
    QSharedPointer<Connectables::Object> cntPtr = objFactory->GetObjectFromId(parent.data(UserRoles::value).toInt());
    static_cast<Connectables::Container*>(cntPtr.data())->RemoveCableFromPin(pin);
}

void SceneView::ToggleHostView(bool show)
{
    viewHost->setVisible(show);
//    emit hostShown(show);
}

void SceneView::ToggleProjectView(bool show)
{
    viewProject->setVisible(show);
//    emit projectShown(show);
}

void SceneView::ToggleProgramView(bool show)
{
    viewProgram->setVisible(show);
//    emit programShown(show);
}

void SceneView::ToggleInsertView(bool show)
{
    viewInsert->setVisible(show);
//    emit insertShown(show);
}
