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

#include "pin.h"
#include "../mainhost.h"

using namespace Connectables;

Pin::Pin(Object *parent,PinType::Enum type, PinDirection::Enum direction, int number, bool bridge) :
    QObject(parent),
    connectInfo(parent->GetIndex(),type,direction,number,bridge),
    value(.0f),
    falloff(.05f),
    stepSize(.1f),
    parent(parent),
    visible(false),
    parentNode(0),
    modelPin(0),
    closed(false)
{
    setObjectName(QString("pin:%1:%2:%3:%4").arg(connectInfo.objId).arg(connectInfo.type).arg(connectInfo.direction).arg(connectInfo.pinNumber));
}

Pin::~Pin()
{
//    SetVisible(false);
    Close();
}

void Pin::setObjectName(const QString &name)
{
    if(modelPin)
        modelPin->setData(name,Qt::DisplayRole);

    QObject::setObjectName(name);
}

bool Pin::event(QEvent *event)
{
    if (event->type() == Event::PinMessage) {
        PinMessageEvent *e = static_cast<PinMessageEvent *>(event);
        ReceiveMsg(e->msgType,e->data);
        return true;
    }
    return QObject::event(event);
}

void Pin::SendMsg(int msgType,void *data)
{
    MainHost::Get()->SendMsg(connectInfo,(PinMessage::Enum)msgType,data);
}

//QString Pin::GetDisplayedText()
//{
//    QString retStr;
//    txtMutex.lock();
//    retStr = displayedText;
//    txtMutex.unlock();
//    return retStr;
//}

//void Pin::SetDisplayedText(const QString &txt)
//{
//    txtMutex.lock();
//    displayedText = txt;
//    txtMutex.unlock();
//}

void Pin::SetParentModelNode(QStandardItem *parent_Node)
{
    closed=false;

    bool wasVisible=visible;
    SetVisible(false);

    parentNode=parent_Node;
    connectInfo.container=parentNode->parent()->parent()->data(UserRoles::value).toInt();

    if(wasVisible)
        SetVisible(true);
}

void Pin::Close()
{
    QMutexLocker l(&objMutex);
    disconnect(MainHost::Get()->updateViewTimer,SIGNAL(timeout()),
            this,SLOT(updateView()));
    parentNode=0;
    modelPin=0;
    closed=true;
}

void Pin::SetVisible(bool vis)
{
    if(visible==vis)
        return;

    QMutexLocker l(&objMutex);

    visible=vis;

    if(closed)
        return;

    if(visible) {
        if(parentNode) {
            modelPin = new QStandardItem("pin");
            modelPin->setData(objectName(),Qt::DisplayRole);
            modelPin->setData(falloff,UserRoles::falloff);
            modelPin->setData(GetValue(),UserRoles::value);
            modelPin->setData( QVariant::fromValue(ObjectInfo(NodeType::pin)),UserRoles::objInfo);
            modelPin->setData(QVariant::fromValue(connectInfo),UserRoles::connectionInfo);
            modelPin->setData(stepSize,UserRoles::stepSize);
//            MainHost::Get()->modelProxy->Add(connectInfo.GetId(),modelPin,parentNode);
            parentNode->appendRow(modelPin);
        }

        if(connectInfo.type!=PinType::Bridge) {
            connect(MainHost::Get()->updateViewTimer,SIGNAL(timeout()),
                    this,SLOT(updateView()));
        }
    } else {
        if(modelPin) {
            //remove cables from pin
            QSharedPointer<Object> cnt = ObjectFactory::Get()->GetObjectFromId(connectInfo.container);
            if(!cnt.isNull()) {
                static_cast<Container*>(cnt.data())->RemoveCableFromPin(connectInfo);
            }

            //remove pin
            if(connectInfo.type!=PinType::Bridge) {
                disconnect(MainHost::Get()->updateViewTimer,SIGNAL(timeout()),
                        this,SLOT(updateView()));
            }

            parentNode->removeRow(modelPin->row());
//            MainHost::Get()->modelProxy->Remove(connectInfo.GetId());
            modelPin=0;
        }
    }
}

void Pin::updateView()
{
    QMutexLocker l(&objMutex);

    if(!closed && visible) {
        modelPin->setData(GetValue(),UserRoles::value);
        if(!displayedText.isEmpty()) modelPin->setData(displayedText,Qt::DisplayRole);
    }
//        MainHost::Get()->modelProxy->Update(connectInfo,UserRoles::value,GetValue());
}
